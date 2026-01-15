#ifndef CHIP8_IO_HANDLER_HPP
#define CHIP8_IO_HANDLER_HPP

#include <string>
#include <vector>

#include "../core.hpp"

namespace chip8 {

class IOHandler {
public:
    virtual ~IOHandler() {}

    virtual void log(const std::string &msg) = 0;
    virtual void log(word pc, const Core::DecodedOperation &op) = 0;
    virtual void log(word /* pc */){};

    virtual void init(State &_state) = 0;
    virtual void handleEvents(State &_state) = 0;
    virtual void draw(const State &_state) = 0;

    virtual std::vector<byte> load() = 0;
    virtual std::vector<byte> load(std::istream &provider) = 0;
};

}  // namespace chip8

#endif  // CHIP8_IO_HANDLER_HPP
