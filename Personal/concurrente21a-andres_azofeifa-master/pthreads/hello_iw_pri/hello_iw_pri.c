#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int create_threads(size_t thread_count);
void* run(void* data);

typedef struct  {
  size_t thread_number;
  size_t thread_count;
} private_data_t;

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
  private_data_t* private_data = (private_data_t*)
    calloc(thread_count, sizeof(private_data_t));
  if (threads && private_data) {
    for (size_t index = 0; index < thread_count; ++index) {
      private_data[index].thread_number = index;
      private_data[index].thread_count = thread_count;
      if (pthread_create(&threads[index], /*attr*/ NULL, run
        , &private_data[index]) == EXIT_SUCCESS) {
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
    free(private_data);
  } else {
    fprintf(stderr, "Could not allocate memory for %zu threads\n"
      , thread_count);
    error = 22;
  }
  
  return error;
}

void* run(void* data) {
  const private_data_t* private_data = (private_data_t*)data;
  printf("Hello from secondary thread %zu of %zu\n", private_data->thread_number
    , private_data->thread_count);

  return NULL;
}
