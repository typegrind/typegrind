
#pragma once

#include "DirectoryMapper.h"
#include "MethodMatcher.h"

class AppConfig
{
public:
  AppConfig(std::string filename);

  typegrind::DirectoryMapper const& getDirectoryMapping() const;
  typegrind::MethodMatcher const& getMethodMatcher() const;
  bool isValid() const;
  std::string getErrorMessage() const;
private:
  typegrind::DirectoryMapper mapping;
  typegrind::MethodMatcher methodMatches;
  std::string errorMessage;
};
