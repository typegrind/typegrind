
#pragma once

#include "DirectoryMapper.h"
#include "MethodMatcher.h"
#include "TypedFile.h"

#include <string>
#include <vector>

class AppConfig {
 public:
  AppConfig(std::string filename);

  typegrind::DirectoryMapper const& getDirectoryMapping() const;
  typegrind::MethodMatcher const& getMethodMatcher() const;
  bool isValid() const;
  std::string getErrorMessage() const;
  bool shouldPrependInclude() const;
  std::vector<std::string> getPrependInclude(FileType forType) const;

 private:
  TypedFile& getPrependRecordFor(std::string filename);

  std::vector<TypedFile> prepend_include;
  typegrind::DirectoryMapper mapping;
  typegrind::MethodMatcher methodMatches;
  std::string errorMessage;
};
