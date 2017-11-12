project(record)
MESSAGE( “In record CMAKELIST” )
# Include externalproject {portaudio} if lib/portaudio don't exist.

INCLUDE(ExternalProject)
ExternalProject_Add(project_portaudio
    GIT_REPOSITORY      https://git.assembla.com/portaudio.git
    PREFIX              lib/portaudio
    UPDATE_COMMAND      ""
    CONFIGURE_COMMAND   <SOURCE_DIR>/configure --enable-cxx
    BUILD_IN_SOURCE     0
    BUILD_COMMAND       make
    INSTALL_COMMAND     sudo make install
)
ExternalProject_Get_Property(project_portaudio BINARY_DIR)
ExternalProject_Get_Property(project_portaudio SOURCE_DIR)

SET(portaudio_lib_dir "${BINARY_DIR}/lib/.libs")
SET(portaudio_inc_dir "${SOURCE_DIR}/include")

add_library(record STATIC record.cpp record.h AudioFile.cpp AudioFile.h)

add_library(portaudio STATIC IMPORTED)

set_target_properties(portaudio PROPERTIES
    IMPORTED_LOCATION "${portaudio_lib_dir}/libportaudio.so")

set_property(TARGET portaudio APPEND PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES ${SOURCE_DIR}/include)

add_dependencies(portaudio project_portaudio)     # Not sure if this is allowed for imported targets though

find_package(Threads REQUIRED)
find_package(ALSA REQUIRED)

IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
   SET(EXTRA_LIBS rt asound jack)
ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Linux")

#
# this makes sure we have compiler flags that allow class::class() = default (>= C++11)
target_link_libraries(record PUBLIC  ${EXTRA_LIBS} ${ALSA_LIBRARIES} portaudio Threads::Threads )
target_compile_features(record PUBLIC cxx_defaulted_functions)
target_include_directories(record PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${project_portaudio})
