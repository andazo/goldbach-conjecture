// Copyright 2021 Andres Azofeifa-Garcia <andres.azofeifagarcia@ucr.ac.cr> CC-BY
// Este programa representa La Conjetura de Goldbach. En caso de que el numero
//sea par este dara dos sumas, si el valor es impar, este dara tres sumas

#include <assert.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

int64_t* input_casos(int64_t *tamano);
void execute_Goldbach(int64_t caso);
int64_t esPrimo(int64_t valor);
int64_t* fuerte(int64_t number,int64_t *NumSums);
int64_t* debil(int64_t number,int64_t *NumSums);
void printFuerte(int64_t valor); 
void printDebil(int64_t valor);

int main() {

    int64_t *p; 
    int64_t size;
    p = input_casos(&size);
    int64_t* casosPrueba = calloc(size,sizeof(int64_t));

    for (int i = 0; i < size; i++){ 
        casosPrueba[i] = *(p + i); 
    }

    for (int i = 0; i < size; i++) { ///recorrido casos con ambos metodos
        execute_Goldbach(casosPrueba[i]);
    }

    free(casosPrueba);
    return 0;
}

/**
 *  @brief funcion para ejecutar las conjeturas
 *  @param primos valores primos que se necesitan para ejecutar los casos
 *  @param caso valores por analizar con la cojetura
 *  @param cantidad_Primos cantidad de primos necesarios para la conjetura
 *  @return void
 */
void execute_Goldbach(int64_t caso) {
    if (caso%2 == 0) { ///caso par con la conjetura fuerte
        printFuerte(caso);
    } else if (caso%2 != 0) { ///caso impar con la conjetura debil
        printDebil(caso);
    }
}

/**
 *  @brief funcion para el ingreso de datos
 *  @param tamano referencia para el size del arreglo dinamico
 *  @return int*
 */
int64_t* input_casos(int64_t *tamano){

    int64_t size = 0;
    int64_t* lista_Casos = malloc(1 * sizeof(*lista_Casos));
    assert(lista_Casos);
    int64_t caso_Goldbach;
    
    printf("Ingrese los casos que desea calcular: \n");
    while(scanf("%ld",&caso_Goldbach)!=EOF){

        if(caso_Goldbach == 0){
            break;
        }

        size++;
        lista_Casos = realloc(lista_Casos, size*sizeof(int64_t));
        assert(lista_Casos);
        lista_Casos[size-1] = caso_Goldbach;
    } 
    *tamano = size;
    return lista_Casos;
}

/**
 *  @brief Analiza si numero es primo
 *  @param valor valor que sera analizado
 *  @return int
 */
int64_t esPrimo(int64_t valor){

    int64_t esPrimo = 1;
    if (valor != 2 && valor % 2 == 0) {
    esPrimo = 0;
  }else {
        for (int64_t index = 3; index <= sqrt(valor); index = index + 2) {
            if (valor % index == 0) {
                esPrimo = 0;
                break;
            }
        }
    }
    return esPrimo;
}

/**
 *  @brief Cinjetura fuerte de Goldbach
 *  @param valor valor que sera analizado en la conjetura
 *  @return void
 */
int64_t* fuerte(int64_t valor,int64_t *NumSums){

    int64_t* arrayFuerte = calloc(1,sizeof(int64_t));
    assert(arrayFuerte);
    int64_t sumas = 0;

    if (valor < 0) {
        valor = labs(valor); /// valor absoluto del caso si este es nagtivo
    }

    for (int64_t index = 2; index <= (valor / 2); index++) {
        int64_t index2 = valor - index;
        if (esPrimo(index) && esPrimo(index2) && index <= index2) {
            if(sumas == 0){
                arrayFuerte = malloc(2 * sizeof(*arrayFuerte));
                arrayFuerte[0] = index;
                printf("Valor: %ld \n", arrayFuerte[0]);
                arrayFuerte[1] = index2;
                printf("Valor: %ld \n", arrayFuerte[1]);
            }else{
                int64_t temp = ((sumas)+2);
                arrayFuerte = realloc(arrayFuerte, temp * sizeof(int64_t));
                arrayFuerte[temp-2] = index;
                arrayFuerte[temp-1] = index2;
            }
            sumas += 2;
        }
    }
    
    *NumSums = sumas;
    return arrayFuerte;
}

/**
 *  @brief Conjetura debil de Goldbach
 *  @param valor valor que sera analizado en la conjetura
 *  @return void
 */
int64_t* debil(int64_t valor,int64_t *NumSums){

    int64_t* sumas_debil = calloc(1,sizeof(int64_t));
    assert(sumas_debil);
    int64_t sumas = 0;

    if (valor < 0) {
        valor = labs(valor); /// valor absoluto del caso si este es nagtivo
    }

    for (int64_t index = 2; index <= valor / 3; index++) {
        if (esPrimo(index)) {
            for (int64_t index2 = index; index2 < valor / 2; index2+=2) {
                if (esPrimo(index2)) {
                int64_t auxiliar_index = valor - (index + index2);
                    if (index + index2 + auxiliar_index == valor
                        && esPrimo(auxiliar_index)) {
                        if (auxiliar_index > 2 && auxiliar_index >= index2) {
                            if (sumas == 0) {
                                sumas_debil = malloc(3 * sizeof(*sumas_debil));
                                sumas_debil[0] = index;
                                sumas_debil[1] = index2;
                                sumas_debil[2] = auxiliar_index;
                            } else {
                                int64_t temp = ((sumas)+3);
                                sumas_debil = realloc(sumas_debil,
                                temp * sizeof(int64_t));
                                sumas_debil[temp-3] = index;
                                sumas_debil[temp-2] = index2;
                                sumas_debil[temp-1] = auxiliar_index;
                            }
                            sumas += 3;
                        }
                    }
                }
            }
        }
    }

    *NumSums = sumas;
    return sumas_debil;
}

void printFuerte(int64_t valor){

    int64_t *p; 
    int64_t sumas;
    p = fuerte(valor,&sumas);
    int64_t* casosPrueba = calloc(sumas,sizeof(int64_t));

    for (int i = 0; i < sumas; i++){ 
        casosPrueba[i] = *(p + i); 
    }

    int64_t caso = valor;
    bool caso_Negativo = false;
    if (valor < 0) {
        caso_Negativo = true;
        valor = labs(valor); /// valor absoluto del caso si este es nagtivo
    }

   if (sumas == 0) {
        printf("%ld: NA",caso);
        /// Caso en que no existan sumas, contador se mantiene nulo
    } else if(caso_Negativo == true) {
            printf("%ld: %ld sums: ",caso, sumas/2);
            for (int i = 0; i < (sumas); i += 2) {
                if (i+2 < sumas) {
                    printf(" %ld + %ld,", casosPrueba[i], casosPrueba[i+1]);
                    /// si hay mas de 2 se imprime la siguiente
                }else{
                    printf(" %ld + %ld", casosPrueba[i], casosPrueba[i+1]);
                }
            }
    }else if(caso_Negativo == false){
            printf("%ld: %ld sums ",caso, sumas/2);
    }

    printf("\n");
    free(casosPrueba);
}

void printDebil(int64_t valor){

    int64_t *p; 
    int64_t sumas;
    p = debil(valor,&sumas);
    int64_t* casosPrueba = calloc(sumas,sizeof(int64_t));

    for (int i = 0; i < sumas; i++){ 
        casosPrueba[i] = *(p + i); 
    }

    int64_t caso = valor;
    bool caso_Negativo = false;
    if (valor < 0) {
        caso_Negativo = true;
        valor = labs(valor); /// valor absoluto del caso si este es nagtivo
    }

    if (sumas == 0) {
        printf("%ld: NA",caso);
        /// Caso en que no existan sumas, contador se mantiene nulo
    } else if(caso_Negativo == true) {
        printf("%ld: %ld sums: ",caso, sumas/3);
        for (int i = 0; i < (sumas); i += 3) {
            // en caso de que el valor sea negativo, se muestran las sumas
            printf(" %ld + %ld + %ld", casosPrueba[i], casosPrueba[i+1],
            casosPrueba[i+2]);
            if (i+3 < sumas) {
                /// si hay mas de 3 se imprime la siguiente
                printf(",");
            }
        }
      
    }else if(caso_Negativo == false){
        printf("%ld: %ld sums ",caso, sumas/3);
    }

    printf("\n");
    free(casosPrueba);
}