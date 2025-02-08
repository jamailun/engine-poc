#include <engine/sdl/configuration.hh>

using namespace engine::sdl;

const int DEFAULT_SCREEN_WIDTH = 1080;
const int DEFAULT_SCREEN_HEIGHT = 720;
const std::string DEFAULT_SCREEN_TITLE = "POC - engine";

configuration_builder::configuration_builder() {
    screen_width = DEFAULT_SCREEN_WIDTH;
    screen_height = DEFAULT_SCREEN_HEIGHT;
    screen_title = DEFAULT_SCREEN_TITLE;
}

configuration configuration_builder::build() {
    return configuration(*this);
}

configuration::configuration(configuration_builder builder) {
    screen_width = builder.screen_width;
    screen_height = builder.screen_height;
    screen_title = builder.screen_title;
}

configuration::configuration() : configuration(configuration_builder()) {}
