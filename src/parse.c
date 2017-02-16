/**
 * CREATED: 03/22/16
 * AUTHOR:  Adelin A. Destain
 */

#include "parse.h"

/**
 * Parse a beam file.
 *
 * @param filename A string of characters containing the path to the file
 * to be solved. The path can be absolute or relative.
 *
 * @param new A pointer to a beam problem structure that has already been
 * initialized with the init_beam function.
 *
 * @return 0 on success. -3 if element read fails. -4 if pointload read fails.
 * -2 if constraints read fails. -5 if a bad section header is encountered.
 * -6 if an unknown property is read.
 */
int parse_problem(const char * filename, struct beam_problem * new)
{
	FILE * fd;
	char section_header[256];
	int retval;
	
	fd = fopen(filename, "r");
	if (fd == NULL)
		return -1;
	
	while (fscanf(fd, "%s", section_header) != EOF)
	{
		if (strcmp(section_header, "[Variables]") == 0)
			retval = read_properties(fd, new);
		else if (strcmp(section_header, "[Nodes]") == 0)
			retval = read_beam(fd, new);
		else if (strcmp(section_header, "[Elements]") == 0)
			retval = read_elements(fd, new);
		else if (strcmp(section_header, "[PointLoads]") == 0)
			retval = read_pointloads(fd, new);
		else if (strcmp(section_header, "[Constraints]") == 0)
			retval = read_constraints(fd, new);
		else
			retval = -5;
		if (retval != 0)
			break;
	}

	fclose(fd);
	
	return retval;
}

/**
 * Read the properties section of a beam problem file.
 *
 * This function expects a certain number of variables in this section.
 * If some variables are missing or there are too many variables,
 * the parsing of the beam problem file will fail. The user should not have
 * to use this function. It should only be used by the parse_problem function.
 *
 * @param fd Pointer to the file to read from. This includes stdin.
 *
 * @param new Pointer to a beam problem structure. Since the properties section
 * should be the first section to be parsed, the beam problem structure
 * should be blank.
 */
int read_properties(FILE * fd, struct beam_problem * new)
{
	unsigned int i;
	char var_name[256];
	for (i = 0; i < NUM_VARS; i++)
	{
		fscanf(fd, "%s = ", var_name);
		if (strcmp(var_name, "num_nodes") == 0)
			fscanf(fd, "%ui\n", &(*new).num_nodes);
		else if (strcmp(var_name, "num_elements") == 0)
			fscanf(fd, "%ui\n", &(*new).num_elements);
		else if (strcmp(var_name, "height") == 0)
			fscanf(fd, "%le\n", &(*new).height);
		else if (strcmp(var_name, "width") == 0)
			fscanf(fd, "%le\n", &(*new).width);
		else if (strcmp(var_name, "young_modulus") == 0)
			fscanf(fd, "%le\n", &(*new).young_modulus);
		else if (strcmp(var_name, "num_constraints") == 0)
			fscanf(fd, "%ui\n", &(*new).num_constraints);
		else if (strcmp(var_name, "num_ptloads") == 0)
			fscanf(fd, "%ui\n", &(*new).num_ptloads);
		else
			return -6;
	}
	return 0;
}

/**
 * Read the nodes of a beam problem file.
 *
 * This function should not be used by the end user. It should only be used by the
 * parse_problem function.
 *
 * @param fd Pointer to the open file to read from. This includes stdin.
 *
 * @param new Pointer to a beam problem structure. The num_nodes variable in the
 * beam problem structure should be greater than zero.
 */
int read_beam(FILE * fd, struct beam_problem * new)
{
	unsigned int i, index;
	double temporary;
	if ((*new).num_nodes == 0)
		return -2;

	(*new).node = (double *)malloc(sizeof((*new).node) * (*new).num_nodes);
	for (i = 0; i < (*new).num_nodes; i++)
	{
		fscanf(fd, "%u %le", &index, &temporary);
		(*new).node[index - 1] = temporary;
	}	
	return 0;
}

/**
 * Read the elements of a beam problem file, and the nodes they connect.
 *
 * This function should not be used by the end user. It should only be used by the
 * parse_problem function.
 *
 * @param fd Pointer to the open file to read from. This includes stdin.
 *
 * @param new Pointer to the beam problem structure. The num_elements variable in
 * the beam problem structure should be equal to the number of nodes minus one.
 *
 * @return Zero on success, -1 if the number of elements is equal to zero.
 */
int read_elements(FILE * fd, struct beam_problem * new)
{
	unsigned int i, left, right, index;
	if ((*new).num_elements == 0)
		return -3;

	(*new).element = (struct element_struct *)malloc(sizeof(struct element_struct) * (*new).num_elements);
	for (i = 0; i < (*new).num_elements; i++)
	{
		fscanf(fd, "%u %u %u", &index, &left, &right);
		(*new).element[index - 1].left = left;
		(*new).element[index - 1].right = right;
	}	
	return 0;
}

/**
 * Read the point loads of a beam problem file, and their magnitude.
 *
 * This function should not be used by the end user. It should only be used by the
 * parse_problem function.
 *
 * @param fd Pointer to the open file to read from. This includes stdin.
 *
 * @param new Pointer to the beam problem structure. The num_ptloads variable should
 * be greater than zero if this section exists, and equal to zero if it does not.
 *
 * @return Zero on success, -1 if the number of pointloads is equal to zero.
 */
int read_pointloads(FILE * fd, struct beam_problem * new)
{
	unsigned int i, index;
	double temporary;
	if ((*new).num_ptloads == 0)
		return -4;
	
	(*new).ptload = (struct ptload_struct *)malloc(sizeof(struct ptload_struct) * (*new).num_ptloads);
	for (i = 0; i < (*new).num_ptloads; i++)
	{
		fscanf(fd, "%u %le", &index, &temporary);
		(*new).ptload[i].dof = index;
		(*new).ptload[i].load = temporary;
	}
	return 0;
}

/**
 * Read the constrained degrees of freedom from a beam problem file.
 * 
 * This function should not be used by the end user, It should only be used by the
 * parse_problem function.
 *
 * @param fd Pointer to the open file to read from. This includes stdin.
 *
 * @param new Pointer to the beam problem structure. The num_constraints variable
 * should be greater than zero if this section exists, and equal to zero if it
 * does not.
 *
 * @return Zero on success, -2 if the number of constraints is equal to zero.
 */
int read_constraints(FILE * fd, struct beam_problem * new)
{

	unsigned int i;
	if ((*new).num_constraints == 0)
		return -2;
	
	(*new).constraint = (unsigned int*)malloc(sizeof(unsigned int) * (*new).num_constraints);
	for (i = 0; i < (*new).num_constraints; i++)
	{
		fscanf(fd, "%u", &(*new).constraint[i]);
	}
	return 0;
}

/**
 * Initialize the values of beam problem variables.
 *
 * Because some of the variables are allocated memory, this function should 
 * only be called on beam_problem structure variables that have never been 
 * initialized, or beam_problem structure variables that have been passed 
 * through the destroy_beam function.
 *
 * @param beam Pointer to the beam problem structure.
 */
int init_beam(struct beam_problem * beam)
{
	(*beam).young_modulus = 0;
	(*beam).height = 0.0;
	(*beam).width = 0.0;
	(*beam).num_constraints = 0.0;
	(*beam).constraint = NULL;
	(*beam).num_ptloads = 0;
	(*beam).num_elements = 0;
	(*beam).element = NULL;
	(*beam).num_nodes = 0;
	(*beam).node = NULL;
	
	return 0;
}

/**
 * Display the values of beam problem variables.
 *
 * @param out File open for writing to which the variables will be printed.
 * This includes stdout.
 *
 * @param beam Pointer to the beam problem structure to be displayed.
 */
int disp_beam(FILE * out, struct beam_problem * beam)
{
	unsigned int i = 0;
	fprintf(out, "young_modulus = %le\n", (*beam).young_modulus);
	fprintf(out, "height = %le\n", (*beam).height);
	fprintf(out, "width = %le\n", (*beam).width);
	fprintf(out, "num_ptloads = %u\n", (*beam).num_ptloads);
	fprintf(out, "num_elements = %u\n", (*beam).num_elements);
	for (i = 0; i < (*beam).num_elements; i++)
		fprintf(out, "%u\tleft = %u\tright = %u\n", i + 1, (*beam).element[i].left, (*beam).element[i].right);
	fprintf(out, "num_nodes = %u\n", (*beam).num_nodes);
	for (i = 0; i < (*beam).num_nodes; i++)
		fprintf(out, "%u\tx = %le\n", i + 1, (*beam).node[i]);
	fprintf(out, "num_ptloads = %u\n", (*beam).num_ptloads);
	for (i = 0; i < (*beam).num_ptloads; i++)
		fprintf(out, "%u\tdof = %u\tload = %le\n", i + 1, (*beam).ptload[i].dof, (*beam).ptload[i].load);
	
	return 0;
}

/**
 * Free the memory allocated for various arrays in a beam problem structure.
 *
 * The variable passed to this function can then be reused for a different beam
 * problem if it is also passed to the init_beam function after it is passed to
 * this function.
 *
 * @param beam Pointer to the beam problem structure to be destroyed.
 */
int destroy_beam(struct beam_problem * beam)
{
	free((void*)(*beam).constraint);
	free((void*)(*beam).element);
	free((void*)(*beam).node);
	init_beam(beam);
	
	return 0;
}
