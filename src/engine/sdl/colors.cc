#include <engine/sdl/colors.hh>

using namespace engine;

Color colors::semi_random(int id) {
    switch (id) {
        case 0:
            return colors::red;
        case 1:
            return colors::blue;
        case 2:
            return colors::green;
        case 3:
            return colors::orange;
        case 4:
            return colors::purple;
        case 5:
            return colors::pink;
        case 6:
            return colors::light_blue;
        case 7:
            return colors::yellow;
        default:
            return colors::white;
    }
}
