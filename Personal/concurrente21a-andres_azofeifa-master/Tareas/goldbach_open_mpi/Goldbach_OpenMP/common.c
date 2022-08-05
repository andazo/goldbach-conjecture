// Copyright 2021 Andres Azofeifa-Garcia <andres.azofeifagarcia@ucr.ac.cr>CC-BY
// Este programa representa La Conjetura de Goldbach. En caso de que el numero
// sea par este dara dos sumas, si el valor es impar, este dara tres sumas

#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <math.h>


#include "common.h"
#include "goldbach.h"

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
 *  @brief define si el numero ingresado es primo o no (No optimizado)
 *  @param cantidad_Primos caso que desea ser estudiado
 *  @return int 
 */
int esPrimo(int64_t valor) {
  int64_t max_iteration = sqrt(valor)+1;
  if (valor == 2 || valor == 3 || valor == 5) {
    return 1;
  }
  if (valor % 2 == 0 || valor % 3 == 0 || valor % 5 == 0) {
    return 0;
  }
  for (int64_t index = 5; index < max_iteration; index+=2) {
    if (valor % index == 0) {
      return 0;
    }
  }
  return 1;
}
