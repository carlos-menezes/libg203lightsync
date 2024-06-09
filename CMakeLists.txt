cmake_minimum_required(VERSION 3.10)
project(g203lightsync LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/libusb-cmake/libusb
)

# Add libusb-cmake as a subdirectory
add_subdirectory(libusb-cmake)

# Library
add_library(libg203lightsync SHARED
        include/libg203lightsync.h
        src/libg203lightsync.cpp
)

add_library(libg203lightsync_static STATIC
        include/libg203lightsync.h
        src/libg203lightsync.cpp
)
set_target_properties(libg203lightsync_static PROPERTIES OUTPUT_NAME libg203lightsync)

# Link with libusb
target_link_libraries(libg203lightsync
        usb-1.0
)
target_link_libraries(libg203lightsync_static
        usb-1.0
)


# Optionally compile `examples`
add_executable(set_solid_color examples/set_solid_color.cpp)
set_target_properties(set_solid_color PROPERTIES EXCLUDE_FROM_ALL true)
target_link_libraries(set_solid_color libg203lightsync)

add_executable(google examples/google.cpp)
set_target_properties(google PROPERTIES EXCLUDE_FROM_ALL true)
target_link_libraries(google libg203lightsync)

