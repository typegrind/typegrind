
#include "DirectoryMapping.h"

#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

namespace typegrind
{
  bool DirectoryMapping::apply(std::string& path) const
  {
    if (boost::starts_with(path, from))
    {
      boost::replace_first(path, from, to);
      return true;
    }

    return false;
  }
}
