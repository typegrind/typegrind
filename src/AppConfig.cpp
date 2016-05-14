
#include "AppConfig.h"

#include <fstream>

#define PICOJSON_USE_INT64
#include <picojson.h>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

AppConfig::AppConfig(std::string filename) : errorMessage("") {
  // try to load config
  std::ifstream configFile(filename);

  boost::filesystem::path config_directory(filename);
  config_directory = boost::filesystem::absolute(filename).remove_filename();

  if (!configFile) {
    errorMessage = "typegrind.json not found!";
    return;
  }

  std::istreambuf_iterator<char> cfBegin(configFile);
  std::istreambuf_iterator<char> cfEnd;
  std::string err;
  picojson::value v;
  picojson::parse(v, cfBegin, cfEnd, &err);
  if (!err.empty()) {
    errorMessage = "Error while parsing typegrind.json: ";
    errorMessage += err;
    return;
  }

  if (!v.is<picojson::object>()) {
    errorMessage = "typegrind.json is not an object!";
    return;
  }

  const picojson::value::object& mainConfig = v.get<picojson::object>();

  try {
    auto prepend_it = mainConfig.find("prepend_include");
    if (prepend_it != mainConfig.end()) {
      if (prepend_it->second.is<std::string>()) {
        prepend_include.push_back(prepend_it->second.to_str());
      } else if (prepend_it->second.is<picojson::array>()) {
        const picojson::value::array& prepends = prepend_it->second.get<picojson::array>();
        for (auto const& prep : prepends) {
          prepend_include.push_back(prep.to_str());
        }
      }
    }

    auto const& mappingC = mainConfig.at("mapping");
    if (!mappingC.is<picojson::object>()) {
      errorMessage = "mapping is not an object!";
      return;
    }
    const picojson::value::object& mappingConfig = mappingC.get<picojson::object>();

    for (auto const& pair : mappingConfig) {
      boost::system::error_code src_error;
      std::string source_path = boost::filesystem::canonical(boost::filesystem::path(pair.first),
                                                             config_directory, src_error)
                                    .string();
      if (src_error) {
        errorMessage = "couldn't canonize " + pair.first;
      }
      // TODO: type check, we should allow multiple variations!
      std::string destination_path =
          boost::filesystem::absolute(boost::filesystem::path(pair.second.to_str()),
                                      config_directory)
              .string();
      mapping.add(source_path, destination_path);
    }
  } catch (std::out_of_range oor) {
    errorMessage = "typegrind.json has no key named mapping!";
    return;
  }

  try {
    auto const& watchC = mainConfig.at("watch");
    if (!watchC.is<picojson::array>()) {
      errorMessage = "watch is not an array!";
      return;
    }
    const picojson::value::array& watchConfig = watchC.get<picojson::array>();

    for (auto const& r : watchConfig) {
      std::string regex;
      std::string custom_name;
      uint64_t flags = 0;
      if (r.is<std::string>()) {
        regex = r.to_str();
      } else if (r.is<picojson::object>()) {
        const picojson::value::object& actualWatch = r.get<picojson::object>();

        auto regex_it = actualWatch.find("regex");
        if (regex_it != actualWatch.end()) {
          regex = regex_it->second.to_str();
        } else {
          errorMessage = "object in watch without regex property!";
          return;
        }

        auto name_it = actualWatch.find("name");
        if (name_it != actualWatch.end()) {
          custom_name = name_it->second.to_str();
        }

        auto flags_it = actualWatch.find("flags");
        if (flags_it != actualWatch.end() && flags_it->second.is<int64_t>()) {
          flags = flags_it->second.get<int64_t>();  // TODO: named flag support
        }
      } else {
        errorMessage = "invalid record in watch!";
        return;
      }
      methodMatches.add({regex, custom_name, flags});
    }
  } catch (std::out_of_range oor) {
    // not an error
  }
}

bool AppConfig::isValid() const { return errorMessage == ""; }

std::string AppConfig::getErrorMessage() const { return errorMessage; }

typegrind::DirectoryMapper const& AppConfig::getDirectoryMapping() const { return mapping; }

typegrind::MethodMatcher const& AppConfig::getMethodMatcher() const { return methodMatches; }

bool AppConfig::shouldPrependInclude() const { return !prepend_include.empty(); }

std::vector<std::string> AppConfig::getPrependInclude() const { return prepend_include; }
