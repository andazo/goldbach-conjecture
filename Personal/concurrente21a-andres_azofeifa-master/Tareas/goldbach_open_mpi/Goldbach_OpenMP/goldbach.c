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
#include "common.h"
#include "matrix.h"

/**
 *  @brief funcion para determinar si se ejecuta la conjetura fuerte o debil
 *  dependiendo del valor ingresado
 *  @param caso valor por analizar en la funcion
 *  @param thread_id numero privado del hilo
 *  @return void
 */
void execute_Goldbach(int64_t caso, int64_t id) {
  if (caso%2 == 0) {  /// caso par con la conjetura fuerte
    fuerte(caso, id);
  } else if (caso%2 != 0) {  /// caso impar con la conjetura debil
    debil(caso, id);
  }
}

/**
 *  @brief funcion que ejecuta la conjetura fuerte de Goldbach
 *  @param valor caso que sera analizado
 *  @param thread_id numero privado del hilo utilizado para guardar los datos
 *  en la matriz cond safe
 *  @return void
 */
void fuerte(int64_t valor, int64_t id) {
  int64_t sumas = 0;

  #pragma omp parallel num_threads(data.thread_count) \
    default(none) shared(data, sumas, valor, id)
  {
    data.goldbach_safe[id][0] = valor;

    if (valor < 0) {
      valor = labs(valor);  ///  valor absoluto del caso si este es nagtivo
    }

    #pragma omp for schedule(dynamic, 2)
    for (int64_t index = 2; index <= (valor / 2); index++) {
      int64_t index2 = valor - index;
      if (esPrimo(index) && esPrimo(index2) && index <= index2) {
        if (sumas == 0) {
          data.goldbach_safe[id][2] = index;
          data.goldbach_safe[id][3] = index2;
        } else {
          int64_t temp = ((sumas)+2);
          data.goldbach_safe[id][temp] = index;
          data.goldbach_safe[id][temp + 1] = index2;
        }
        #pragma omp critical(can_access_sum)
        sumas += 2;
      }
    }
    data.goldbach_safe[id][1] = sumas;
  }
  printf("Fuerte\n");
}

/**
 *  @brief funcion que ejecuta la conjetura debil de Goldbach
 *  @param valor caso que sera analizado
 *  @param thread_id numero privado del hilo utilizado para guardar los datos 
 *  en la matriz cond safe
 *  @return void
 */
void debil(int64_t valor, int64_t id) {
  int64_t sumas = 0;

  #pragma omp parallel num_threads(data.thread_count) \
  default(none) shared(data, sumas, valor, id)
  {
    data.goldbach_safe[id][0] = valor;

    if (valor < 0) {
      valor = labs(valor);  /// valor absoluto del caso si este es nagtivo
    }

    #pragma omp for schedule(dynamic, 2)
    for (int64_t index = 2; index <= valor / 3; index++) {
      if (esPrimo(index)) {
        for (int64_t index2 = index; index2 < valor / 2; index2+=2) {
          if (esPrimo(index2)) {
            int64_t auxiliar_index = valor - (index + index2);
            if (index + index2 + auxiliar_index == valor
              && esPrimo(auxiliar_index)) {
              if (auxiliar_index > 2 && auxiliar_index >= index2) {
                if (sumas == 0) {
                  data.goldbach_safe[id][2] = index;
                  data.goldbach_safe[id][3] = index2;
                  data.goldbach_safe[id][4] = auxiliar_index;
                } else {
                  int64_t temp = ((sumas)+3);
                  data.goldbach_safe[id][temp-1] = index;
                  data.goldbach_safe[id][temp] = index2;
                  data.goldbach_safe[id][temp+1] = auxiliar_index;
                }
                #pragma omp critical(can_access_sum)
                sumas += 3;
              }
            }
          }
        }
      }
    }
    data.goldbach_safe[id][1] = sumas;
  }
  printf("Debil\n");
}

/**
 *  @brief funcion para imprimir la matriz cond safe con los 
 *  resultados ordenados
 *  @return void
 */
void printSafeMatrix() {
  for (int index = 0; index < data.size; index++) {  /// Manejo indice filas
    if (data.goldbach_safe[index][0] % 2 == 0) {  /// Valor Par
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
    } else {  /// Valores impares
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
}
