#include "ld.hpp"

#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

#include "../opcode_impl.hpp"

using std::dec;
using std::hex;
using std::setfill;
using std::setw;
using std::string;
using std::stringstream;
using std::vector;

namespace chip8::opcode {

void LD::apply(State &state, word _data) {
    byte r1 = getReg(_data, 0);
    state.v(r1) = getByte(_data);
}

string LD::toString(word _data) {
    stringstream ss;
    byte reg = getReg(_data, 0);
    byte val = getByte(_data);
    ss << NMEMONIC << " v" << hex << int(reg) << ", 0x" << setfill('0') << setw(2) << word(val) << dec << setfill(' ');
    return ss.str();
}

void LDI::apply(State &state, word _data) { state.indexRegister(getWord(_data)); }

string LDI::toString(word _data) {
    stringstream ss;
    word val = getWord(_data);
    ss << NMEMONIC << " i"
       << ", 0x" << hex << setfill('0') << setw(4) << val << dec << setfill(' ');
    return ss.str();
}

void ADD::apply(State &state, word _data) {
    byte reg = getReg(_data, 0);
    byte val = getByte(_data);
    state.v(reg) += val;
}

string ADD::toString(word _data) {
    stringstream ss;
    byte reg = getReg(_data, 0);
    byte val = getByte(_data);
    ss << NMEMONIC << " v" << hex << int(reg) << ", 0x" << setfill('0') << setw(2) << word(val) << dec << setfill(' ');
    return ss.str();
}

void LDX::apply(State &state, word _data) {
    byte reg = getReg(_data, 0);
    byte f = getByte(_data);

    if (f == GET_DT_OPCODE) {
        state.v(reg) = state.delayTimer();
    } else if (f == GET_KEY_OPCODE) {
        state.readKey(reg);
    } else if (f == SET_DT_OPCODE) {
        state.delayTimer(state.v(reg));
    } else if (f == SET_ST_OPCODE) {
        state.soundTimer(state.v(reg));
    } else if (f == ADDX_OPCODE) {
        state.indexRegister(state.indexRegister() + state.v(reg));
    } else if (f == FONT_OPCODE) {
        state.indexRegister(state.sprite(state.v(reg)));
    } else if (f == BCD_OPCODE) {
        state.storeBCD(state.v(reg));
    } else if (f == STORE_OPCODE) {
        vector<byte> data;
        byte i = 0;
        for (; i <= reg; ++i) data.push_back(state.v(i));
        state.write(data, state.indexRegister());
        state.indexRegister(state.indexRegister() + i);
    } else if (f == READ_OPCODE) {
        vector<byte> data = state.read(state.indexRegister(), reg + 1);
        byte i = 0;
        for (; i <= reg; ++i) state.v(i) = data[i];
        state.indexRegister(state.indexRegister() + i);
    }
}

string LDX::toString(word _data) {
    stringstream ss;
    byte reg = getReg(_data, 0);
    byte f = getByte(_data);

    if (f == GET_DT_OPCODE) {
        ss << NMEMONIC << " v" << hex << int(reg) << ", dt" << dec;
    } else if (f == GET_KEY_OPCODE) {
        ss << NMEMONIC << " v" << hex << int(reg) << ", k" << dec;
    } else if (f == SET_DT_OPCODE) {
        ss << NMEMONIC << " dt"
           << ", v" << hex << int(reg) << dec;
    } else if (f == SET_ST_OPCODE) {
        ss << NMEMONIC << " st"
           << ", v" << hex << int(reg) << dec;
    } else if (f == ADDX_OPCODE) {
        ss << ADD_NMEMONIC << " i"
           << ", v" << hex << int(reg) << dec;
    } else if (f == FONT_OPCODE) {
        ss << NMEMONIC << " f"
           << ", v" << hex << int(reg) << dec;
    } else if (f == BCD_OPCODE) {
        ss << NMEMONIC << " b"
           << ", v" << hex << int(reg) << dec;
    } else if (f == STORE_OPCODE) {
        ss << NMEMONIC << " [i]"
           << ", v" << hex << int(reg) << dec;
    } else if (f == READ_OPCODE) {
        ss << NMEMONIC << " v" << hex << int(reg) << ", [i]" << dec;
    } else {
        ss << NMEMONIC << hex << " DATA: 0x" << setw(4) << _data << dec << " UNKNOWN";
    }

    return ss.str();
}

bool LDX::valid(word _data) {
    byte f = getByte(_data);
    switch (f) {
    case GET_DT_OPCODE:
    case GET_KEY_OPCODE:
    case SET_DT_OPCODE:
    case SET_ST_OPCODE:
    case ADDX_OPCODE:
    case FONT_OPCODE:
    case BCD_OPCODE:
    case STORE_OPCODE:
    case READ_OPCODE:
        return true;
    }

    return false;
}

}  // namespace chip8::opcode