#include <gtest/gtest.h>

#include <array>
#include <memory>

#include "state.hpp"
#include "state_impl.hpp"

using std::array;
using std::copy;
using std::make_unique;
using std::unique_ptr;

namespace chip8 {

class StateTest : public testing::Test {
protected:
    void SetUp() override {
        state = make_unique<State>();
        state->pc(0);
        copy(program.begin(), program.end(), state->_impl->memory.begin());
    }

    void TearDown() override { state.reset(); }

    State::_Pimpl &impl() { return *state->_impl; }

    unique_ptr<State> state;
    array<byte, 16> program{
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    };
};

TEST_F(StateTest, fetch) {
    word opcode = state->fetch();
    ASSERT_EQ(0x0102, opcode);
    ASSERT_EQ(2, state->pc());
    opcode = state->fetch();
    ASSERT_EQ(0x0304, opcode);
    ASSERT_EQ(4, state->pc());
}

TEST_F(StateTest, push) {
    ASSERT_EQ(0, impl().sp);
    state->push(0x1234);
    ASSERT_EQ(1, impl().sp);
    ASSERT_EQ(0x1234, impl().stack[0]);
    state->push(0x4321);
    ASSERT_EQ(2, impl().sp);
    ASSERT_EQ(0x1234, impl().stack[0]);
    ASSERT_EQ(0x4321, impl().stack[1]);
}

}  // namespace chip8
