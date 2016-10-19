
#pragma once

#include <string>

namespace typegrind {
namespace config {
// maps from a canonical path to another path
struct DirectoryMapping {
  std::string from;
  std::string to;

  bool apply(std::string& path) const;
};
}
}
