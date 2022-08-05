// Copyright 2021 Andres Azofeifa-Garcia <andres.azofeifagarcia@ucr.ac.cr>
// Javier Molina-Herrera <javier.molinaherrera@ucr.ac.cr>
// Gabriel Zúñiga-Orozco <gabriel.zunigaorozco@ucr.ac.cr> CC-BY
// This program solves the Enchanted Forest problem

#include <omp.h>
#include "Forest.hpp"

/**
 * @brief Construct a new Forest::Forest object
 * 
 */
Forest::Forest() {
  threadCount = 0;
  rowCounter = 0;
  columnCounter = 0;
}

/**
 * @brief Destroy the Forest::Forest object
 * 
 */
Forest::~Forest() { }

/**
 * @brief Sets the number of threads of the computer to be used by the program
 * to process the maps
 * 
 * @param threads The number of threads
 */
void Forest::setThreadCount(int threads) {
  threadCount = threads;
}

/**
 * @brief Sets out the atribute acualNight and updates the atributes
 * rowsCounter and columnsCounter 
 * 
 * @param map The new vector<vector<char>> to be seted
 * @param rows The number of rows 
 * @param columns The number of columns
 */
void Forest::setActualNight(std::vector<std::vector<char>> map,
 size_t rows, size_t columns) {
  rowCounter = rows;
  columnCounter = columns;
  actualNight = map;
}

/**
 * @brief Sets out the atribute nextNight
 * 
 * @param map The new vector<vector<char>> to be seted
 */
void Forest::setNextNight(std::vector<std::vector<char>> map) {
  nextNight = map;
}

/**
 * @brief Returns the number of rows that the actualNight matrix have
 * 
 * @return size_t rowCounter : Number of rows of the atribute actualNight
 */
size_t Forest::getRowCounter() {
  return rowCounter;
}

/**
 * @brief Returns the number of columns that the actualNight matrix have
 * 
 * @return size_t columnCounter : Number of columns of the atribute actualNight
 */
size_t Forest::getColumnCounter() {
  return columnCounter;
}

/**
 * @brief Returns the actualNight atribute
 * 
 * @return vector<vector<char>> actualNight : The actualNight matrix 
 */
std::vector<std::vector<char>> Forest::getActualNight() {
  return actualNight;
}

/**
 * @brief Returns the nextNight atribute
 * 
 * @return vector<vector<char>> nextNight : The nextNight matrix 
 */
std::vector<std::vector<char>> Forest::getNextNight() {
  return nextNight;
}

/**
 * @brief Creates a vector<vector<char>> with the dimensions given by parameter
 * then returns it.
 * 
 * @details Creates a matrix with the given dimensions but adding 2 for each
 * coordinate, this is for avoid overflow in the enchantCell() function. 
 * 
 * @param rows The rows coordinate
 * @param columns The columns coordinate
 * @return vector<vector<char>> The new matrix
 */
std::vector<std::vector<char>> Forest::createForestMap(size_t rows,
 size_t columns) {
  std::vector<std::vector<char>> temporalMap = std::vector<std::vector<char>>
  (rows + 2);
  for (size_t index = 0; index < rows + 2; index++) {
    temporalMap[index].resize(columns + 2);
    for (size_t index2 = 0; index2 < columns + 2; index2++) {
      temporalMap[index][index2] = '|';  // Fill the matrix with a default char
    }
  }
  return temporalMap;
}

/**
 * @brief Applies the magical rules for each cell of the matrix.
 * The enchanted result is stored in the nextNight matrix. 
 * 
 */
void Forest::enchantForest() {
  #pragma omp parallel for num_threads(threadCount) collapse(2) default(none) \
  shared(rowCounter, columnCounter , actualNight, nextNight) schedule(static, 8)
  for (size_t rowIndex = 1; rowIndex <= rowCounter; rowIndex++) {
    for (size_t columnIndex = 1; columnIndex <= columnCounter; columnIndex++) {
      enchantCell(rowIndex, columnIndex);  // Enchants each cell
    }
  }
  actualNight = nextNight;
  //actualNight.swap(nextNight);  //  Does not work
}

/**
 * @brief Reads the caracter stored in the given coordinates of the actualNight
 * matrix, evaluates the magical rules for this caracter and stores the
 * enchanted result in the same given position but in the nextNight matrix.
 * 
 * 
 * @param rowPosition Rows coordinate
 * @param columnPosition columns coordinate
 */
void Forest::enchantCell(size_t rowPosition, size_t columnPosition) {
  char thisCell = actualNight[rowPosition][columnPosition];
  int magicalTrees = lookForNeighbours(rowPosition, columnPosition, 'a');
  int enchantedLakes = lookForNeighbours(rowPosition, columnPosition, 'l');
  // Evaluates the magical rules:
  // if the cell is a magical tree
  if (thisCell == 'a') {
    if (enchantedLakes >= 4) {
      // stores the new char in the nexNight matrix
      nextNight[rowPosition][columnPosition] = 'l';
    } 
    if (magicalTrees > 4) {
      // stores the new char in the nexNight matrix
      nextNight[rowPosition][columnPosition] = '-';
    }
  } 
  // if the cell is an enchanted lake
  if (thisCell == 'l' && enchantedLakes < 3) {
    // stores the new char in the nexNight matrix
    nextNight[rowPosition][columnPosition] = '-';
  }
  // if the cell is a grassland
  if (thisCell == '-' && magicalTrees >= 3) {
    // stores the new char in the nexNight matrix
    nextNight[rowPosition][columnPosition] = 'a';
  }
}

/**
 * @brief Looks for the number of neighbours with the given char type 
 * in the given cell position 
 * 
 * @param rowPosition Rows coordinate
 * @param columnPosition columns coordinate
 * @param cellType The type of char. 
 * Tree = 'a', lake = 'l' and grassland = '-'
 * @return int cellTypeCounter : 
 * The number of neighbours of the given char type
 */
int Forest::lookForNeighbours(size_t rowPosition, size_t columnPosition,
 char cellType) {
  int cellTypeCounter = 0;
  // looks for neighbours in the previous row
  if (actualNight[rowPosition - 1][columnPosition - 1] == cellType) {
    cellTypeCounter++;
  }
  if (actualNight[rowPosition - 1][columnPosition] == cellType) {
    cellTypeCounter++;
  }
  if (actualNight[rowPosition - 1][columnPosition + 1] == cellType) {
    cellTypeCounter++;
  }
  // looks for neighbours in the actual row
  if (actualNight[rowPosition][columnPosition - 1] == cellType) {
    cellTypeCounter++;
  }
  if (actualNight[rowPosition][columnPosition + 1] == cellType) {
    cellTypeCounter++;
  }
  // looks for neighbours in the next row
  if (actualNight[rowPosition + 1][columnPosition - 1] == cellType) {
    cellTypeCounter++;
  }
  if (actualNight[rowPosition + 1][columnPosition] == cellType) {
    cellTypeCounter++;
  }
  if (actualNight[rowPosition + 1][columnPosition + 1] == cellType) {
    cellTypeCounter++;
  }
  return cellTypeCounter;
}

/**
 * @brief Returns the actualNight matrix in string format
 * 
 * @return string : ActualNight matrix string  
 */
std::string Forest::actualNightToString() {
  std::stringstream outputStream;
  for (size_t rowIndex = 1; rowIndex <= rowCounter; rowIndex++) {
    for (size_t columnIndex = 1; columnIndex <= columnCounter; columnIndex++) {
      outputStream << actualNight[rowIndex][columnIndex];
    }
    outputStream << "\n";
  }
  return outputStream.str();
}

/**
 * @brief Returns the nextNight matrix in string format
 * 
 * @return string : nextNight matrix string  
 */
std::string Forest::nextNightToString() {
  std::stringstream outputStream;
  for (size_t rowIndex = 1; rowIndex <= rowCounter; rowIndex++) {
    for (size_t columnIndex = 1; columnIndex <= columnCounter; columnIndex++) {
      outputStream << nextNight[rowIndex][columnIndex];
    }
    outputStream << "\n";
  }
  return outputStream.str();
}
