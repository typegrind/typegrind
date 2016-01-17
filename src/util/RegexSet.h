
#pragma once

#include <vector>
#include <regex>

namespace typegrind
{
  class RegexSet
  {
  public:
    void add(std::string regex);

    bool matches(std::string query) const;
  private:
    std::vector<std::regex> regexes;
    std::vector<std::string> strings;
  };
}
