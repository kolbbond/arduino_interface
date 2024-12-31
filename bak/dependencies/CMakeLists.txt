# use External Project to build dependencies

cmake_minimum_required(VERSION 3.10.2)

project(dependencies)

# build influxdb-cxx
add_subdirectory(influxdb-cxx)
