#ifndef MGUTILS_CSVLOADER_H
#define MGUTILS_CSVLOADER_H

#include "rapidcsv.h"
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include "Exceptions.h"

namespace mgutils {

  class CsvLoader {
  public:
    // Constructor with optional delimiter
    explicit CsvLoader(const std::string& filename, char delimiter = ',') {
      try {
        // Pass delimiter parameter to the rapidcsv::Document
        _document = std::make_unique<rapidcsv::Document>(filename, rapidcsv::LabelParams(0, -1), rapidcsv::SeparatorParams(delimiter));
      } catch (const std::exception& e) {
        throw CsvReadException("Failed to load CSV file: " + std::string(e.what()));
      }
    }

    template <typename T>
    std::vector<T> getColumn(const std::string& columnName) const {
      try {
        return _document->GetColumn<T>(columnName);
      } catch (const std::exception& e) {
        throw  CsvUsageException("Failed to get column: " + std::string(e.what()));
      }
    }

    template <typename T>
    std::vector<T> getRow(const size_t rowIndex) const {
      try {
        return _document->GetRow<T>(rowIndex);
      } catch (const std::exception& e) {
        throw CsvUsageException("Failed to get row: " + std::string(e.what()));
      }
    }

    template <typename T>
    T getCell(const std::string& columnName, const size_t rowIndex) const {
      try {
        return _document->GetCell<T>(columnName, rowIndex);
      } catch (const std::exception& e) {
        throw CsvUsageException("Failed to get cell: " + std::string(e.what()));
      }
    }

    void save(const std::string& filename) const {
      try {
        _document->Save(filename);
      } catch (const std::exception& e) {
        throw CsvWriteException("Failed to save CSV file: " + std::string(e.what()));
      }
    }

    // New method to get all data as a vector of vector of strings
    std::vector<std::vector<std::string>> getAllData() const {
      try {
        std::vector<std::vector<std::string>> data;
        size_t rowCount = _document->GetRowCount();
        size_t colCount = _document->GetColumnCount();

        for (size_t i = 0; i < rowCount; ++i) {
          std::vector<std::string> row;
          for (size_t j = 0; j < colCount; ++j) {
            row.push_back(_document->GetCell<std::string>(j, i));
          }
          data.push_back(row);
        }

        return data;
      } catch (const std::exception& e) {
        throw CsvReadException("Failed to get all data: " + std::string(e.what()));
      }
    }

  private:
    std::unique_ptr<rapidcsv::Document> _document;
  };

} // namespace mgutils

#endif //MGUTILS_CSVLOADER_H
