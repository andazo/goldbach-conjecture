// Copyright 2021 Andres Azofeifa-Garcia <andres.azofeifagarcia@ucr.ac.cr>
// Javier Molina-Herrera <javier.molinaherrera@ucr.ac.cr>
// Gabriel Zúñiga-Orozco <gabriel.zunigaorozco@ucr.ac.cr> CC-BY
// This program solves the Enchanted Forest problem

#include <dirent.h>
#include <unistd.h>
#include "FileManager.hpp"

/**
 * @brief Construct a new FileManager::FileManager object.
 * 
 */
FileManager::FileManager() { }

/**
 * @brief Destroy the FileManager::FileManager object.
 * 
 */
FileManager::~FileManager() { }

/**
 * @brief Returns the name of the job file to be read.
 * 
 * @return jobFileName : The name of the file.
 */
std::string FileManager::getJobFileName() {
  return jobFileName;
}

/**
 * @brief Sets the name of the job file to be read.
 * 
 * @param fileName The name of the file.
 */
void FileManager::setJobFileName(std::string fileName) {
  jobFileName = fileName;
}

/**
 * @brief Returns the path of the job file to be read.
 * 
 * @return jobFileDir : The path of the file.
 */
std::string FileManager::getJobFileDir() {
  return jobFileDir;
}

/**
 * @brief Sets the name of the job file to be read.
 * 
 * @param filePath The path of the file.
 */
void FileManager::setJobFileDir(std::string filePath) {
  jobFileDir = filePath;
}

/**
 * @brief Gets the input file's path from the arguments.
 * @param argc The number of parameters.
 * @param argv The parameters.
 * @return filePath : The path to the file.
 */
std::string FileManager::getFilePath(int argc, char* argv[]) {
  std::string filePath = "";
  if (argc >= 3) {
    filePath = argv[2];
    if (filePath != "/" && filePath != "./") {
      int lastChar = filePath.length() - 1;
      if (filePath[lastChar] != '/') {
        filePath.append("/");
      }
    } else {
      filePath = "";
    }
  }
  return filePath;
}

/**
 * @brief Gets the input file's name from the arguments.
 * @param argv The parameters.
 * @return argv[1] : The parameter containing the name.
 */
std::string FileManager::getFileName(char* argv[]) {
  return argv[1];
}

/**
 * @brief Returns a vector with the data of the map files.
 * 
 * @return mapFilesData : The vector containing the data.
 */
std::vector<std::string> FileManager::getMapFilesData() {
  return mapFilesData;
}

/**
 * @brief Reads the map and creates a matrix with that data.
 * @param mapFileName The map's name.
 * @param inputFileDir The map's path.
 */
void FileManager::readFileData
(std::string mapFileName, Forest& enchantedForest) {
  std::string fileData = "";
  int delimiterPosition = 0;
  std::string delimiter = " ";
  size_t rows = 0;
  size_t columns = 0;
  inputFile.open(jobFileDir + mapFileName);  // Tries to open the map
  if (inputFile.is_open()) {  // If it's open
     bool isFirstLine = true;
     size_t row_index = 1;
     std::vector<std::vector<char>> temporalMap;
    while (getline(inputFile, fileData)) {  // While there are lines left
      if (isFirstLine) {  // If it's the first line of the file,
      // it contains the matrix dimensions, separated by space
        delimiterPosition = fileData.find(delimiter);  // Search for a space
        rows = stoi(fileData.substr(0, delimiterPosition));  // Sets the rows
        fileData.erase(0, delimiterPosition + 1);
        columns = stoi(fileData);  // Sets the columns
        // Creates a matrix with those numbers
        temporalMap = enchantedForest.createForestMap(rows, columns);
        isFirstLine = false;
      } else {  // Stores the data from the file in the matrix
          for (size_t column_index = 0; column_index < columns;
            column_index++) {
            temporalMap[row_index][column_index+1] = fileData[column_index];
          }
        row_index++;
      }
    }
    inputFile.close();  // Closes the map
    enchantedForest.setActualNight(temporalMap, rows, columns);
    enchantedForest.setNextNight(temporalMap);
  } else {  // If the map can't be opened
    std::cout << "Error: could not open file: " << mapFileName << std::endl;
  }
}

/**
 * @brief Reads the job file and stores it's data in a vector.
 * @param mapFileName The map's name.
 * @param inputFileDir The map's path.
 */
void FileManager::storeMapFilesData() {
  std::string mapData;  // string of data from the file
  inputFile.open(jobFileDir + jobFileName);
  while (getline(inputFile, mapData)) {
      mapFilesData.push_back(mapData);
  }
  inputFile.close();
}

/**
 * @brief Returns the number of nights to be calculated in a map.
 * @param index The position of the map in the vector containing all the maps.
 * @return nights : The number of nights of the respective map.
 */
int FileManager::getMapNights(size_t index) {
  size_t nameEndPos = 0;  // Position of the end of a name in a string
  std::string delimiter = " ";  // Character to look for to place nameEndPos
  nameEndPos = mapFilesData[index].find(delimiter);
  std::string mapFileName = mapFilesData[index].substr(0, nameEndPos);
  std::string nights = mapFilesData[index].erase(0, nameEndPos + 1);
  mapFilesData[index] = mapFileName;
  return stoi(nights);
}

/**
 * @brief Checks if the folders where to save the results already exist,
 * and if they don't, then creates them.
 */
void FileManager::createOutputDir() {
  DIR* posOutputDir = opendir("positive_output");  // Tries to open the
    // positive_output folder
    if (posOutputDir) {  // If directory opens, it exists
      closedir(posOutputDir);  // Close it
    } else if (ENOENT == errno) {  // If directory does not exist
      system("mkdir positive_output");  // Create it
    }

    DIR* negOutputDir = opendir("negative_output");  // Tries to open the
    // negative_output folder
    if (negOutputDir) {  // If directory opens, it exists
      closedir(negOutputDir);  // Close it
    } else if (ENOENT == errno) {  // If directory does not exist
      system("mkdir negative_output");  // Create it
    }
}

/**
 * @brief Creates a file with the name decided by enchanter, saves the data
 * from the matrix, and saves the file in the respective output folder
 * @param enchantedForest The Forest object to use it's methods
 * @param outputFileName The map's name.
 */
void FileManager::createFile
(Forest& enchantedForest, std::string outputFileName) {
  outputFile.open(outputFileName);  // Create the file
  // Copy the matrix
  outputFile << enchantedForest.nextNightToString();
  outputFile.close();  // Close the file
}
