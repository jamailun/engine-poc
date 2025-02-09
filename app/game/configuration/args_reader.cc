#include "./args_reader.hh"

#include <argparse/argparse.hpp>
#include <spdlog/spdlog.h>

using namespace game;

void handle_log_level(const std::string& level) {
    auto spdlog_level = spdlog::level::from_str(level);
    if(spdlog_level == spdlog::level::off) {
        spdlog::error("Invalid log level '{}'.", level);
        throw std::invalid_argument("Invalid log level.");
    }
    spdlog::set_level(spdlog_level);
}

argument_read_output game::read_arguments(int argc, char** argv) {
    argparse::ArgumentParser program("engine-poc");

    std::string log_level;
    argument_read_output output;
    
    // Global
    program.add_argument("-L", "--level")
        .help("Log level")
        .default_value("trace")
        .store_into(log_level);

    // Resources
    program.add_argument("-a", "--assets")
        .help("Path to the assets directory")
        .store_into(output.assets_folder);

    // Window configuration
    program.add_argument("-w", "--width")
        .help("Screen width")
        .default_value(1080)
        .store_into(output.config.screen_width);
    program.add_argument("-h", "--height")
        .help("Screen height")
        .default_value(920)
        .store_into(output.config.screen_height);

    // process
    program.parse_args(argc, argv);

    // read log level
    handle_log_level(log_level);

    // output
    return output;
}
