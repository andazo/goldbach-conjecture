// Copyright 2021 Andres Azofeifa-Garcia <andres.azofeifagarcia@ucr.ac.cr>
// Javier Molina-Herrera <javier.molinaherrera@ucr.ac.cr>
// Gabriel Zúñiga-Orozco <gabriel.zunigaorozco@ucr.ac.cr> CC-BY
// This program solves the Enchanted Forest problem

#include "Controller.hpp"

int analizeArguments(int argc);

int main(int argc, char* argv[]) {
  int exitState = EXIT_SUCCESS;
  Controller programManager;
  std::string inputFileName = "";
  std::string inputFileDir = "";
  int threadCount = 0;
  exitState = analizeArguments(argc);
  if (exitState == EXIT_SUCCESS) {
    programManager.setArguments(argc, argv);
    threadCount = programManager.getThreads(argc, argv);
    if (threadCount != 0) {
      programManager.getEnchantedForest().setThreadCount(threadCount);
      exitState = programManager.enchantmentManager();
    } else {
      exitState = EXIT_FAILURE;
    }
  }
  return exitState;
}

int analizeArguments(int argc) {
  int exitState = EXIT_FAILURE;
  if (argc >= 2 && argc <= 4) {
    exitState = EXIT_SUCCESS;
  }
  if (argc == 1) {
    std::cout << "Error: must introduce at least one argument\n";
  }
  if (argc > 4) {
    std::cout << "Error: too many arguments\n";
  }
  return exitState;
}
