
#pragma once

#include <string>

namespace typegrind {
namespace file_utils {
bool canonizePath(std::string OriginalPath, std::string& Output);

bool ensureDirectoryExists(std::string OriginalPath);

bool findConfigUsingParents(std::string Filename, std::string& Output);
}
}
