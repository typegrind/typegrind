
#include "typegrind/config/DirectoryMapping.h"

namespace typegrind {
namespace config {
bool DirectoryMapping::apply(std::string& path) const {
  if (from.size() <= path.size() && std::equal(from.begin(), from.end(), path.begin())) {
    path.replace(0, from.length(), to);
    return true;
  }

  return false;
}
}
}
