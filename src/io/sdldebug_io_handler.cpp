#include "sdldebug_io_handler.hpp"

#include <string>
#include <thread>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <SDL2/SDL.h>

#include "../core.hpp"
#include "../opcode.hpp"
#include "../state.hpp"

#include "sdl_io_handler.hpp"

using std::cout;
using std::dec;
using std::endl;
using std::hex;
using std::istream;
using std::setfill;
using std::setw;
using std::stringstream;
using std::thread;
using std::vector;

namespace chip8 {

SDLDebugIOHandler::SDLDebugIOHandler(std::istream &_in, size_t magnify) : SDLIOHandler(_in, magnify) {}

SDLDebugIOHandler::~SDLDebugIOHandler() {}

void SDLDebugIOHandler::log(const std::string &msg) { cout << msg << endl; };

void SDLDebugIOHandler::log(word pc, const Core::DecodedOperation &op) {
    auto opcode = op.first.toString(op.second);
    if (opcode.empty()) opcode = op.first.family;
    cout << "0x" << setw(4) << setfill('0') << hex << pc << dec << " " << opcode << endl;
};

void SDLDebugIOHandler::init(State &_state) { SDLIOHandler::init(_state); }
void SDLDebugIOHandler::draw(const State &_state) {
    cout << to_string(_state) << endl;
    SDLIOHandler::draw(_state);
}

}  // namespace chip8