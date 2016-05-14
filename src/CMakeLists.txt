

include_directories(${Boost_INCLUDE_DIRS}) 
include_directories(${TYPEGRIND_ROOT}/vendor/picojson/) 

add_library(typegrind-core STATIC
  AppConfig.cpp
  MethodMatcher.cpp
  DirectoryMapper.cpp
  DirectoryMapping.cpp
  FileUtils.cpp
  TypedFile.cpp
)
target_link_libraries(typegrind-core ${Boost_LIBRARIES})
set_property(TARGET typegrind-core PROPERTY CXX_STANDARD 14)
