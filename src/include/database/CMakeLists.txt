project(database)
MESSAGE(“In database CMAKELIST”)


#
# Build static library
add_library(database database.cpp database.h)
include_directories(${record_SOURCE_DIR} ${spectogram_SOURCE_DIR})

target_compile_features(database PUBLIC cxx_defaulted_functions)
target_include_directories(database PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${record_SOURCE_DIR} ${spectogram_SOURCE_DIR})
