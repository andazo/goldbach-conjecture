/// Copyright 2021 Javier Molina, Gabriel Zúñiga and Andrés Azofeifa.
/// Universidad de Costa Rica

#include <stdlib.h>
#include <math.h>
#include <iostream>

#include "GoldbachCalculator.hpp"

///  Constructor
GoldbachCalculator::GoldbachCalculator(size_t thread_id) {
  golbachThread_id = thread_id;
}
///  Destructor
GoldbachCalculator::~GoldbachCalculator() {
}

/**
 * @brief Checks if a given number is prime.
 * @details
 * @param number The number to check if it's prime.
 * @return checkPrime: 1 for yes, 0 for no.
 */

// Check if the number is prime
/// based on : https://stackoverflow.com/a/2055349
int GoldbachCalculator::checkPrime(int64_t number) {
  int64_t max_iteration = sqrt(number) + 1;
  if (number == 2 || number == 3 || number == 5 || number == 7) {
    return 1;
  }
  if (number % 2 == 0 || number % 3 == 0 || number % 5 == 0 ||
  number % 7 == 0) {
    return 0;
  }
  for (int64_t index = 7; index < max_iteration; index+=2) {
    if (number % index == 0) {
      return 0;
    }
  }
  return 1;
}

/**
 * @brief Calculates all the goldbach sums of the number stored in the 
 * goldbachSums structure. 
 * The goldbachSums structure also stores the results.
 * 
 * @details for a positive number only update the sumsCounter atribute of
 * the struct goldbachSums.
 * If the number is negative do the same update to the sumsCounter atribute,
 * and then enqueue the three numbers that represents the summands of each 
 * goldbach sum into the sums vector that is stored in the goldbachSums 
 * struct.
 * 
 * Example if the number is -7 at the end of the execution of this method 
 * the sums vector will have: 2 -> 2 -> 3
 * The sums of goldbach of 7 are 2 + 2 + 3
 * @param goldbachData A structure containing all the relevant data for the
 * operation.
 * @return goldbachSums goldbachData
 */
goldbachSums GoldbachCalculator::weakConjecture(goldbachSums goldbachData) {
  int64_t number = goldbachData.numberGoldbach;
  for (int64_t index = 2; index <= number / 3; index++) {
    if (checkPrime(index)) {
      for (int64_t index2 = index; index2 < number / 2; index2+=2) {
        if (checkPrime(index2)) {
          int64_t auxiliar_index = number - (index + index2);
          if (index + index2 + auxiliar_index == number
             && checkPrime(auxiliar_index)) {
            if (auxiliar_index > 2 && auxiliar_index >= index2) {
              goldbachData.sumsCounter++;
              if (goldbachData.sign == 1) {
                sum temporal;
                temporal.num1 = index;
                temporal.num2 = index2;
                temporal.num3 = auxiliar_index;
                goldbachData.sums.push_back(temporal);
              }
            }
          }
        }
      }
    }
  }
  return goldbachData;
}

/**
 * @brief Calculates all the goldbach sums of the number stored in the 
 * goldbachSums structure. The number has to be odd.
 * The goldbachSums structure also stores the results.
 * 
 * @details for a positive number only update the sumsCounter atribute of
 * the struct goldbachSums.
 * If the number is negative do the same update to the sumsCounter atribute,
 * and then enqueue the two numbers that represents the summands of each 
 * goldbach sum into the sums vector that is stored in the goldbachSums 
 * struct.
 * 
 * Example if the number is -10 at the end of the execution of this method 
 * the sums vector will have: 3 -> 7 and 5 -> 5
 * the sums of goldbach of 10 are: 3 + 7, 5 + 5
 * @param goldbachData A structure containing all the relevant data for the
 * operation.
 * @return goldbachSums goldbachData
*/
goldbachSums GoldbachCalculator::strongConjecture
(goldbachSums goldbachData) {
  int64_t number = goldbachData.numberGoldbach;
  for (int64_t index = 2; index <= (number / 2); index++) {
    int64_t index2 = number - index;
    if (checkPrime(index) && checkPrime(index2) && index <= index2) {
      goldbachData.sumsCounter++;
      if (goldbachData.sign == 1) {
        sum temporal;
        temporal.num1 = index;
        temporal.num2 = index2;
        temporal.num3 = 0;
        goldbachData.sums.push_back(temporal);
      }
    }
  }
  return goldbachData;
}

/**
 * @brief Consume from the goldbachSums work queue until the stop condition is
 * popped.
 * For each data consumed, the consume method will be called
 * 
 * @return 0 : Ends when the program closes.
 */
int GoldbachCalculator::run() {
  this->consumeForever();
  return 0;
}

/**
 * @brief Gets data from consumeForever to calculate the 
 * Goldbach sums of the given number.
 * 
 * @param data 
 */
void GoldbachCalculator::consume(const goldbachSums& data) {
  int64_t number = data.numberGoldbach;  // Number to calculate
  goldbachSums result;  // Result vector
  if (data.isValid == true) {  // Number is bigger than 5 or smaller than -5
    if (number % 2 == 0) {  // Even
      result = strongConjecture(data);
      data.responseArray[data.originVectorPosition] = result;
      mutex.lock();  // Critical section
      // Increases the respective request's done numbers counter
      *data.doneNumbers = *data.doneNumbers + 1;
      // If all the numbers in the request are done
      if (*data.doneNumbers == data.numbersQuantity) {
        data.canContinue->signal();  // Allow to send answer to the user
      }
      mutex.unlock();
    } else {  // Odd
      result = weakConjecture(data);
      data.responseArray[data.originVectorPosition] =  result;
      mutex.lock();  // Critical section
      // Increases the respective request's done numbers counter
      *data.doneNumbers = *data.doneNumbers + 1;
      // If all the numbers in the request are done
      if (*data.doneNumbers == data.numbersQuantity) {
        data.canContinue->signal();  // Allow to send answer to the user
      }
      mutex.unlock();
    }
  } else {
      data.responseArray[data.originVectorPosition] = data;
      mutex.lock();  // Critical section
      // Increases the respective request's done numbers counter
      *data.doneNumbers = *data.doneNumbers + 1;
      // If all the numbers in the request are done
      if (*data.doneNumbers == data.numbersQuantity) {
        data.canContinue->signal();  // Allow to send answer to the user
      }
      mutex.unlock();
  }
}
