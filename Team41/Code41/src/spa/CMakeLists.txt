# include all directories of /src/, e.g /src/SimpleParser/*.cpp, /src/Common/*.cpp
file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.hpp")
add_library(spa ${srcs} ${headers} src/DesignExtractor/DesignExtractor.h src/DesignExtractor/ModifiesExtractor.h src/DesignExtractor/ModifiesExtractor.cpp src/DesignExtractor/UsesExtractor.h src/DesignExtractor/UsesExtractor.cpp)
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
