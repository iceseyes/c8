cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0167 NEW)
project(chip8 CXX)

include(CheckIncludeFileCXX)
include(${CMAKE_SOURCE_DIR}/cmake/warnings.cmake)

option(WITH_TESTS "Enable test module" ON)

set(CMAKE_CXX_STANDARD 20)

find_package(Threads)
find_package(Boost REQUIRED COMPONENTS program_options)
find_package(SDL2 CONFIG REQUIRED)

CHECK_INCLUDE_FILE_CXX(unistd.h HAVE_UNISTD)
CHECK_INCLUDE_FILE_CXX(termios.h HAVE_TERMIOS)

if (HAVE_UNISTD)
    add_compile_definitions(HAVE_UNISTD)
endif ()

if (HAVE_TERMIOS)
    add_compile_definitions(HAVE_TERMIOS)
endif ()

include_directories(${SDL2_INCLUDE_DIRS})

add_library(c8opcodes
        src/opcodes/sys.cpp
        src/opcodes/jp.cpp
        src/opcodes/call.hpp
        src/opcodes/call.cpp
        src/opcodes/skip.cpp
        src/opcodes/ld.cpp
        src/opcodes/rnd.cpp
        src/opcodes/drw.cpp src/opcodes/vreg.cpp)

add_library(c8support
        src/interpreter.cpp
        src/state.cpp
        src/opcode.cpp
        src/core.cpp
        src/keyboard.cpp
        src/io/text_io_handler.cpp
        src/io/sdl_io_handler.cpp
        src/io/debug_io_handler.cpp
        src/io/sdldebug_io_handler.cpp)

add_executable(chip8 src/main.cpp)
add_executable(dch8 src/dch8.cpp)

target_link_libraries(chip8
        PRIVATE
        c8support
        c8opcodes
        Threads::Threads
        Boost::program_options
        SDL2::SDL2)
target_link_libraries(dch8 PRIVATE c8support c8opcodes Boost::program_options)

enable_warnings(c8support)
enable_warnings(c8opcodes)
enable_warnings(chip8)
enable_warnings(dch8)

if (WITH_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()
