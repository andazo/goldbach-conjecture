// Copyright 2021 Andres Azofeifa-Garcia <andres.azofeifagarcia@ucr.ac.cr>CC-BY
// Este programa representa La Conjetura de Goldbach. En caso de que el numero
// sea par este dara dos sumas, si el valor es impar, este dara tres sumas

#ifndef MATRIX_H
#define MATRIX_H
#define cond_safe 500000 * 50
#define sizeQueue 50

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <math.h>

#include "threads.h"

void** create_matrix(int64_t row_count, int64_t col_count,
int64_t element_size);
void free_matrix(const int64_t row_count, void** matrix);

#endif  // MATRIX_H

