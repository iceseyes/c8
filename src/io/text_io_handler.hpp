#ifndef CHIP8_TEXT_IO_HANDLER_HPP
#define CHIP8_TEXT_IO_HANDLER_HPP

#include <iostream>
#include <string>
#include <thread>

#include "../core.hpp"
#include "io_handler.hpp"

namespace chip8 {

class TextIOHandler : public IOHandler {
    std::ostream &_out;
    std::istream &_in;
    bool _running;
    char _lastCh;

public:
    TextIOHandler(std::ostream &_out, std::istream &_in);
    ~TextIOHandler() noexcept override;

    void log(const std::string &msg) override;
    void log(word pc, const Core::DecodedOperation &op) override;

    void init(State &_state) override;
    void handleEvents(State &_state) override;
    void draw(const State &_state) override;

    std::vector<byte> load() override;
    std::vector<byte> load(std::istream &provider) override;

private:
    void setChar(State &_state, char ch, bool pressed);
};

}  // namespace chip8

#endif  // CHIP8_TEXT_IO_HANDLER_HPP
