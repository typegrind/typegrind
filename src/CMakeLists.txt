

include_directories(${Boost_INCLUDE_DIRS}) 
include_directories(${TYPEGRIND_ROOT}/vendor/picojson/) 

add_library(typegrind-core STATIC
  typegrind/config/AppConfig.cpp
  typegrind/config/MethodMatcher.cpp
  typegrind/config/DirectoryMapper.cpp
  typegrind/config/DirectoryMapping.cpp
  typegrind/config/TypedFile.cpp
  typegrind/file_utils/FileUtils.cpp
)
target_link_libraries(typegrind-core ${Boost_LIBRARIES})
set_property(TARGET typegrind-core PROPERTY CXX_STANDARD 14)
