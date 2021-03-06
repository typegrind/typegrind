
#include "typegrind/config/DirectoryMapper.h"

namespace typegrind {
namespace config {
void DirectoryMapper::add(std::string from, std::string to) { mappings.push_back({from, to}); }

bool DirectoryMapper::apply(std::string& path) const {
  for (auto const& mapping : mappings) {
    if (mapping.apply(path)) {
      return true;
    }
  }
  return false;
}
}
}
