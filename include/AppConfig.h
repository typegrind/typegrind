
#pragma once

#include "DirectoryMapper.h"
#include "MethodMatcher.h"

#include <string>
#include <vector>

class AppConfig
{
public:
  AppConfig(std::string filename);

  typegrind::DirectoryMapper const& getDirectoryMapping() const;
  typegrind::MethodMatcher const& getMethodMatcher() const;
  bool isValid() const;
  std::string getErrorMessage() const;
  bool shouldPrependInclude() const;
  std::vector<std::string> getPrependInclude() const;
private:
  std::vector<std::string> prepend_include;
  typegrind::DirectoryMapper mapping;
  typegrind::MethodMatcher methodMatches;
  std::string errorMessage;
};
