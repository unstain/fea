/**
 * CREATED: 03/22/16
 * AUTHOR:  Adelin A. Destain
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "beam.h"

#define NUM_VARS 7

int init_beam(struct beam_problem * beam);
int parse_problem(const char * filename, struct beam_problem * beam);
int read_properties(FILE * fd, struct beam_problem * beam);
int read_beam(FILE * fd, struct beam_problem * beam);
int read_elements(FILE * fd, struct beam_problem * beam);
int read_pointloads(FILE * fd, struct beam_problem * beam);
int read_constraints(FILE * fd, struct beam_problem * beam);
int disp_beam(FILE * out, struct beam_problem * beam);
int destroy_beam(struct beam_problem * beam);
