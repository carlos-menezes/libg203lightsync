cmake_minimum_required(VERSION 3.28)
project(g203lightsync LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(libusb-cmake)
add_subdirectory(examples)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libusb-cmake/libusb)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(g203lightsync STATIC include/libg203lightsync.h src/libg203lightsync.cpp)
target_link_libraries(g203lightsync usb-1.0)
add_dependencies(g203lightsync usb-1.0)