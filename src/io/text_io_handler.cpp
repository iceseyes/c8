#include "text_io_handler.hpp"

#ifdef HAVE_TERMIOS
#include <termios.h>
#endif

#ifdef HAVE_UNISTD
#include <unistd.h>
#endif

#include <cstdio>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>
#include <sstream>

#include "../core.hpp"
#include "../opcode.hpp"
#include "../state.hpp"

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

#ifdef HAVE_TERMIOS
static struct termios oldt, newt;
#endif

TextIOHandler::TextIOHandler(ostream &_out, istream &_in) : _out(_out), _in(_in) {
#ifdef HAVE_TERMIOS
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ICANON;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
#endif
}

TextIOHandler::~TextIOHandler() noexcept {
#ifdef HAVE_TERMIOS
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
}

void TextIOHandler::log(const std::string &msg) { _out << msg << endl; }

void TextIOHandler::log(word pc, const Core::DecodedOperation &op) {
    _out << "\x1B[1;1H"
         << "\x1B[2K";
    _out << "Read PC: 0x" << setw(4) << setfill('0') << hex << pc << " opcode: " << op.first.family << " 0x" << setw(4)
         << op.second << dec << setfill(' ') << endl;
}

void TextIOHandler::init(State &_state) {
    _out << "\x1B[2J"
         << "\x1B[1;1H";
    _running = true;
}

void TextIOHandler::handleEvents(State &_state) {
    while (_running) {
        char ch = getchar();
        if (ch == 'q' || ch == 'Q')
            _running = false;
        else if (ch == 'r' || ch == 'R') {
            _state.reset();
        } else if (ch == 'p' || ch == 'P') {
            if (!_state.stopped()) _state.stop();
        } else if (ch == 'n' || ch == 'N') {
            if (_state.stopped() && !_state.nextInstructionEnabled()) _state.nextInstruction();
        } else if (ch == 'c' || ch == 'C') {
            if (_state.stopped()) _state.resume();
        } else if (ch != _lastCh) {
            setChar(_state, _lastCh, false);
            setChar(_state, ch, true);
            _lastCh = ch;
        }
    }
    exit(0);
}

void TextIOHandler::draw(const State &_state) {
    stringstream ss;
    auto &v = _state.video();
    for (int i = 0; i < CHIP8_ROWS; ++i) {
        for (int j = 0; j < CHIP8_COLS; ++j) {
            ss << (v[i * CHIP8_COLS + j] ? "#" : " ");
        }
        ss << endl;
    }

    _out << ss.str() << endl;

    if (!_running) {
        _out << "\x1B[2J"
             << "\x1B[1;1H" << endl;
#ifdef HAVE_TERMIOS
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
        _inThread->join();
        exit(0);
    }
}

vector<byte> TextIOHandler::load() { return load(_in); }

vector<byte> TextIOHandler::load(istream &provider) {
    vector<byte> program;
    while (provider) {
        char b;
        provider.get(b);
        program.push_back(b);
    }

    return program;
}

void TextIOHandler::setChar(State &_state, char ch, bool pressed) {
    if (ch >= 0x30 && ch <= 0x39)
        _state.keyPressed(ch - 0x30, pressed);
    else if (tolower(ch) >= 0x61 && tolower(ch) <= 0x66)
        _state.keyPressed(tolower(ch) - 0x61 + 0x0a, pressed);
}

}  // namespace chip8