#ifndef CHIP8_OPCODE_IMPL_HPP
#define CHIP8_OPCODE_IMPL_HPP

#include "opcode.hpp"

namespace chip8::opcode {

class JMP : public Opcode {
public:
    JMP() : Opcode("jmp", JMP_OPCODE) {}
    void apply(State &state, word _data) override;
};

class CALL : public Opcode {
public:
    CALL() : Opcode("call", CALL_OPCODE) {}
    void apply(State &state, word _data) override;
};

class EQ : public Opcode {
public:
    EQ() : Opcode("eq", EQ_OPCODE) {}
    void apply(State &state, word _data) override;
};

class MVI : public Opcode {
public:
    MVI() : Opcode("mvi", MVI_OPCODE) {}
    void apply(State &state, word _data) override;
};

}  // namespace chip8::opcode

#endif  // CHIP8_OPCODE_IMPL_HPP
