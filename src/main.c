/**
 * CREATED: 03/22/16
 * AUTHOR:  Adelin A. Destain
 */

#include "main.h"

int main (int argc, const char ** argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: feasolver [OPTIONS] FILENAMES\n");
		return 1;
	}

	struct beam_problem beam;
	double ** k;
	int retval;
	unsigned int i, j, size;
	FILE * outfile = stdout;
	const char * problem[16];
	unsigned int num_problems = 0;

	/* Parse the command line arguments. Determine whether they are
	   options or a filepath. */
	for (i = 1; (int)i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			switch(argv[i][1])
			{
				case 'o': 
				{
					outfile = fopen(argv[++i], "w");
					if (outfile == NULL)
					{
						fprintf(stderr, "ERROR: Could not open output file '%s'\n", argv[i]);
						return 2;
					}
					break;
				}
				default:
					fprintf(stderr, "Unknown option '%s'\n", argv[i]);
			}
		}
		else 
			problem[num_problems++] = argv[i];
	}

	/* Go through each of the problem files one by one and solve them, print results to the
	   screen. */
	for (i = 0; i < num_problems; i++)
	{
		init_beam(&beam);

		if ((retval = parse_problem(problem[i], &beam)) == 0)
		{
			disp_beam(outfile, &beam);
			fprintf(outfile, "\n");

			size = beam.num_nodes * 2;
			double ** e[beam.num_elements];

			k = generate_matrix(size, size);

			/* Assemble the local stiffness matrices. */
			for (j = 0; j < beam.num_elements; j++)
				e[j] = generate_matrix(2 * NUM_DOF, 2 * NUM_DOF);
			local_stiffness(&beam, e);

			/* Print the local stiffness matrices. */
			for (j = 0; j < beam.num_elements; j++)
			{
				fprintf(outfile, "Element %i stiffness matrix\n", j + 1);
				print_matrix(outfile, e[j], 2 * NUM_DOF, 2 * NUM_DOF);
				fprintf(outfile, "\n");
			}

			/* Assemble the local matrices into a global stiffness matrix. */
			global_stiffness(&beam, e, k);
			fprintf(outfile, "Global stiffness matrix BEFORE constraints.\n");
			print_matrix(outfile, k, size, size);
			fprintf(outfile, "\n");

			/* Impose constraints. */
			impose_constraints(&beam, k);
			fprintf(outfile, "Global stiffness matrix AFTER constraints.\n");
			print_matrix(outfile, k, size, size);
			fprintf(outfile, "\n");

			/* Assemble the global force array and apply point loads. */
			double forces[size];
			for (j = 0; j < size; j++)
				forces[j] = 0;
			
			for (j = 0; j < beam.num_ptloads; j++)
				forces[beam.ptload[j].dof - 1] = beam.ptload[j].load;
			fprintf(outfile, "Point force vector.\n");
			print_array(outfile, forces, size);
			fprintf(outfile, "\n");

			/* Solve the system of equations. */
			double * solution = solve_matrix(k, forces, size);
			fprintf(outfile, "Nodal displacements.\n");
			for (j = 0; j < beam.num_nodes * 2; j+=2)
				fprintf(outfile, "%i %15e\t%15e\n", j/2 + 1, solution[j], solution[j + 1]);
			fprintf(outfile, "\n");

			/* Free dynamically allocated memory. */
			for (j = 0; j < beam.num_elements; j++)
				destroy_matrix(e[j], 2 * NUM_DOF);

			destroy_matrix(k, size);
		}
		else
			fprintf(stderr, "Invalid problem file '%s'. Return value %i.\n", problem[i], retval);

		/* Free the memory allocated for the beam problem. */
		destroy_beam(&beam);
	}

	if (outfile != stdout) /* outfile != NULL error already checked for when parsing args. */
		fclose(outfile);

	return 0;
}
