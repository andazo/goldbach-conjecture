// Copyright 2021 Andres Azofeifa-Garcia <andres.azofeifagarcia@ucr.ac.cr>CC-BY
// Este programa representa La Conjetura de Goldbach. En caso de que el numero
// sea par este dara dos sumas, si el valor es impar, este dara tres sumas

#ifndef THREADS_H
#define THREADS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <math.h>

#include "goldbach.h"
#include "matrix.h"

typedef struct Goldbach {
  void (*goldbachFunction)(int64_t, int64_t);
  int64_t* casosPrueba;
  int64_t caso;
  int64_t contador;
  int64_t size;
  int** goldbach_safe;
} Goldbach;

typedef struct shared_data_t {
  int64_t thread_count;
  int64_t position;
} shared_data_t;

typedef struct private_data_t {
  int64_t thread_number;
  shared_data_t* shared_data;
} private_data_t;

Goldbach data;
Goldbach sums_Queue[sizeQueue];
pthread_mutex_t mutexQueue;
pthread_cond_t condQueue;

int64_t input_casos();
void executeTask(Goldbach* task);
void submitTask(Goldbach task);
void* startThread(void* args);
int create_threads(shared_data_t* shared_data);

#endif  // THREADS_H
