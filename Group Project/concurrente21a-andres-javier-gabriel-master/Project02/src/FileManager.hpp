// Copyright 2021 Andres Azofeifa-Garcia <andres.azofeifagarcia@ucr.ac.cr>
// Javier Molina-Herrera <javier.molinaherrera@ucr.ac.cr>
// Gabriel Zúñiga-Orozco <gabriel.zunigaorozco@ucr.ac.cr> CC-BY
// This program solves the Enchanted Forest problem

#include <fstream>
#include <string>
#include <vector>
#include "Forest.hpp"

#ifndef FILE_MANAGER_HPP

class FileManager {
 private:
  /// The file to be read
  std::ifstream inputFile;
  /// The file where to save results
  std::ofstream outputFile;
  /// The name of the job file
  std::string jobFileName;
  /// The path to the job file
  std::string jobFileDir;
  /// Lines read from the job file
  std::vector<std::string> mapFilesData;

 public:
  FileManager();
  ~FileManager();
  /// Gets the name of the job file
  std::string getJobFileName();
  /// Sets the name of the job file
  void setJobFileName(std::string fileName);
  /// Gets the path to the job file
  std::string getJobFileDir();
  /// Sets the path to the job file
  void setJobFileDir(std::string filePath);
  /// Gets the name of the job file from the arguments
  std::string getFileName(char* argv[]);
  /// Gets the path of the job file from the arguments
  std::string getFilePath(int argc, char* argv[]);
  /// Reads the data of a map
  void readFileData(std::string mapFileName, Forest& enchantedForest);
  /// Saves the lines of the job file
  void storeMapFilesData();
  /// Returns the number of nights to calculate for a map
  int getMapNights(size_t index);
  /// Creates a file to store the results
  void createFile(Forest& enchantedForest, std::string outputFileName);
  /// Creates the folders where to save the result files
  void createOutputDir();
  /// Returns the vector containing the lines of the job file
  std::vector<std::string> getMapFilesData();
};

#endif  //  FILE_MANAGER_HPP
