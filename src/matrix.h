/**
 * CREATED: 03/22/16
 * AUTHOR:  Adelin A. Destain
 */

#include <stdlib.h>
#include <stdio.h>

double ** generate_matrix(int rows, int cols);
int zero_matrix(double ** m, int rows, int columns);
int print_matrix(FILE * out, double ** m, int num_rows, int num_cols);
int print_array(FILE * out, double * array, int size);
int destroy_matrix(double ** matrix, int rows);

/* These functions allocate memory for you. */
double ** ludecomp(double ** matrix, int dimension);
double * lubacksub_matrix(double ** matrix, double * array, int dimension);
double * backsub_matrix(double ** matrix, double * array, int dimension);
double * fwdsub_matrix(double ** matrix, double * array, int dimension);
double ** invert_matrix(double ** matrix, int dimension);
double * solve_matrix(double ** matrix, double * array, int dimension);

/* Assumes the memory is allocated by the user. */
/*
int ludecomp(double ** result, double ** matrix, int dimension);
int lubacksub_matrix(double * result, double ** matrix, double * array, int dimension);
int backsub_matrix(double * result, double ** matrix, double * array, int dimension);
int fwdsub_matrix(double * result, double ** matrix, double * array, int dimension);
int invert_matrix(double ** result, double ** matrix, int dimension);
int solve_matrix(double * result, double ** matrix, double * array, int dimension);
*/
