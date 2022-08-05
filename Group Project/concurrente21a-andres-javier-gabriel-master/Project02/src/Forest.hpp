// Copyright 2021 Andres Azofeifa-Garcia <andres.azofeifagarcia@ucr.ac.cr>
// Javier Molina-Herrera <javier.molinaherrera@ucr.ac.cr>
// Gabriel Zúñiga-Orozco <gabriel.zunigaorozco@ucr.ac.cr> CC-BY
// This program solves the Enchanted Forest problem

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#ifndef FOREST_HPP

class Forest {
 private:
  /// The number of threads to be used when processing
  int threadCount;
  /// The matrix that represents the actual magical forest
  std::vector<std::vector<char>> actualNight;
  /// The matrix that represents the updated magical forest
  std::vector<std::vector<char>> nextNight;
  /// Number of rows of the magical forest
  size_t rowCounter;
  /// Number of columns of the magical forest
  size_t columnCounter;

 public:
  Forest();
  ~Forest();
  ///  Sets the number of threads to be used by the program
  void setThreadCount(int threadCount);
  ///  Sets out the actualNighth matrix
  void setActualNight(std::vector<std::vector<char>> map, size_t rows,
   size_t columns);
  ///  sets out the nextNight matrix
  void setNextNight(std::vector<std::vector<char>> map);
  /// returns the actual state of the forest
  std::vector<std::vector<char>> getActualNight();
  /// returns the next state of the forest
  std::vector<std::vector<char>> getNextNight();
  /// returns the number of rows
  size_t getRowCounter();
  /// returns the number of columns
  size_t getColumnCounter();
  ///  Creates a vector<vector<char>> with the given dimensions
  std::vector<std::vector<char>> createForestMap(size_t row, size_t column);
  ///  applies the magical rules to all the actualNight matrix
  void enchantForest();
  ///  applies the magical rules to the given position
  void enchantCell(size_t rowPosition, size_t columnPosition);
  ///  search for neighbours of the char type in the given position
  int lookForNeighbours(size_t rowPosition, size_t columnPosition,
  char cellType);
  /// returns the actualNight matrix in string format
  std::string actualNightToString();
  /// returns the nextNight matrix in string format
  std::string nextNightToString();
};

#endif  // FOREST_HPP
