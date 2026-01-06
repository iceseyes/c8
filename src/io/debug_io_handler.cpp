#include "debug_io_handler.hpp"

#include <cstdio>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>

#include "../core.hpp"
#include "../opcode.hpp"
#include "../state.hpp"

using std::cout;
using std::dec;
using std::endl;
using std::hex;
using std::istream;
using std::ostream;
using std::setfill;
using std::setw;
using std::stringstream;
using std::thread;
using std::to_string;
using std::vector;

using namespace std::chrono_literals;

namespace chip8 {

DebugIOHandler::DebugIOHandler() = default;

DebugIOHandler::~DebugIOHandler() noexcept = default;

void DebugIOHandler::log(const std::string &msg) { cout << msg << endl; }

void DebugIOHandler::log(word pc, const Core::DecodedOperation &op) {
    cout << "0x" << setw(4) << setfill('0') << hex << pc << dec << " " << op.first.toString(op.second) << endl;
}

void DebugIOHandler::log(word pc) { cout << "Reading PC: 0x" << setw(4) << setfill('0') << hex << pc << dec << endl; }

void DebugIOHandler::init(State &/* _state */) {}

void DebugIOHandler::draw(const State &_state) { cout << to_string(_state) << endl; }

vector<byte> DebugIOHandler::load() { return load(std::cin); }

vector<byte> DebugIOHandler::load(istream &provider) {
    vector<byte> program;
    while (provider) {
        char b;
        provider.get(b);
        program.push_back(b);
    }

    return program;
}

}  // namespace chip8