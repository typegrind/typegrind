
#pragma once

#include <vector>
#include "DirectoryMapping.h"

namespace typegrind {
namespace config {
class DirectoryMapper {
 public:
  void add(std::string from, std::string to);
  bool apply(std::string& path) const;  // returns = found a mapping

 private:
  std::vector<DirectoryMapping> mappings;
};
}
}
