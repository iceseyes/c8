#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include "interpreter.hpp"
#include "io/debug_io_handler.hpp"
#include "io/text_io_handler.hpp"
#include "io/sdl_io_handler.hpp"
#include "io/sdldebug_io_handler.hpp"

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::make_unique;
using std::size_t;
using std::string;
using std::unique_ptr;

using namespace chip8;

namespace po = boost::program_options;

namespace {

unique_ptr<IOHandler> ioHandler;
string filename;
size_t clockHZ;

void parseArgs(int argc, char *argv[]);
void run();

}  // namespace

int main(int argc, char *argv[]) {
    parseArgs(argc, argv);
    run();
    return 0;
}

namespace {

void run() {
    Interpreter i{std::move(ioHandler), filename, clockHZ};

    i.init();
    i.load();
    i.run();
}

void parseArgs(int argc, char *argv[]) {
    // Declare the supported options.
    po::options_description desc("chip8 emulator");
    po::positional_options_description pd;
    po::variables_map vm;

    // clang-format off
    desc.add_options()
        ("help", "produce help message")
        ("render,r", po::value<string>()->default_value("sdl"),
            "IO render type:\n"
            "\tsdl   : graphic [DEFAULT]\n"
            "\ttext  : text (needs a 64x32 terminal or more)\n"
            "\tdebug : deassembled output only\n"
            "\tdsdl  : deassembled output with SDL graphics")
        ("file", po::value<string>()->required(), "file to decode")
        ("hz", po::value<size_t>()->default_value(Interpreter::CLOCK_HZ), "emulated CPU Herz [default: 200]")
        ("magnify,m", po::value<size_t>()->default_value(20), "magnification factor [default: 20]");
    // clang-format on

    pd.add("file", 1);
    try {
        auto parsed = po::command_line_parser(argc, argv).options(desc).positional(pd).run();

        po::store(parsed, vm);
        po::notify(vm);
    } catch (po::required_option &e) {
        cout << desc << "\n";
        exit(1);
    }

    if (vm.count("help")) {
        cout << desc << "\n";
        exit(1);
    }

    filename = vm["file"].as<string>();
    clockHZ = vm["hz"].as<size_t>();
    auto render = vm["render"].as<string>();
    if (render == "text")
        ioHandler = make_unique<TextIOHandler>(cout, cin);
    else if (render == "debug")
        ioHandler = make_unique<DebugIOHandler>();
    else if (render == "sdl")
        ioHandler = make_unique<SDLIOHandler>(cin, vm["magnify"].as<size_t>());
    else if (render == "dsdl")
        ioHandler = make_unique<SDLDebugIOHandler>(cin, vm["magnify"].as<size_t>());
    else {
        cerr << "Invalid render type" << endl;
        exit(1);
    }
}

}  // namespace