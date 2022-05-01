#ifndef CHIP8_STATE_HPP
#define CHIP8_STATE_HPP

#include <array>
#include <vector>
#include <memory>
#include <tuple>
#include <string>

#include "defines.hpp"
#include "keyboard.hpp"

namespace chip8 {

class Opcode;

class State {
    struct _Pimpl;
    std::unique_ptr<_Pimpl> _impl;

public:
    using VideoMemory = std::array<byte, CHIP8_COLS * CHIP8_ROWS>;
    using KeyPad = std::array<bool, KEYPAD_SIZE>;

    static constexpr word CODE_ADDRESS{0x200};
    static constexpr word OPCODE_BYTES{2};

    State();
    virtual ~State();

    void reset();

    void load(const std::vector<byte> &program, word address = CODE_ADDRESS);
    void load(const byte *program, std::size_t size, word address = CODE_ADDRESS);

    /** the system will fetch one opcode from the memory at the location specified by the program counter (pc).
     * In our Chip 8 emulator, data is stored in an array in which each address contains one byte. As one opcode is 2
     * bytes long, we will need to fetch two successive bytes and merge them to get the actual opcode. */
    virtual word fetch();

    virtual void write(std::vector<byte> data, word address);
    virtual std::vector<byte> read(word address, word size) const;

    virtual VideoMemory &video() const;
    virtual void video(const std::vector<byte> &spriteMap, word address);
    virtual bool videoChanged() const;

    virtual void readKey(byte reg);
    virtual bool keyPressed(byte key) const;
    virtual void keyPressed(byte key, bool pressed);
    virtual void noKeyPressed();

    virtual byte &v(byte index);
    virtual byte &v(byte index) const;

    virtual word indexRegister() const;
    virtual void indexRegister(word v);

    virtual bool stopped() const;
    virtual bool nextInstructionEnabled() const;
    virtual void stop();
    virtual void resume();
    virtual void nextInstruction();

    virtual word pc() const;
    virtual void pc(word _pc);

    virtual void push(word datum);
    virtual word pop();

    virtual byte rand() const;

    virtual void clrscr();

    virtual byte &delayTimer() const;
    virtual void delayTimer(byte tmr);

    virtual byte &soundTimer() const;
    virtual void soundTimer(byte tmr);

    virtual std::size_t sprite(byte index) const;
    virtual void storeBCD(byte value);

    virtual void tick();

    friend class StateTest;
};

std::string to_string(const State &state);

}  // namespace chip8

#endif  // CHIP8_STATE_HPP
