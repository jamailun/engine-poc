#pragma once

#include <engine/sdl/configuration.hh>

namespace game {
    
// Configuration of the program instance.
struct argument_read_output {
    std::string assets_folder;
    engine::sdl::configuration_builder config;
};

/// Parse program arguments.
/// @return program configuration.
argument_read_output read_arguments(int argc, char** argv);

}
