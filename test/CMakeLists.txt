find_package(GTest CONFIG REQUIRED)

include_directories(
        src
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_CURRENT_BINARY_DIR})

set(TEST_SRC
        src/main.cpp                  # entry point
        src/test_gtest.cpp            # test for gtest framework is working
        src/test_state.cpp
        src/test_opcodes.cpp)

add_executable(c8_tests
        ${TEST_SRC}
)

target_link_libraries(c8_tests
        PRIVATE
        c8support
        c8opcodes
        GTest::gtest
        GTest::gmock
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(c8_tests)
