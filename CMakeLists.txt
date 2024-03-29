cmake_minimum_required(VERSION 3.0)
project(chip8 CXX)

include(CheckIncludeFileCXX)

option(WITH_TEST "Enable test module" ON)

set(CMAKE_CXX_STANDARD 20)

find_package(Threads)
find_package(Boost REQUIRED COMPONENTS program_options)
find_package(SDL2 REQUIRED)

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

target_link_libraries(chip8 PRIVATE c8support c8opcodes Threads::Threads Boost::program_options ${SDL2_LIBRARIES})
target_link_libraries(dch8 PRIVATE c8support c8opcodes Boost::program_options)

if (WITH_TEST)
    add_subdirectory(test)
endif ()
