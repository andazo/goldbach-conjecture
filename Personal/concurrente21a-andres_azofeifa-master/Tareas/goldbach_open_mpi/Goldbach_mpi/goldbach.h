// Copyright 2021 Andres Azofeifa-Garcia <andres.azofeifagarcia@ucr.ac.cr>CC-BY
// Este programa representa La Conjetura de Goldbach. En caso de que el numero
// sea par este dara dos sumas, si el valor es impar, este dara tres sumas

#ifndef GOLDBACH_H
#define GOLDBACH_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

typedef struct Goldbach {
  int64_t* casosPrueba;
  int64_t sums;
  int64_t size;
  int** goldbach_safe;
  int64_t thread_count;
  int64_t* threadArray;
} Goldbach;

Goldbach data;

void fuerte(int64_t valor, int64_t id);
void debil(int64_t valor, int64_t id);
void execute_Goldbach(int64_t caso, int64_t id);
void printSafeMatrix();
void run(int64_t* workers_Array, int argc, char* argv[]);

#endif  // GOLDBACH_H
