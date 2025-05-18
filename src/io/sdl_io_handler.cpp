#include "sdl_io_handler.hpp"

#include <string>
#include <thread>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <SDL2/SDL.h>

#include "../core.hpp"
#include "../opcode.hpp"
#include "../state.hpp"

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

SDLIOHandler::SDLIOHandler(std::istream &_in, size_t magnify)
    : _in(_in), magnify(magnify), screenWidth{CHIP8_COLS * magnify}, screenHeight{CHIP8_ROWS * magnify} {}

SDLIOHandler::~SDLIOHandler() noexcept {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void SDLIOHandler::log(const std::string &msg) {}

void SDLIOHandler::log(word pc, const Core::DecodedOperation &op) {
    cout << "Read PC: 0x" << setw(4) << setfill('0') << hex << pc << " opcode: " << op.first.family << " 0x" << setw(4)
         << op.second << dec << setfill(' ') << endl;
}

void SDLIOHandler::init(State &_state) {
    _running = true;
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth + 2 * magnify,
                              screenHeight, SDL_WINDOW_ALWAYS_ON_TOP);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void SDLIOHandler::handleEvents(State &_state) {
    SDL_Event e;
    while (_running) {
        while (SDL_PollEvent(&e) != 0) {
            handleEvent(_state, &e);
        }
    }
    exit(0);
}

void SDLIOHandler::draw(const State &_state) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);

    auto &v = _state.video();
    for (int i = 0; i < CHIP8_ROWS; ++i) {
        for (int j = 0; j < CHIP8_COLS; ++j) {
            SDL_Rect fillRect = {int(j * screenWidth / CHIP8_COLS), int(i * screenHeight / CHIP8_ROWS), magnify,
                                 magnify};
            if (v[i * CHIP8_COLS + j]) SDL_RenderFillRect(renderer, &fillRect);
        }
    }
    SDL_RenderPresent(renderer);
}

vector<byte> SDLIOHandler::load() { return load(_in); }

vector<byte> SDLIOHandler::load(istream &provider) {
    vector<byte> program;
    while (provider) {
        char b;
        provider.get(b);
        program.push_back(b);
    }

    return program;
}

void SDLIOHandler::handleEvent(State &_state, SDL_Event *e) {
    bool pressed = e->type == SDL_KEYDOWN;
    char ch = e->key.keysym.sym;

    if (pressed || e->type == SDL_KEYUP) {
        if (ch == 'q')
            _running = false;
        else if (ch == SDL_SCANCODE_F5)
            _state.reset();
        else if (ch == SDL_SCANCODE_F6) {
            if (!_state.stopped()) _state.stop();
        } else if (ch == SDL_SCANCODE_F7) {
            if (!pressed && _state.stopped() && !_state.nextInstructionEnabled()) _state.nextInstruction();
        } else if (ch == SDL_SCANCODE_F8) {
            if (_state.stopped()) _state.resume();
        } else if (ch >= 0x30 && ch <= 0x39)
            _state.keyPressed(ch - 0x30, pressed);
        else if (ch >= 0x61 && ch <= 0x66)
            _state.keyPressed(ch - 0x61 + 0x0a, pressed);
    } else if (e->type == SDL_QUIT) {
        _running = false;
    }
}

}  // namespace chip8