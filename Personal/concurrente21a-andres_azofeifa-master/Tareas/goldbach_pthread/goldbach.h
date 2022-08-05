// Copyright 2021 Andres Azofeifa-Garcia <andres.azofeifagarcia@ucr.ac.cr>CC-BY
// Este programa representa La Conjetura de Goldbach. En caso de que el numero
// sea par este dara dos sumas, si el valor es impar, este dara tres sumas

#ifndef GOLDBACH_H
#define GOLDBACH_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "threads.h"
#include "matrix.h"

int esPrimo(int64_t valor);
void fuerte(int64_t valor, int64_t thread_id);
void debil(int64_t valor, int64_t thread_id);
void execute_Goldbach(int64_t caso, int64_t thread_id);
void StrongSafeMatrix();
void WeakSafeMatrix();

#endif  // GOLDBACH_H
