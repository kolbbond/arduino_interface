cmake_minimum_required(VERSION 3.0.0)
project(arduino_interfaces)

# fetch content?
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

# add libraries
find_package(TCLAP REQUIRED)
find_package(Boost 1.66.0 REQUIRED system )
find_package(InfluxDB REQUIRED)

# maybe?
find_package(Armadillo)

# create helper library
add_library(extra Log.cpp)

# serial monitor
add_executable(serial_monitor serial_monitor.cpp)
target_link_libraries(serial_monitor
    extra
    Boost::system
)

# http_getter
add_executable(http_getter http_getter.cpp)
target_link_libraries(http_getter
    extra 
    Boost::system
)

# influxwrite_http
add_executable(influxwrite influxwrite.cpp)
target_link_libraries(influxwrite
    extra 
    Boost::system
    InfluxDB
)

