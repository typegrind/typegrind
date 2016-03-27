
#pragma once

#include "DirectoryMapper.h"
#include "RegexSet.h"

class AppConfig
{
public:
  AppConfig(std::string filename);

  typegrind::DirectoryMapper const& getDirectoryMapping() const;
  typegrind::RegexSet const& getRegexSet() const;
  bool isValid() const;
  std::string getErrorMessage() const;
private:
  typegrind::DirectoryMapper mapping;
  typegrind::RegexSet regexes;
  std::string errorMessage;
};
