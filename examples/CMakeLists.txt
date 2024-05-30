cmake_minimum_required(VERSION 3.28)
project(examples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

get_filename_component(PARENT_DIR ../ ABSOLUTE)
include_directories(${PARENT_DIR}/include)

add_executable(solid_color solid_color.cpp)
target_link_libraries(solid_color PRIVATE g203lightsync)

add_executable(siren siren.cpp)
target_link_libraries(siren PRIVATE g203lightsync)