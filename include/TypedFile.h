
#pragma once

#include <initializer_list>
#include <string>
#include <vector>

#include "FileType.h"

class TypedFile {
 public:
  TypedFile(std::string filename, std::initializer_list<FileType> types);

  bool supports(FileType ft) const;

  std::string getFileName() const;

  TypedFile& addFileType(FileType ft);

 private:
  std::string fileName;
  std::vector<FileType> fileTypes;
};
