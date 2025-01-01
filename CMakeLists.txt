cmake_minimum_required(VERSION 3.20.0)
project(arduino_interfaces)

# fetch content?
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

# should be default for clang LSP support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add libraries
find_package(TCLAP REQUIRED)
find_package(Boost 1.66.0 REQUIRED system )
find_package(InfluxDB REQUIRED)
find_package(jsoncpp REQUIRED)

# maybe?
find_package(Armadillo)

# create helper library
add_library(extra 
    src/Log.cpp)

target_include_directories(extra PUBLIC
    include)

# serial monitor
add_executable(serial_monitor examples/serial_monitor.cpp)
target_link_libraries(serial_monitor
    extra
    Boost::system
)

# http_getter
add_executable(http_getter examples/http_getter.cpp)
target_link_libraries(http_getter
    extra 
    Boost::system
    jsoncpp
)

# influxwrite_http
add_executable(influxwrite examples/influxwrite.cpp)
target_link_libraries(influxwrite PUBLIC
    extra 
    Boost::system
    InfluxData::InfluxDB
    jsoncpp
)

