#include "matrix.h"
#include "mesh.h"

/* int local_stiffness(struct beam_problem * beam, double *** element_matrices); */
int local_stiffness(struct beam_problem * beam, double *** local_matrices);
int global_stiffness(struct beam_problem * beam, double *** local_matrices, double ** global_matrix);
int impose_constraints(struct beam_problem * beam, double ** global_matrix);

