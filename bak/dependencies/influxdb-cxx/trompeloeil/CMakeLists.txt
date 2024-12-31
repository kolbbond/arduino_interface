cmake_minimum_required(VERSION 3.1)

project(external-trompeloeil)
include(ExternalProject)

list(APPEND CMAKE_ARGS
    "-DCMAKE_PREFIX_PATH:PATH=${CMAKE_PREFIX_PATH}"
    "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}"
    "-DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}"
    "-DCMAKE_POSITION_INDEPENDENT_CODE=ON"
    "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
    "-DBUILD_SHARED_LIBS=ON"
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

message(STATUS "Preparing external project \"trompeloeil\" with args:")
foreach(CMAKE_ARG ${CMAKE_ARGS})
    message(STATUS "-- ${CMAKE_ARG}")
endforeach()

ExternalProject_Add(
    trompeloeil
    GIT_REPOSITORY https://github.com/rollbear/trompeloeil.git
    GIT_TAG main
    PREFIX trompeloeil 
    CMAKE_ARGS "${CMAKE_ARGS}"
    )

