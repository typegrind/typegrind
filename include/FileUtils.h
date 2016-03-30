
#pragma once

#include <string>

namespace typegrind
{
  namespace file_utils
  {
    bool canonize_path(std::string originalPath, std::string& output);

    bool ensure_directory_exists(std::string originalPath);
  };
};

