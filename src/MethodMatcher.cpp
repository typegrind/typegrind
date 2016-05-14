
#include "MethodMatcher.h"

namespace typegrind {
bool MethodMatcher::add(MethodMatch method) noexcept {
  try {
    methods.push_back(MatcherWithRegex(method));
    return true;
  } catch (std::regex_error e) {
    return false;
  }
}

boost::optional<MethodMatcher::MethodMatch> MethodMatcher::matches(std::string query) const
    noexcept {
  for (auto const& item : methods) {
    if (std::regex_match(query, item.real_regex)) return item.matcher;
  }
  return boost::none;
}

MethodMatcher::MatcherWithRegex::MatcherWithRegex(MethodMatch mm)
    : matcher(mm), real_regex(matcher.regex) {}
}
