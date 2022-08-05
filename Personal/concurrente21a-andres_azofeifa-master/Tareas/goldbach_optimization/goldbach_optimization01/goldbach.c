// Copyright 2021 Andres Azofeifa-Garcia <andres.azofeifagarcia@ucr.ac.cr>CC-BY
// Este programa representa La Conjetura de Goldbach. En caso de que el numero
// sea par este dara dos sumas, si el valor es impar, este dara tres sumas

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "goldbach.h"
#include "matrix.h"

/**
 *  @brief define si el numero ingresado es primo o no (No optimizado)
 *  @param cantidad_Primos caso que desea ser estudiado
 *  @return int 
 */
int esPrimo(int64_t valor) {
  int esPrimo = 1;
  if (valor != 2 && valor % 2 == 0) {
  esPrimo = 0;
  } else {
    for (int index = 3; index <= sqrt(valor); index = index + 2) {
      if (valor % index == 0) {
        esPrimo = 0;
        break;
      }
    }
  }
  return esPrimo;
}

/**
 *  @brief funcion para determinar si se ejecuta la conjetura fuerte o debil
 *  dependiendo del valor ingresado
 *  @param caso valor por analizar en la funcion
 *  @param thread_id numero privado del hilo
 *  @return void
 */
void execute_Goldbach(int64_t caso, int64_t thread_id) {
  if (caso%2 == 0) {  /// caso par con la conjetura fuerte
    fuerte(caso, thread_id);
  } else if (caso%2 != 0) {  /// caso impar con la conjetura debil
    debil(caso, thread_id);
  }
}

/**
 *  @brief funcion que ejecuta la conjetura fuerte de Goldbach
 *  @param valor caso que sera analizado
 *  @param thread_id numero privado del hilo utilizado para guardar los datos
 *  en la matriz cond safe
 *  @return void
 */
void fuerte(int64_t valor, int64_t thread_id) {
  int64_t auxNumber = valor;
  if (valor < 0) {
    auxNumber = labs(auxNumber);
  }

  data.goldbach_safe[thread_id][0] = valor;
  int64_t sumas = 0;
  int64_t index1 = 2;  /// Primer valor primo con el que se inicia las sumas
  int64_t index2 = auxNumber - index1;
  /// Primer valor primo con el que se inicia las sumas

  while (index1 <= index2) {
    if (esPrimo(index1) == 1 && esPrimo(index2) == 1) {
      if (sumas == 0) {
        data.goldbach_safe[thread_id][2] = index1;
        data.goldbach_safe[thread_id][3] = index2;

      } else {
        int64_t temp = ((sumas) +2);
        data.goldbach_safe[thread_id][temp] = index1;
        data.goldbach_safe[thread_id][temp + 1] = index2;
        /// Los valores se colocan en sus casillas
        /// respectivas de la matriz
      }
      sumas += 2;
      /// Cada iteracion aumenta la cantidad de sumas en 2
    }
    index1++;
    index2--;
  }
  data.goldbach_safe[thread_id][1] = sumas;
}

/**
 *  @brief funcion que ejecuta la conjetura debil de Goldbach
 *  @param valor caso que sera analizado
 *  @param thread_id numero privado del hilo utilizado para guardar los datos 
 *  en la matriz cond safe
 *  @return void
 */
void debil(int64_t valor, int64_t thread_id) {
  /// Valor auxiliar para calculo de valores negativos y preservas signo
  int64_t auxNumber = valor;
  if (valor < 0) {
    auxNumber = labs(auxNumber);
  }

  /// El valor ingresado es guardado en la primera casilla de la fila
  data.goldbach_safe[thread_id][0] = valor;
  int64_t index1 = 2;  /// Primer valor primo con el que se inicia las sumas
  int64_t sumas = 0;

  while (index1 <= auxNumber / 2) {
    if (esPrimo(index1) == 1) {
      int64_t index2 = index1;  /// Segundo valor posible
      int64_t index3 = auxNumber - (index1 + index2);  /// Tercer Valor posible
      while (index2 <= index3) {
        if (esPrimo(index2) == 1 && esPrimo(index3) == 1) {
          if (sumas == 0) {
            data.goldbach_safe[thread_id][2] = index1;
            data.goldbach_safe[thread_id][3] = index2;
            data.goldbach_safe[thread_id][4] = index3;
          } else {
            int64_t temp = ((sumas)+3);
            data.goldbach_safe[thread_id][temp-1] = index1;
            data.goldbach_safe[thread_id][temp] = index2;
            data.goldbach_safe[thread_id][temp+1] = index3;
              /// Los valores se colocan en sus casillas
              /// respectivas de la matriz
            }
            sumas += 3;
            /// Cada iteracion aumenta la cantidad de sumas en 3
          }
        index2++;
        index3--;
      }
    }
    index1++;
  }
  data.goldbach_safe[thread_id][1] = sumas;
  /// se guarda en la segunda casilla de
  /// la fila la totalidad de sumas
}

/**
 *  @brief funcion para imprimir la matriz cond safe con los 
 *  resultados ordenados
 *  @return void
 */
void StrongSafeMatrix() {
  for (int index = 0; index < data.size; index++) {  /// Manejo indice filas
     /// Valor Par
    if (data.goldbach_safe[index][1] == 0) {
        printf("%d: NA", data.goldbach_safe[index][0]);  /// Sumas 0 NA
        /// Numero negativo imprime sumas si tiene sumas
    } else if (data.goldbach_safe[index][0] < 0) {
        printf("%d: %d sums: ", data.goldbach_safe[index][0],
        data.goldbach_safe[index][1] / 2);
        for (int colummas2 = 0; colummas2 <
          data.goldbach_safe[index][1]; colummas2 += 2) {
          if (colummas2 + 2 < data.goldbach_safe[index][1]) {
            printf("%d + %d, ", data.goldbach_safe[index]
            [colummas2 + 2], data.goldbach_safe[index]
            [colummas2 + 3]);
          } else {
            printf("%d + %d", data.goldbach_safe[index]
            [colummas2 + 2], data.goldbach_safe[index]
            [colummas2 + 3]);
          }
        }
        /// Positivo con sumas imprime cantidad sums
    } else if (data.goldbach_safe[index][0] > 0) {
        printf("%d: %d sums ", data.goldbach_safe[index][0],
        data.goldbach_safe[index][1] / 2);
      }
      printf("\n");
  }
}

void WeakSafeMatrix() {
    for (int index = 0; index < data.size; index++) {
        if (data.goldbach_safe[index][1] == 0) {
        printf("%d: NA", data.goldbach_safe[index][0]);  /// Sumas 0 NA
      } else if (data.goldbach_safe[index][0] < 0) {
        /// Numero negativo imprime sumas si tiene sumas
        printf("%d: %d sums: ", data.goldbach_safe[index][0], 
        data.goldbach_safe[index][1] / 3);
        for (int colummas2 = 0; colummas2 < data.goldbach_safe[index]
          [1]; colummas2 += 3) {
          printf("%d + %d + %d", data.goldbach_safe[index]
          [colummas2 + 2], data.goldbach_safe[index][colummas2 + 3],
          data.goldbach_safe[index][colummas2 + 4]);
          if (colummas2 + 3 < data.goldbach_safe[index][1]) {
            printf(", ");
          }
        }
        /// Positivo con sumas imprime cantidad sums
      } else if (data.goldbach_safe[index][0] > 0) {
        printf("%d: %d sums ", data.goldbach_safe[index][0],
        data.goldbach_safe[index][1] / 3);
      }
      printf("\n");
    }
}