
#pragma once

#include <vector>
#include <regex>
#include <stdint.h>
#include <boost/optional.hpp>

namespace typegrind
{
  class MethodMatcher
  {
  public:
    struct MethodMatch
    {
      std::string regex;
      std::string custom_name;
      uint64_t    flags;
    };

    bool add(MethodMatch method) noexcept;

    boost::optional<MethodMatch> matches(std::string query) const noexcept;
  private:
    struct MatcherWithRegex
    {
      MethodMatch matcher;
      std::regex real_regex;

      MatcherWithRegex(MethodMatch mm);
    };

    std::vector<MatcherWithRegex> methods;
  };
}
