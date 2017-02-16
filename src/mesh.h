/**
 * CREATED: 03/22/16
 * AUTHOR:  Adelin A. Destain
 */

#ifndef MESH_H
#define MESH_H

#define NUM_DOF 2

struct element_struct
{
	unsigned int left, right;
};

struct ptload_struct
{
	unsigned int dof;
	double load;
};

struct beam_problem
{
	double young_modulus;
	double height, width;

	unsigned int num_constraints;
	unsigned int * constraint;

	unsigned int num_ptloads;
	struct ptload_struct * ptload;
	
	unsigned int num_elements;
	struct element_struct * element;
	
	unsigned int num_nodes;
	double * node;
};

#endif
