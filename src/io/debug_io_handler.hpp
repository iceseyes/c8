#ifndef CHIP8_DEBUG_IO_HANDLER_HPP
#define CHIP8_DEBUG_IO_HANDLER_HPP

#include <iostream>
#include <string>
#include <thread>

#include "../core.hpp"
#include "io_handler.hpp"

namespace chip8 {

class DebugIOHandler : public IOHandler {
public:
    DebugIOHandler();
    ~DebugIOHandler();

    void log(const std::string &msg) override;
    void log(word pc, const Core::DecodedOperation &op) override;
    void log(word pc) override;

    void init(State &_state) override;
    void handleEvents(State &_state) override;
    void draw(const State &_state) override;

    std::vector<byte> load() override;
    std::vector<byte> load(std::istream &provider) override;
};

}  // namespace chip8

#endif  // CHIP8_DEBUG_IO_HANDLER_HPP
