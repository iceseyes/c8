# A chip8 emulator

This is a simple emulator for chip8 chips: https://en.wikipedia.org/wiki/CHIP-8

In particular, it was implemented using this
document http://www.cs.columbia.edu/~sedwards/classes/2016/4840-spring/designs/Chip8.pdf.

## Build

For building the project you need gcc/clang compiler and cmake build tool installed. You also
need [program_options](https://www.boost.org/doc/libs/1_75_0/doc/html/program_options.html)
by libboost and [SDL2](https://www.libsdl.org/download-2.0.php).

Then, you have to run cmake, for example in this way:

    mkdir -p build/Debug
    cd build/Debug
    cmake -DCMAKE_BUILD_TYPE=Debug ../../
    cmake --build . -- -j4

### vcpkg and modern cmake presets (recommended)

If you are using [vcpkg](https://vcpkg.io/en/), cmake can install dependencies automatically for you.

For that, you have to set the environment variable `VCPKG_ROOT` pointing to your vcpkg installation folder.

You can run cmake from the project root folder like this:

    cmake --presets dev
    cmake --build --preset dev -- -j4

This will install dependencies and build the project in `build/debug` folder.
Therefore, all the binaries will be created in `build/debug` folder, i.e.,
use `build/debug/chip8` to run the emulator.

To run tests, you can use:

    ctest --preset dev

Also, you can use `--preset release` to build a release version of the project.

## Use

You can find some rom to try, here: https://github.com/dmatlack/chip8/tree/master/roms

You have two programs available:

* dch8 is a dissambler for chip8
* chip8 is an emulator for chip8

### dch8

run something like that

    ./dch8 pong.ch8 

and you will get

```
0x200	ld va, 0x02		## 0x6a02
0x202	ld vb, 0x0c		## 0x6b0c
0x204	ld vc, 0x3f		## 0x6c3f
0x206	ld vd, 0x0c		## 0x6d0c
0x208	ld i, 0x02ea		## 0xa2ea
0x20a	drw va, vb, 0x6		## 0xdab6
0x20c	drw vc, vd, 0x6		## 0xdcd6
...
0x2f6	sys 0x0000		## 0x0000
```

where:

* first column is the memory address of instruction
* second column is mnemonic form of instruction
* third column is a comment with numeric form of instruction

### chip8

*chip8* is a command to run a chip8 program. For example:

    ./chip8 pong.ch8

open an SDL window running pong program. Use 'q' or 'Q' to quit, and 1 to 9 or 'a' to 'f' keys as input.

You can use different renderers with flag `-r`:

* sdl: default renderer. Creates an SDL window where every point is a square with side equals to magnify value.
* text: render graphics using ascii symbols in a terminal 64x32 chars
* debug: show instructions and registers at every step
* dsdl: show instructions and registers at every step, with default rendering