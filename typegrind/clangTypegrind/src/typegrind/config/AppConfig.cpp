
#include "typegrind/config/AppConfig.h"

#include <fstream>

namespace typegrind {
namespace config {

AppConfig::AppConfig() : errorMessage("") {}

bool AppConfig::isValid() const { return errorMessage == ""; }

std::string AppConfig::getErrorMessage() const { return errorMessage; }

DirectoryMapper const& AppConfig::getDirectoryMapping() const { return mapping; }

MethodMatcher const& AppConfig::getMethodMatcher() const { return methodMatches; }

bool AppConfig::shouldPrependInclude() const { return !prepend_include.empty(); }

std::vector<std::string> AppConfig::getPrependInclude(FileType forType) const {
  std::vector<std::string> ret;
  for (auto const& v : prepend_include) {
    if (v.supports(forType)) ret.push_back(v.getFileName());
  }
  return ret;
}

TypedFile& AppConfig::getPrependRecordFor(std::string filename) {
  auto it = std::find_if(prepend_include.begin(), prepend_include.end(),
                         [filename](auto& v) { return v.getFileName() == filename; });
  if (it != prepend_include.end()) return *it;

  prepend_include.push_back(TypedFile(filename, {}));
  return prepend_include.back();
}
}
}
