cmake_minimum_required(VERSION 2.6)

project("Eureka")

set(CMAKE_BUILD_TYPE Debug)


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

find_package(SFML COMPONENTS window graphics system)
find_package(Box2D)

set(EUREKA_INC_DIR ${CMAKE_SOURCE_DIR}/inc)

include(${CMAKE_MODULE_PATH}/constants.cmake) # this should be first
include(${CMAKE_MODULE_PATH}/resource_config.cmake) #gen resources.h

add_subdirectory(src)

