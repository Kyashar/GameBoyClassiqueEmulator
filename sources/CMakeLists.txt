cmake_minimum_required(VERSION 3.7)

add_subdirectory(modules)

add_executable(emulator srcs/main.cpp srcs/Cpu.cpp srcs/Cpu.hpp srcs/Register.hpp srcs/InstructionInfos.cpp srcs/InstructionInfos.hpp srcs/Memory.cpp srcs/Memory.hpp)

target_include_directories(emulator PRIVATE srcs modules)
target_link_libraries(emulator PRIVATE read_rom)
target_compile_options(emulator PRIVATE -Wall -Wextra)
