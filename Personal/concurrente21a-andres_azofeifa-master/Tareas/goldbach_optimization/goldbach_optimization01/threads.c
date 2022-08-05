// Copyright 2021 Andres Azofeifa-Garcia <andres.azofeifagarcia@ucr.ac.cr>CC-BY
// Este programa representa La Conjetura de Goldbach. En caso de que el numero
// sea par este dara dos sumas, si el valor es impar, este dara tres sumas

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <semaphore.h>
#include <errno.h>
#include <math.h>

#include "threads.h"
int taskCount = 0;

/**
 *  @brief funcion para ingresar datos mediante entrada estandar
 *  @return data.size
 */
int64_t input_casos() {
  int64_t tam = 0;
  data.casosPrueba = malloc(1 * sizeof(*data.casosPrueba));
  assert(data.casosPrueba);
  int64_t caso_Goldbach;

  printf("Casos de Goldbach: \n");
  while (scanf("%ld", &caso_Goldbach) != EOF) {
    if (caso_Goldbach == 0) {
      break;
    }

    tam++;
    data.casosPrueba = realloc(data.casosPrueba, tam*sizeof(int64_t));
    assert(data.casosPrueba);
    data.casosPrueba[tam-1] = caso_Goldbach;
  }

  data.size = tam;
  return data.size;
}

/**
 *  @brief funcion para el manejo dinamico de hilos y calculo sumas
 *  @param data puntero a estructuras
 *  @return NULL
 */
void* startThread(void* data) {
  const private_data_t* private_data = (private_data_t*)data;

  while (1) {
    Goldbach task;

    pthread_mutex_lock(&mutexQueue);
    while (taskCount == 0) {
      pthread_cond_wait(&condQueue, &mutexQueue);
    }

    task = sums_Queue[0];
    int i;
    for (i = 0; i < taskCount - 1; i++) {
      sums_Queue[i] = sums_Queue[i + 1];
    }

    taskCount--;
    pthread_mutex_unlock(&mutexQueue);

    executeTask(&task);
    if (taskCount == 0) {
      break;
    }
  }
  return NULL;
}

/**
 *  @brief funcion para evitar condiciones de carrera y manejo de cola
 *  @param task objeto task para
 *  @return void
 */
void submitTask(Goldbach task) {
  pthread_mutex_lock(&mutexQueue);
  sums_Queue[taskCount] = task;
  taskCount++;
  pthread_mutex_unlock(&mutexQueue);
  pthread_cond_signal(&condQueue);
}

/**
 *  @brief funcion para la ejecucion de una funcion puntero que ejecuta
 *  la funcion que ejecuta las sumas de goldbach
 *  @param task objeto puntero para el llamado de valores dentro de la estructura
 *  @return void
 */
void executeTask(Goldbach* task) {
  task->goldbachFunction(task -> caso, task -> contador);
}

/**
 *  @brief funcion para la creacion de hilos y calculo de sumas
 *  de goldbach en el hilo principal
 *  @param shared_data objeto puntero a los valores compartidos dentro de esta estructura
 *  @return EXIT_SUCCESS
 */
int create_threads(shared_data_t* shared_data) {
  assert(shared_data);
  int error = EXIT_SUCCESS;

  pthread_t* threads = (pthread_t*) calloc(shared_data->thread_count,
  sizeof(pthread_t));
  private_data_t* private_data = (private_data_t*)
  calloc(shared_data->thread_count, sizeof(private_data_t));

  pthread_mutex_init(&mutexQueue, NULL);
  pthread_cond_init(&condQueue, NULL);

  if (threads && private_data) {
    for (int64_t index = 0; index < shared_data->thread_count; ++index) {
      private_data[index].thread_number = index;
      private_data[index].shared_data = shared_data;

      if (error == EXIT_SUCCESS) {
        if (pthread_create(&threads[index], /*attr*/ NULL, &startThread,
         &private_data[index]) == EXIT_SUCCESS) {
        } else {
          fprintf(stderr, "error: could not create thread %zu\n", index);
          error = 21;
          shared_data->thread_count = index;
          break;
        }
      } else {
        fprintf(stderr, "error: could not init semaphore %zu\n", index);
        error = 22;
        shared_data->thread_count = index;
        break;
      }
    }

    for (int64_t i = 0; i < data.size; i++) {
      Goldbach t = {
        .goldbachFunction = &execute_Goldbach,
        .caso = data.casosPrueba[i],
        .contador = taskCount
      };
      submitTask(t);
    }

    for (int64_t index = 0; index < shared_data->thread_count; ++index) {
      pthread_join(threads[index], NULL);
    }

    pthread_mutex_destroy(&mutexQueue);
    pthread_cond_destroy(&condQueue);
    free(threads);
    free(private_data);
  }
  return error;
}
