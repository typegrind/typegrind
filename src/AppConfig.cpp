
#include "AppConfig.h"

#include <fstream>
#include <picojson.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

AppConfig::AppConfig(std::string filename)
: errorMessage("")
{
  // try to load config
  std::ifstream configFile(filename);

  boost::filesystem::path config_directory(filename);
  config_directory = boost::filesystem::absolute(filename).remove_filename();

  if(!configFile)
  {
    errorMessage = "typegrind.json not found!";
    return;
  }

  std::istreambuf_iterator<char> cfBegin(configFile);
  std::istreambuf_iterator<char> cfEnd;
  std::string err;
  picojson::value v;
  picojson::parse(v, cfBegin, cfEnd, &err);
  if (! err.empty()) {
    errorMessage = "Error while parsing typegrind.json: ";
    errorMessage += err;
    return;
  }

  if (!v.is<picojson::object>())
  {
    errorMessage = "typegrind.json is not an object!";
    return;
  }

  const picojson::value::object& mainConfig = v.get<picojson::object>();

  try
  {
    auto const& mappingC = mainConfig.at("mapping");
    if (!mappingC.is<picojson::object>())
    {
      errorMessage = "mapping is not an object!";
      return;
    }
    const picojson::value::object& mappingConfig = mappingC.get<picojson::object>();

    for (auto const& pair : mappingConfig)
    {
      boost::system::error_code src_error;
      std::string source_path = boost::filesystem::canonical(boost::filesystem::path(pair.first), config_directory, src_error).string();
      if (src_error)
      {
        errorMessage = "couldn't canonize " + pair.first;
      }
      // TODO: type check, we should allow multiple variations!
      std::string destination_path = boost::filesystem::absolute(boost::filesystem::path(pair.second.to_str()), config_directory).string();
      mapping.add(source_path, destination_path);
    }
  } catch(std::out_of_range oor)
  {
    errorMessage = "typegrind.json has no key named mapping!";
    return;
  }

  try
  {
    auto const& watchC = mainConfig.at("watch");
    if (!watchC.is<picojson::array>())
    {
      errorMessage = "watch is not an array!";
      return;
    }
    const picojson::value::array& watchConfig = watchC.get<picojson::array>();

    for (auto const& r : watchConfig)
    {
      methodMatches.add({ r.to_str(), "", 0 }); // TODO: type check
    }
  }
  catch(std::out_of_range oor)
  {
   // not an error
  }
}

bool AppConfig::isValid() const
{
  return errorMessage == "";
}

std::string AppConfig::getErrorMessage() const
{
  return errorMessage;
}

typegrind::DirectoryMapper const& AppConfig::getDirectoryMapping() const
{
  return mapping;
}

typegrind::MethodMatcher const& AppConfig::getMethodMatcher() const
{
  return methodMatches;
}
