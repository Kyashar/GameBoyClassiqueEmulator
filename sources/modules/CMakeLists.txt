cmake_minimum_required(VERSION 3.7)

add_library(read_rom SHARED read_rom/Reader.cpp read_rom/Reader.hpp)
add_library(gfx SHARED gfx/Screen.cpp gfx/Screen.hpp)


target_link_libraries(gfx PRIVATE sfml-audio sfml-system sfml-graphics sfml-window)
