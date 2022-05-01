#include "state.hpp"
#include "state_impl.hpp"

#include <algorithm>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

#include "opcode.hpp"
#include "keyboard.hpp"

using std::copy;
using std::dec;
using std::endl;
using std::hex;
using std::make_unique;
using std::pair;
using std::setfill;
using std::setw;
using std::string;
using std::stringstream;
using std::vector;

namespace chip8 {

State::State() : _impl{make_unique<_Pimpl>()} { srand(word(time(0))); }

State::~State() {}

void State::load(const std::vector<byte> &program, word address) {
    copy(program.begin(), program.end(), _impl->memory.begin() + address);
}

void State::load(const byte *program, std::size_t size, word address) {
    copy(program, program + size, _impl->memory.begin() + address);
}

void State::reset() {
    _impl->vMemory.fill(0);
    _impl->v.fill(0);
    _impl->stack.fill(0);
    _impl->key.fill(0);
    _impl->i = 0;
    _impl->pc = CODE_ADDRESS;
    _impl->sp = 0;
    _impl->delayTimer = 0;
    _impl->soundTimer = 0;
    _impl->videoChanged = true;
    _impl->keyPressed = false;
    srand(word(time(0)));
}

word State::fetch() {
    word opcode = (_impl->memory.at(_impl->pc) << 8) | _impl->memory.at(_impl->pc + 1);
    _impl->pc += OPCODE_BYTES;

    return opcode;
}

word State::indexRegister() const { return _impl->i; }
void State::indexRegister(word v) { _impl->i = v; }

word State::pc() const { return _impl->pc; }
void State::pc(word _pc) {
    if (_pc > MEMORY_SIZE)
        throw std::logic_error("Invalid PC: " + std::to_string(_pc) + "/" + std::to_string(MEMORY_SIZE));
    _impl->pc = _pc;
}

void State::push(word datum) {
    _impl->stack[_impl->sp] = datum;
    ++_impl->sp;
}

word State::pop() {
    --_impl->sp;
    return _impl->stack[_impl->sp];
}

byte State::rand() const { return (::rand() % 255); }

byte &State::v(byte index) { return _impl->v.at(index); }
byte &State::v(byte index) const { return _impl->v.at(index); }

void State::clrscr() {
    _impl->vMemory.fill(0);
    _impl->videoChanged = true;
}

State::VideoMemory &State::video() const { return _impl->vMemory; }

void State::video(const std::vector<byte> &spriteMap, word address) {
    word cursor{address};
    for (auto p : spriteMap)
        if (cursor < _impl->vMemory.size()) _impl->vMemory[cursor++] = p;
    _impl->videoChanged = true;
}

bool State::videoChanged() const { return _impl->videoChanged; }

void State::tick() {
    if (_impl->delayTimer > 0) --_impl->delayTimer;
}

void State::write(vector<byte> data, word address) { copy(data.begin(), data.end(), _impl->memory.begin() + address); }

vector<byte> State::read(word address, word size) const {
    return vector<byte>(_impl->memory.begin() + address, _impl->memory.begin() + address + size);
}

void State::readKey(byte reg) {
    if (_impl->keyPressed < 0xff) {
        v(reg) = _impl->keyPressed;
    } else {
        pc(pc() - 1);
    }
}

bool State::keyPressed(byte key) const { return _impl->key.at(key); }

void State::keyPressed(byte key, bool pressed) {
    _impl->key.at(key) = pressed;
    if (pressed) _impl->keyPressed = key;
}

void State::noKeyPressed() { _impl->keyPressed = 0xff; }

byte &State::delayTimer() const { return _impl->delayTimer; }
void State::delayTimer(byte tmr) { _impl->delayTimer = tmr; }

byte &State::soundTimer() const { return _impl->soundTimer; }
void State::soundTimer(byte tmr) { _impl->soundTimer = tmr; }

std::size_t State::sprite(byte index) const { return index * 5; }

void State::storeBCD(byte value) {
    byte h = value / 100;
    byte d = (value - h * 100) / 10;
    byte u = (value - d * 10 - h * 100);

    write({h, d, u}, indexRegister());
}

string to_string(const State &state) {
    stringstream ss;
    for (int i = 0; i < 0x10; ++i) {
        ss << "v" << setfill('0') << setw(1) << hex << i << dec << ": 0x" << setw(2) << hex << word(state.v(i)) << dec
           << setfill(' ') << " ";
        if (i == 0x07) ss << endl;
    }

    ss << endl;
    ss << "i: 0x" << setw(4) << setfill('0') << hex << state.indexRegister() << dec << setfill(' ') << endl;
    ss << "pc: 0x" << setw(4) << setfill('0') << hex << state.pc() << dec << setfill(' ') << endl;

    return ss.str();
}

}  // namespace chip8
