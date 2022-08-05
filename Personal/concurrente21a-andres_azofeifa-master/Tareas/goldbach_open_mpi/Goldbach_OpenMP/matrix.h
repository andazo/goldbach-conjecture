// Copyright 2021 Andres Azofeifa-Garcia <andres.azofeifagarcia@ucr.ac.cr>CC-BY
// Este programa representa La Conjetura de Goldbach. En caso de que el numero
// sea par este dara dos sumas, si el valor es impar, este dara tres sumas

#ifndef MATRIX_H
#define MATRIX_H
#define cond_safe 1000000 * 10

#include <stdio.h>
#include <inttypes.h>

void** create_matrix(int64_t row_count, int64_t col_count,
int64_t element_size);
void free_matrix(const int64_t row_count, void** matrix);

#endif  // MATRIX_H
