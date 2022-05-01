#include "vreg.hpp"

#include <string>
#include <sstream>
#include <iomanip>

#include "../opcode_impl.hpp"

using std::dec;
using std::hex;
using std::setfill;
using std::setw;
using std::string;
using std::stringstream;

namespace chip8::opcode {

const string VREG::NMEMONIC[] = {"ld", "or", "and", "xor", "add", "sub", "shr", "subn", "shl"};

bool VREG::valid(word _data) {
    byte h = getNibble(_data);
    return (h < 8) || (h == 0xE);
}

void VREG::apply(State &state, word _data) {
    byte r1 = getReg(_data, 0);
    byte r2 = getReg(_data, 1);
    byte h = getNibble(_data);
    word ris = 0;

    switch (h) {
    case 0:
        state.v(r1) = state.v(r2);
        break;
    case 1:
        state.v(r1) |= state.v(r2);
        break;
    case 2:
        state.v(r1) &= state.v(r2);
        break;
    case 3:
        state.v(r1) ^= state.v(r2);
        break;
    case 4:
        ris = state.v(r1) + state.v(r2);
        state.v(0xf) = (ris & 0xff00) ? 1 : 0;
        state.v(r1) = 0x00ff & ris;
        break;
    case 5:
        state.v(0xf) = (state.v(r1) > state.v(r2)) ? 1 : 0;
        state.v(r1) -= state.v(r2);
        break;
    case 6:
        state.v(0xf) = state.v(r1) & 0x01;
        state.v(r1) >>= 1;
        break;
    case 7:
        state.v(0xf) = (state.v(r1) < state.v(r2)) ? 1 : 0;
        state.v(r1) = state.v(r2) - state.v(r1);
        break;
    case 0xE:
        state.v(0xf) = (state.v(r1) & 0x80) >> 7;
        state.v(r1) <<= 1;
        break;

    default:
        break;
    }
}

string VREG::toString(word _data) {
    stringstream ss;
    byte r1 = getReg(_data, 0);
    byte r2 = getReg(_data, 1);
    byte h = getNibble(_data);
    auto nmemonic = (h < 8) ? NMEMONIC[h] : NMEMONIC[8];

    ss << nmemonic << " v" << hex << int(r1) << ", v" << int(r2);
    return ss.str();
}

}  // namespace chip8::opcode