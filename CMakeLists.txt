cmake_minimum_required(VERSION 2.8)
project(10930_screenkeyboard_image_drawer_project CXX)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-lpthread")
add_compile_options(-std=c++11)
SET(CMAKE_CXX_STANDARD 14)

add_subdirectory(10930_screenkeyboard_image_drawer)
