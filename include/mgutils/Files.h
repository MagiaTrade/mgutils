//
// Created by Arthur Motelevicz on 26/08/24.
//

#ifndef MGUTILS_FILES_H
#define MGUTILS_FILES_H

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Exceptions.h"

namespace mgutils {

  class Files {
  public:
    // Check if the path is a file
    static bool isFile(const std::string& path) {
      return std::filesystem::is_regular_file(std::filesystem::path(path));
    }

    // Check if the path is a directory
    static bool isDirectory(const std::string& path) {
      return std::filesystem::is_directory(std::filesystem::path(path));
    }

    // Check if the path is absolute
    static bool isAbsolutePath(const std::string& path) {
      return std::filesystem::path(path).is_absolute();
    }

    // Check if the path is relative
    static bool isRelativePath(const std::string& path) {
      return std::filesystem::path(path).is_relative();
    }

    // Read the contents of a file into a string
    static std::string readFile(const std::string& path) {
      std::ifstream file(path);
      if (!file.is_open()) {
        throw FilesException("Could not open file: " + path);
      }

      std::stringstream buffer;
      buffer << file.rdbuf();
      return buffer.str();
    }

    // Write a string to a file
    static void writeFile(const std::string& path, const std::string& content) {
      std::ofstream file(path);
      if (!file.is_open()) {
        throw FilesException("Could not open file for writing: " + path);
      }

      file << content;
    }

    // List all files in a directory
    static std::vector<std::string> listFiles(const std::string& directory) {
      std::vector<std::string> files;
      for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
          files.push_back(entry.path().string());
        }
      }
      return files;
    }

    // List all directories in a directory
    static std::vector<std::string> listDirectories(const std::string& directory) {
      std::vector<std::string> directories;
      for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_directory()) {
          directories.push_back(entry.path().string());
        }
      }
      return directories;
    }

    // Append a component to a path
    static std::string appendPath(const std::string& base, const std::string& toAppend) {
      return (std::filesystem::path(base) / std::filesystem::path(toAppend)).string();
    }

    // Remove the last component of a path
    static std::string removeLastComponent(const std::string& path) {
      std::filesystem::path p(path);
      return p.parent_path().string();
    }

    // Create a directory (including intermediate directories)
    static void createDirectory(const std::string& path, bool createIntermediates = true) {
      std::filesystem::path p(path);
      if (createIntermediates) {
        std::filesystem::create_directories(p);
      } else {
        std::filesystem::create_directory(p);
      }
    }

    // Create an empty file
    static void createFile(const std::string& path) {
      std::ofstream file(path);
      if (!file.is_open()) {
        throw FilesException("Could not create file: " + path);
      }
      // File created, nothing else to do
    }
  };

} // namespace mgutils

#endif //MGUTILS_FILES_H
