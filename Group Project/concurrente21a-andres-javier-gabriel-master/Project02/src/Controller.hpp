// Copyright 2021 Andres Azofeifa-Garcia <andres.azofeifagarcia@ucr.ac.cr>
// Javier Molina-Herrera <javier.molinaherrera@ucr.ac.cr>
// Gabriel Zúñiga-Orozco <gabriel.zunigaorozco@ucr.ac.cr> CC-BY
// This program solves the Enchanted Forest problem

#include <unistd.h>
#include <string>
#include "FileManager.hpp"

#ifndef CONTROLLER_HPP

class Controller {
 private:
  /// FileManager object
  FileManager fileManager;
  /// Forest object
  Forest enchantedForest;
  /// Arguments
  char** argv;
  /// Argument number
  int argc;
 public:
  Controller();
  ~Controller();
  /// Returns the enchantedForest attribute
  Forest getEnchantedForest();
  /// Gets the number of threads to be used
  size_t getThreads(int argc, char* argv[]);
  /// Reads the job file and sends the maps to get read and then enchanted
  int enchantmentManager();
  /// Change the matrix with the given rules
  void enchanter(std::string mapFileName, int nights);
  /// Stores the arguments as atributes to be used later
  void setArguments(int argc, char* argv[]);
};

#endif  // Controller
