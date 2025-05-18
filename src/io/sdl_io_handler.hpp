#ifndef CHIP8_SDL_IO_HANDLER_HPP
#define CHIP8_SDL_IO_HANDLER_HPP

#include <string>
#include <iostream>
#include <thread>

#include "../core.hpp"
#include "io_handler.hpp"

struct SDL_Window;
struct SDL_Renderer;
union SDL_Event;

namespace chip8 {

class SDLIOHandler : public IOHandler {
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool _running;
    char _lastCh;
    std::istream &_in;
    const int magnify{20};
    const size_t screenWidth;
    const size_t screenHeight;

public:
    SDLIOHandler(std::istream &_in, size_t magnify = 20);
    ~SDLIOHandler();

    void log(const std::string &msg) override;
    void log(word pc, const Core::DecodedOperation &op) override;

    void init(State &_state) override;
    void handleEvents(State &_state) override;
    void draw(const State &_state) override;

    std::vector<byte> load() override;
    std::vector<byte> load(std::istream &provider) override;

private:
    void handleEvent(State &_state, SDL_Event *e);
};

}  // namespace chip8

#endif  // CHIP8_SDL_IO_HANDLER_HPP
