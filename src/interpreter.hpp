#ifndef CHIP8_INTERPRETER_HPP
#define CHIP8_INTERPRETER_HPP

#include <iostream>
#include <chrono>

#include "core.hpp"
#include "io/io_handler.hpp"
#include "state.hpp"

namespace chip8 {

class Interpreter {
    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    State _state;
    Core _core;
    std::unique_ptr<IOHandler> _io;
    TimePoint _lastTick;
    std::string _programFile;
    std::size_t _clockHZ{CLOCK_HZ};

public:
    static constexpr std::size_t CLOCK_HZ{500};

    Interpreter(std::unique_ptr<IOHandler> &&_io, const std::string &_programFile = "", std::size_t clockHZ = CLOCK_HZ);

    void init();
    void load();
    void run();
    void runOne();
    void updateVideo();
    void updateKeyboard();

private:
    bool __started{false};

    bool checkTime();

    std::chrono::milliseconds clockRate() const;

    static constexpr std::chrono::milliseconds RATE(std::size_t hz) {
        return std::chrono::milliseconds(std::size_t((1.0 / hz) * 1000 + 0.5));
    };
};

}  // namespace chip8

#endif  // CHIP8_INTERPRETER_HPP
