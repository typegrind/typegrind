

include_directories(${Boost_INCLUDE_DIRS}) 
include_directories(${TYPEGRIND_ROOT}/vendor/picojson/) 

add_library(typegrind-core STATIC
  AppConfig.cpp
  RegexSet.cpp
  DirectoryMapper.cpp
  DirectoryMapping.cpp
)
set_property(TARGET typegrind-core PROPERTY CXX_STANDARD 14)
