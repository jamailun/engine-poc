#pragma once

#include <string>

namespace engine {
namespace sdl {

class configuration;

struct configuration_builder {
public:
    int screen_width;
    int screen_height;
    std::string screen_title;

    configuration_builder();

    configuration build();
};

class configuration {
private:
    int screen_width;
    int screen_height;
    std::string screen_title;
public:
    configuration(configuration_builder builder);
    configuration();

    ~configuration() = default;
    configuration(const configuration& other) = default;
    configuration(configuration&& other) = default;
    configuration& operator=(const configuration& other) {
        screen_width = other.screen_width;
        screen_height = other.screen_height;
        screen_title = other.screen_title;
        return *this;
    }

    inline int get_screen_width() const {
        return screen_width;
    }
    inline int get_screen_height() const {
        return screen_height;
    }
    inline std::string get_screen_title() const {
        return screen_title;
    }
};

}
}
