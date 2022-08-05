// Copyright 2021 Andres Azofeifa-Garcia <andres.azofeifagarcia@ucr.ac.cr>CC-BY
// Este programa representa La Conjetura de Goldbach. En caso de que el numero
// sea par este dara dos sumas, si el valor es impar, este dara tres sumas

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>

#include "matrix.h"

/**
 *  @brief funcion para la creacion de una matriz
 *  @param row_count cantidad de filas
 *  @param col_count cantidad de columnas
 *  @param element_size size de la matriz
 *  @return matrix
 */
void** create_matrix(int64_t row_count, int64_t col_count,
  int64_t element_size) {
  void** matrix = (void**) calloc(row_count, sizeof(void*));
  if ( matrix == NULL ) {
    return NULL;
  }

  for (int64_t row = 0; row < row_count; ++row) {
    if ( (matrix[row] = calloc(col_count, element_size) ) == NULL ) {
      free_matrix(row_count, matrix);
      return NULL;
    }
  }

  return matrix;
}

/**
 *  @brief libera los elementos de la matriz despues de impresion
 *  @param row_count cantidad de filas
 *  @param matrix doble puntero a las filas y columnas de la matriz
 *  @return void
 */
void free_matrix(const int64_t row_count, void** matrix) {
  if (matrix) {
    for (int64_t row = 0; row < row_count; ++row) {
      free(matrix[row]);
    }
  }

  free(matrix);
}
