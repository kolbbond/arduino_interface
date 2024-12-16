cmake_minimum_required(VERSION 3.0.0)
project(arduino_interfaces)

# fetch content?
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

# add libraries
find_package(TCLAP REQUIRED)
find_package(Boost 1.66.0 REQUIRED system)

# create helper library
add_library(extra Log.cpp)

add_executable(serial_monitor serial_monitor.cpp)
target_link_libraries(serial_monitor
    extra
    Boost::system
)

