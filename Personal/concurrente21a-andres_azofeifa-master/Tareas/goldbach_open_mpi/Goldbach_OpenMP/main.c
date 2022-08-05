// Copyright 2021 Andres Azofeifa-Garcia <andres.azofeifagarcia@ucr.ac.cr>CC-BY
// Este programa representa La Conjetura de Goldbach. En caso de que el numero
// sea par este dara dos sumas, si el valor es impar, este dara tres sumas

#include <omp.h>
#include <stdio.h>
#include <inttypes.h>

#include "goldbach.h"
#include "common.h"
#include "matrix.h"

int main(int argc, char* argv[]) {
  data.thread_count = omp_get_max_threads();
  if (argc >= 2) {
    data.thread_count = atoi(argv[1]);
  }

  input_casos();
  data.goldbach_safe = (int**) create_matrix(data.size, cond_safe,
  sizeof(int));  /// Creacion Matriz

  for (int64_t index = 0; index < data.size; index++) {
    execute_Goldbach(data.casosPrueba[index], index);
  }

  printSafeMatrix();
  free_matrix(data.thread_count, (void**)data.goldbach_safe);
  free(data.casosPrueba);

  return 0;
}
