#ifndef CHIP8_STATE_IMPL_HPP
#define CHIP8_STATE_IMPL_HPP

#include <array>

#include "defines.hpp"
#include "state.hpp"
#include "keyboard.hpp"

namespace chip8 {

struct State::_Pimpl {
    std::array<byte, MEMORY_SIZE> memory;  // device memory (reserved + ram + rom)
    State::VideoMemory vMemory;            // video memory
    std::array<byte, V_REG_NUM> v;         // general purpose registers
    std::array<word, STACK_SIZE> stack;    // function call stack
    State::KeyPad key;                     // keypad event mapper

    word i;             // index register
    word pc;            // program counter
    word sp;            // stack pointer
    byte delayTimer;    // timer register
    byte soundTimer;    // sound timer register, when comes to 0, buzzer ring
    bool videoChanged;  // when a video memory update occurs, this flags recorded the event
    byte keyPressed;    // last key pressed;
};

}  // namespace chip8

#endif  // CHIP8_STATE_IMPL_HPP
