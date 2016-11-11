
#include "typegrind/file_utils/FileUtils.h"
#if defined _MSC_VER
// Typegrind requires VS "15"
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
using error_code = std::error_code;
#elif __has_include("experimental/filesystem")
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
using error_code = std::error_code;
#elif __has_include("boost/filesystem.hpp")
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
using error_code = boost::system::error_code;
#else
#error Typegrind requires std::experimental::filesystem or boost::filesystem
#endif

namespace typegrind {
namespace file_utils {
bool canonizePath(std::string OriginalPath, std::string& Output) {
  error_code FsError;
  fs::path CanonicalPath = fs::canonical(OriginalPath, FsError);
  if (!FsError) Output = CanonicalPath.string();
  return !FsError;
}

bool ensureDirectoryExists(std::string FilePath) {
  error_code FsError;
  fs::path TargetPath(FilePath);
  TargetPath = TargetPath.parent_path();

  fs::create_directories(TargetPath, FsError);
  return !FsError;
}

bool findConfigUsingParents(std::string Filename, std::string& Output) {
  fs::path FullFilename = fs::system_complete(Filename);
  fs::path Directory = FullFilename.parent_path();

  error_code FsError;

  while (!fs::is_empty(Directory, FsError) && !FsError) {
    fs::path targetConfig = Directory / "/typegrind.json";
    if (fs::exists(targetConfig)) {
      Output = targetConfig.string();
      return true;
    }

    Directory = Directory.parent_path();
  }

  return false;
}
}
}
