
#include "TypedFile.h"

#include <algorithm>

TypedFile::TypedFile(std::string filename, std::initializer_list<FileType> types)
    : fileName(fileName), fileTypes(types) {}

bool TypedFile::supports(FileType ft) const {
  return std::find(fileTypes.begin(), fileTypes.end(), ft) != fileTypes.end();
}

std::string TypedFile::getFileName() const { return fileName; }

TypedFile& TypedFile::addFileType(FileType ft) {
  if (!supports(ft)) {
    fileTypes.push_back(ft);
  }
  return *this;
}
