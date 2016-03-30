
#include "FileUtils.h"
#include <boost/filesystem.hpp>

namespace typegrind
{
  namespace file_utils
  {
    bool canonize_path(std::string originalPath, std::string& output)
    {
      boost::system::error_code fs_error;
      boost::filesystem::path canonicalPath = boost::filesystem::canonical(originalPath, fs_error);
      if(!fs_error) output = canonicalPath.string();
      return !fs_error;
    }

    bool ensure_directory_exists(std::string filePath)
    {
      boost::system::error_code fs_error;
      boost::filesystem::path targetPath(filePath);
      targetPath = targetPath.parent_path();

      boost::filesystem::create_directories(targetPath, fs_error);
      return !fs_error;
    }
  };
};

