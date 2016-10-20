
#include "typegrind/config/MethodMatcher.h"

namespace typegrind {
namespace config {
bool MethodMatcher::add(MethodMatch method) noexcept {
  methods.push_back(MatcherWithRegex(method));
  return true;
}

llvm::Optional<MethodMatcher::MethodMatch> MethodMatcher::matches(std::string query) const
    noexcept {
  for (auto const& item : methods) {
    if (std::regex_match(query, item.real_regex)) return item.matcher;
  }
  return llvm::Optional<MethodMatcher::MethodMatch>{};
}

MethodMatcher::MatcherWithRegex::MatcherWithRegex(MethodMatch mm)
    : matcher(mm), real_regex(matcher.regex) {}
}
}
