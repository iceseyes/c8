#ifndef CHIP8_OPCODE_HPP
#define CHIP8_OPCODE_HPP

#include <string>

#include "defines.hpp"

namespace chip8 {

class State;

class Opcode {
public:
    enum {
        SYS_OPCODE = 0x0000,   //!< Calls machine code routine at NNN
                               //!< 00E0 => display clear
                               //!> 00EE => Return from subroutine
        JP_OPCODE = 0x1000,    //!< Jumps to address NNN.
        CALL_OPCODE = 0x2000,  //!< Calls subroutine at NNN.
        SE_OPCODE = 0x3000,    //!< Skips the next instruction if VX equals NN.
        SNE_OPCODE = 0x4000,   //!< Skips the next instruction if VX doesn't equal NN.
        SER_OPCODE = 0x5000,   //!< Skips the next instruction if VX equal VY (5XY0).
        LD_OPCODE = 0x6000,    //!< Set VX = NN (6XNN).
        ADD_OPCODE = 0x7000,   //!< VX += NN, add NN to VX (7XNN) [Carry flag is not changed].
        VREG_OPCODE = 0x8000,  //!< V Register Operations:
                               //!< let 8XYO the opcode. Than:
                               //!< O == 0 <=> VX = VY (VY is assiged to VX),
                               //!< O == 1 <=> VX |= VY
                               //!< O == 2 <=> VX &= VY
                               //!< O == 3 <=> VX ^= VY
                               //!< O == 4 <=> VX += VY [with carry flag]
                               //!< O == 5 <=> VX -= VY [with carry flag]
                               //!< O == 6 <=> VX >>= 1 [Least significant bit of VX is stored in VF]
                               //!< O == 7 <=> VX = VY - VX [with carry flag]
                               //!< O == E <=> VX <<= 1 [Most significant bit of VX is stored in VF]
        SNER_OPCODE = 0x9000,  //!< Skips the next instruction if VX is not equal to VY (9XY0).
        LDI_OPCODE = 0xA000,   //!< Sets Index Register to the address NNN.
        JPV0_OPCODE = 0xB000,  //!< Jump to address NNN + V0
        RND_OPCODE = 0xC000,   //!< VX = rand() AND NN [CXNN]
        DRW_OPCODE = 0xD000,   //!< Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N
                               //!< pixels. Each row of 8 pixels is read as bit-coded starting from memory location I; I
        //!< value doesn’t change after the execution of this instruction. As described above, VF
        //!< is set to 1 if any screen pixels are flipped from set to unset when the sprite is
        //!< drawn, and to 0 if that doesn’t happen [DXYN]
        SKP_OPCODE = 0xE000,  //!< let EXCC the opcode. Than:
                              //!< For EX9E Skips the next instruction if the key stored in VX is pressed.
                              //!< For EXA1 Skips the next instruction if the key stored in VX is not pressed.
        LDX_OPCODE =
            0xF000,  //!< For:
                     //!< FX07: Sets VX to the value of the delay timer.
                     //!< FX0A: A key press is awaited, and then stored in VX.
                     //!< FX15: Sets the delay timer to VX.
                     //!< FX18: Sets the sound timer to VX.
                     //!< FX1E: Adds VX to I. VF is not affected.
                     //!< FX29: Sets I to the location of the sprite for the character in VX. Characters 0-F
                     //!<       (in hexadecimal) are represented by a 4x5 font.
                     //!< FX33: Stores the binary-coded decimal representation of VX, with the most significant of three
                     //!<       digits at the address in I, the middle digit at I plus 1, and the least significant
                     //!<       digit at I plus 2. (In other words, take the decimal representation of VX, place the
                     //!<       hundreds digit in memory at location in I, the tens digit at location I+1, and the ones
                     //!<       digit at location I+2.)
                     //!< FX55: Stores V0 to VX (including VX) in memory starting at address I. The offset from I is
                     //!<       increased by 1 for each value written, but I itself is left unmodified.
                     //!< FX65: Fills V0 to VX (including VX) with values from memory starting at address I. The offset
                     //!<       from I is increased by 1 for each value written, but I itself is left unmodified.
    };

    explicit Opcode(const std::string family, word code);
    virtual ~Opcode() = default;

    virtual bool valid(word /* _data */) { return true; };
    virtual void apply(State &state, word _data) = 0;
    void operator()(State &state, word _data);

    virtual std::string toString(word _data);

    const std::string family;
    const word code;

protected:
    std::string _format(const std::string &nmemonic, word data) const;
};

Opcode &makeOpcode(word opcode);

}  // namespace chip8

#endif  // CHIP8_OPCODE_HPP
