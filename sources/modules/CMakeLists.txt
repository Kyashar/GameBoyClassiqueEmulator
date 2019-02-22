cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)

add_library(read_rom SHARED read_rom/Reader.cpp read_rom/Reader.hpp)
add_library(gfx SHARED gfx/Screen.cpp gfx/Screen.hpp)


target_link_libraries(gfx PRIVATE sfml-audio sfml-system sfml-graphics sfml-window)
target_compile_options(gfx PRIVATE -o2)
target_compile_options(read_rom PRIVATE -o2)
