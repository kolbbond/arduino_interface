cmake_minimum_required(VERSION 3.1)

project(external-influxdb-cxx)
include(ExternalProject)

# hardcode local install path
set(CMAKE_INSTALL_PREFIX "~/.local")

list(APPEND CMAKE_ARGS
    "-DCMAKE_PREFIX_PATH:PATH=${CMAKE_PREFIX_PATH}"
    "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}"
    "-DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}"
    "-DCMAKE_POSITION_INDEPENDENT_CODE=ON"
    "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
    "-DBUILD_SHARED_LIBS=OFF"
    "-DJSONCPP_WITH_CMAKE_PACKAGE=ON"
    "-DJSONCPP_WITH_TESTS=OFF"
    )

if(IOS)
    list(APPEND CMAKE_ARGS
        "-DPLATFORM=${PLATFORM}"
        "-DDEPLOYMENT_TARGET=${DEPLOYMENT_TARGET}"
        "-DENABLE_STRICT_TRY_COMPILE=${ENABLE_STRICT_TRY_COMPILE}"
        )
endif()

message(STATUS "Preparing external project \"influxdb-cxx\" with args:")
foreach(CMAKE_ARG ${CMAKE_ARGS})
    message(STATUS "-- ${CMAKE_ARG}")
endforeach()

ExternalProject_Add(
    influxdb-cxx
    GIT_REPOSITORY https://github.com/offa/influxdb-cxx.git
    #GIT_TAG 42e892d96e47b1f6e29844cc705e148ec4856448
    PREFIX influxdb-cxx 
    CMAKE_ARGS "${CMAKE_ARGS}"
    )

# add influxdb-cxx dependencies
add_subdirectory(cpr)
add_subdirectory(Catch2)
add_subdirectory(trompeloeil)
add_dependencies(influxdb-cxx
    cpr
    Catch2
    trompeloeil
)
