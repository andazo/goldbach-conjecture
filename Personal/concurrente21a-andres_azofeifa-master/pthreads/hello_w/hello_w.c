// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4
// Creates an arbitrary amount of threads that greet in stdout

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int create_threads(size_t thread_count);
void* run(void* data);

int main(int argc, char* argv[]) {
  size_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc == 2) {
    if (sscanf(argv[1], "%zu", &thread_count) != 1 || errno) {
      fprintf(stderr, "error: invalid thread count\n");
      return EXIT_FAILURE;
    } 
  }

  int error = create_threads(thread_count);
  return error;
}

int create_threads(size_t thread_count) {
  int error = EXIT_SUCCESS;
  pthread_t* threads = (pthread_t*) calloc(thread_count, sizeof(pthread_t));
  if (threads) {
    for (size_t index = 0; index < thread_count; ++index) {
      if (pthread_create(&threads[index], /*attr*/ NULL, run, (void*)index)
          == EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "Could not create secondary thread %zu\n", index);
        error = 21;
        break;
      }
    }

    printf("Hello from main thread\n");

    for (size_t index = 0; index < thread_count; ++index) {
      pthread_join(threads[index], /*value_ptr*/ NULL);
    }
    free(threads);
  } else {
    fprintf(stderr, "Could not allocate memory for %zu threads\n"
      , thread_count);
    error = 22;
  }
  
  return error;
}

void* run(void* data) {
  const size_t thread_number = (size_t)data;
  printf("Hello from secondary thread %zu\n", thread_number);

  return NULL;
}
