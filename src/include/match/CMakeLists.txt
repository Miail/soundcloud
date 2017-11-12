MESSAGE(“In match CMAKELIST”)

add_library(match STATIC match.cpp match.h)
target_include_directories(match PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(match PUBLIC cxx_defaulted_functions)
