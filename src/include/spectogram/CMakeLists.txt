project(spectogram)
MESSAGE(“In spectogram CMAKELIST”)
MESSAGE( "${CMAKE_CURRENT_SOURCE_DIR}/base/base.h" )

include_directories(${record_SOURCE_DIR})

SET(HEADERS
    #${CMAKE_CURRENT_SOURCE_DIR}/base/base.h
    #${CMAKE_CURRENT_SOURCE_DIR}/fftw/fftw.h
    #${CMAKE_CURRENT_SOURCE_DIR}/base/base.h
    #${CMAKE_CURRENT_SOURCE_DIR}/filter/filter.h
    #${CMAKE_CURRENT_SOURCE_DIR}/gplot/gplot.h
    #${CMAKE_CURRENT_SOURCE_DIR}/image/image.h
    #${CMAKE_CURRENT_SOURCE_DIR}/kalman/kalman.h
    #${CMAKE_CURRENT_SOURCE_DIR}/parser/parser.h
    #${CMAKE_CURRENT_SOURCE_DIR}/resampling/resampling.h
    #${CMAKE_CURRENT_SOURCE_DIR}/spectrum/spectrum.h
    #${CMAKE_CURRENT_SOURCE_DIR}/timing/timing.h
    #${CMAKE_CURRENT_SOURCE_DIR}/window/window.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sigpack.h
    )

find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})

add_library(spectogram STATIC spectogram.cpp spectogram.h ${HEADERS})

target_link_libraries(spectogram PUBLIC ${ARMADILLO_LIBRARIES})
target_include_directories(spectogram PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(spectogram PUBLIC cxx_defaulted_functions)

MESSAGE(“out spectogram CMAKELIST”)
