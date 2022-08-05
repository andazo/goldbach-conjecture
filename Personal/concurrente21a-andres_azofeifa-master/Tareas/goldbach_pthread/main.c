// Copyright 2021 Andres Azofeifa-Garcia <andres.azofeifagarcia@ucr.ac.cr>CC-BY
// Este programa representa La Conjetura de Goldbach. En caso de que el numero
// sea par este dara dos sumas, si el valor es impar, este dara tres sumas

#include <errno.h>
#include "threads.h"

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  shared_data_t* shared_data = (shared_data_t*)
  calloc(1, sizeof(shared_data_t));

  shared_data->position = 0;
  shared_data->thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc == 2) {
    if (sscanf(argv[1], "%zu", &shared_data->thread_count) != 1 || errno) {
      fprintf(stderr, "error: Numero de hilos ingresados invalido \n");
      error = 1;
    }
  }
  /// Ingreso datos entrada Estandar
  input_casos();

  data.goldbach_safe = (int**) create_matrix(data.size, cond_safe,
  sizeof(int));  /// Creacion Matriz
  error = create_threads(shared_data);  /// Creacion de hilos y busqueda sumas
  
  for (int index = 0; index < data.size; index++) {
    if (data.goldbach_safe[index][0]%2 == 0) {  /// caso par con la conjetura fuerte
      StrongSafeMatrix();
    } else if (data.goldbach_safe[index][0]%2 != 0) {  /// caso impar con la conjetura debil
      WeakSafeMatrix();
    }
  }

  free_matrix(shared_data->thread_count, (void**)data.goldbach_safe);
  free(shared_data);
  free(data.casosPrueba);

  return error;
}