/**
 * CREATED: 03/22/16
 * AUTHOR:  Adelin A. Destain
 */

#include "beam.h"

/**
 * Assemble the local stiffness matrix for all elements.
 * 
 * @param m Mesh problem containing the information necessary to
 * assemble the global matrix.
 * 
 * @param e Pointer to an array of local stiffness matrices. Memory for the
 * arrays should be allocated before calling this function, but not necessarily
 * initialized.
 */
int local_stiffness(struct beam_problem * m, double *** e)
{
	double moi, l;
	int l_index, r_index;
	unsigned int i;
	
	moi = (*m).width * (*m).height * (*m).height * (*m).height / 12;

	for (i = 0; i < (*m).num_elements; i++)
	{
		l_index = (*m).element[i].left - 1;
		r_index = (*m).element[i].right - 1;
		l = (*m).node[r_index] - (*m).node[l_index];
		
		e[i][0][0] = 12 * (*m).young_modulus * moi / (l * l * l);
		e[i][0][1] =  6 * (*m).young_modulus * moi / (l * l);
		e[i][0][2] =-12 * (*m).young_modulus * moi / (l * l * l);
		e[i][0][3] =  6 * (*m).young_modulus * moi / (l * l);
		
		e[i][1][0] =  6 * (*m).young_modulus * moi / (l * l);
		e[i][1][1] =  4 * (*m).young_modulus * moi / (l);
		e[i][1][2] = -6 * (*m).young_modulus * moi / (l * l);
		e[i][1][3] =  2 * (*m).young_modulus * moi / (l);
		
		e[i][2][0] =-12 * (*m).young_modulus * moi / (l * l * l);
		e[i][2][1] = -6 * (*m).young_modulus * moi / (l * l);
		e[i][2][2] = 12 * (*m).young_modulus * moi / (l * l * l);
		e[i][2][3] = -6 * (*m).young_modulus * moi / (l * l);
		
		e[i][3][0] =  6 * (*m).young_modulus * moi / (l * l);
		e[i][3][1] =  2 * (*m).young_modulus * moi / (l);
		e[i][3][2] = -6 * (*m).young_modulus * moi / (l * l);
		e[i][3][3] =  4 * (*m).young_modulus * moi / (l);	
	}

	return 0;
}

/**
 * Assemble the global stiffness matrix, without constraints.
 * 
 * @param m Mesh problem containing the information necessary to
 * assemble the global matrix.
 * 
 * @param g Pointer to the global stiffness matrix. Memory for the
 * matrix must have been allocated, but not initialized.
 */
int global_stiffness(struct beam_problem * m, double *** e, double ** g)
{
	zero_matrix(g, (*m).num_nodes * 2, (*m).num_nodes * 2);

	unsigned int i;
	int q, r;
	int l_index, r_index;

	for (i = 0; i < (*m).num_elements; i++)
	{
		l_index = (*m).element[i].left - 1;
		r_index = (*m).element[i].right - 1;
		
		int indices[4];
		indices[0] = l_index * 2;
		indices[1] = l_index * 2 + 1;
		indices[2] = r_index * 2;
		indices[3] = r_index * 2 + 1;

		for (q = 0; q < 4; q++)
		{
			for (r = 0; r < 4; r++)
			{
				g[indices[q]][indices[r]] += e[i][q][r];
			}
		}
	}

	return 0;
}

/**
 * Impose constraints on the global coefficient matrix.
 * 
 * Constraints are imposed by setting the stiffness to very large values
 * (1e15) to prevent that degree of freedom from moving. The global
 * stiffness matrix is changed within the function. No duplication takes
 * place.
 * 
 * @param m Mesh problem containing the information necessary to impose
 * constraints.
 * 
 * @param g Pointer to the global stiffness matrix, after it has been
 * assembled from the local element stiffness matrices.
 */
int impose_constraints(struct beam_problem * m, double ** g)
{
	unsigned int i;

	for (i = 0; i < (*m).num_constraints; i++)
		g[(*m).constraint[i] - 1][(*m).constraint[i] - 1] = 1.0e15;

	return 0;
}
