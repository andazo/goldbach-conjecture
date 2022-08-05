// Copyright 2021 Andres Azofeifa-Garcia <andres.azofeifagarcia@ucr.ac.cr>
// Javier Molina-Herrera <javier.molinaherrera@ucr.ac.cr>
// Gabriel Zúñiga-Orozco <gabriel.zunigaorozco@ucr.ac.cr> CC-BY
// This program solves the Enchanted Forest problem

#include <mpi.h>
#include "Controller.hpp"

Controller::Controller() {}

Controller::~Controller() {
}

/**
 * @brief Returns the enchantedForest attribute.
 * @return enchantedForest.
 */
Forest Controller::getEnchantedForest() {
  return enchantedForest;
}

/**
 * @brief Saves the arguments as atributes to be used later. Needed for MPI
 *
 */
void Controller::setArguments(int myArgc, char* myArgv[]) {
  argc = myArgc;
  argv = myArgv;
}

/**
 * @brief Gets the number of threads to be used from the arguments.
 * @param argc The number of parameters.
 * @param argv The parameters.
 * @return threadCount : The number of threads to be used.
 */
size_t Controller::getThreads(int argc, char* argv[]) {
  int exitState = EXIT_SUCCESS;
  int threadCount = sysconf(_SC_NPROCESSORS_ONLN);  // Number of cores
  if (argc == 4) {  // If there's an argument specifying the number of threads
    if (sscanf(argv[3], "%d", &threadCount) != 1) {
      exitState = EXIT_FAILURE;
    }
  }
  if (exitState == EXIT_FAILURE || threadCount <= 0) {
    printf("Error: invalid thread number\n");
  }
  return threadCount;
}

/**
 * @brief Reads the job file and sends the maps to get read and then enchanted.
 * @return exitState : Tells if there was an error.
 */

int Controller::enchantmentManager() {
  int nights = 0;  // Number of nights the enchantment will change the map
  fileManager.setJobFileName(fileManager.getFileName(argv));
  fileManager.setJobFileDir(fileManager.getFilePath(argc, argv));
  fileManager.createOutputDir();
  fileManager.storeMapFilesData();
  if (MPI_Init(&argc, &argv)== MPI_SUCCESS) {
    int rank = 0;
    int processCount = 1;
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    for (size_t index = rank; index < fileManager.getMapFilesData().size();
    index+=processCount) {
      nights = fileManager.getMapNights(index);
      fileManager.readFileData(fileManager.getMapFilesData()[index],
      enchantedForest);  // Reads the map
      // Enchants the map
      enchanter(fileManager.getMapFilesData()[index], nights);
    }
    MPI_Finalize();
  }
  return 0;
}

/**
 * @brief Calls the method to change the matrix with the given rules, and
 * decides a name for a file where to save the changes.
 * @param mapFileName The map's name.
 * @param nights The number of times enchantForest is called.
 */
void Controller::enchanter(std::string mapFileName, int nights) {
  std::string outputFileName = "";
  std::string delimiter = ".";
  size_t nameEndPos = 0;

  // If the number of nights is positive, creates one file per night
  if (nights > 0) {
    for (int index = 1; index <= nights; index++) {
      // Looks for "." in the file's name, cuts it there, then appends an
      // output destination path, a "-", a number, and finally ".txt"
      nameEndPos = mapFileName.find(delimiter);
      outputFileName = "positive_output/" + mapFileName.substr(0, nameEndPos);
      outputFileName.append("-" + std::to_string(index) + ".txt");
      enchantedForest.enchantForest();  // Changes the matrix
      //  Calls the method to create a file
      fileManager.createFile(enchantedForest, outputFileName);
    }
  // If the number is negative, creates only one file with the last night
  } else if (nights < 0) {
    nights = nights * -1;  // Makes the number positive
    // Calls enchant forest that number of times
    for (int index = 1; index <= nights; index++) {
      enchantedForest.enchantForest();
    }
    // Looks for "." in the file's name, cuts it there, then appends an
    // output destination path, a "-", the number of nights, and finally ".txt"
    nameEndPos = mapFileName.find(delimiter);
    outputFileName = "negative_output/" + mapFileName.substr(0, nameEndPos);
    outputFileName.append("-" + std::to_string(nights) + ".txt");
    // Calls the method to create a file
    fileManager.createFile(enchantedForest, outputFileName);
  } else {  // If the number of nights is zero, there's nothing to do
    std::cout << "Error: number of nights can't be zero" << std::endl;
  }
}
