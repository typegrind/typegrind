
#include "AppConfig.h"

#include <fstream>
#include <picojson.h>

AppConfig::AppConfig(std::string filename)
: errorMessage("")
{
  // try to load config
  std::ifstream configFile(filename);

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
      mapping.add(pair.first, pair.second.to_str()); // TODO: type check the second!
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
