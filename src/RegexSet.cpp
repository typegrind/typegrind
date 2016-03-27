
#include "RegexSet.h"

namespace typegrind
{
  void RegexSet::add(std::string regex)
  {
    try
    {
      strings.push_back(regex);
      regexes.push_back(std::regex(strings.back()));
    } catch(std::regex_error e)
    {
      // TODO: error reporting!
    }
  }

  bool RegexSet::matches(std::string query) const
  {
    for(auto const& regex: regexes)
    {
      if(std::regex_match(query, regex)) return true;
    }
    return false;
  }
}
