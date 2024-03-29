#ifndef CHIP8_OPCODE_IMPL_HPP
#define CHIP8_OPCODE_IMPL_HPP

#include "opcode.hpp"

namespace chip8::opcode {

byte getReg(word data, byte r);
byte getNibble(word data);
byte getByte(word data);
word getWord(word data);

}  // namespace chip8::opcode

#endif  // CHIP8_OPCODE_IMPL_HPP
