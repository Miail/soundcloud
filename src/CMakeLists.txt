MESSAGE(“In src CMAKELIST”)

#
# Build everything in include/ directory
add_subdirectory(include)
#

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Main executable target
add_executable(cmakeDemo main.cpp)

# These libraries get built in include/*/, CMake will auto-set required
# compiler flags and include paths from their definitions
target_link_libraries(cmakeDemo record ${portaudio})
target_link_libraries(cmakeDemo database ${record})
target_link_libraries(cmakeDemo match)
target_link_libraries(cmakeDemo spectogram)
