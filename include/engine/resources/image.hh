#pragma once

#include "engine/utils/memory.hh"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_rect.h>
#include <memory>
#include <string>

namespace engine {
namespace resources {

/**
 * A wrapper for a SDL_Texture.
 */
class Image {
    friend std::unique_ptr<Image> load_image_from_disk(std::string file);
private:
    deleted_unique_ptr<SDL_Texture> _texture;
    int _width, _height;
    std::string _name;

public:
    Image(deleted_unique_ptr<SDL_Texture> texture, int width, int height, std::string name)
        : _texture(std::move(texture)), _width(width), _height(height), _name(name) {}
    ~Image();
    Image(Image&& mE) = default;
    Image& operator=(Image&& mE) = default;

    inline SDL_FRect create_rect(float x, float y) {
        return SDL_FRect { x, y, (float) _width, (float) _height };
    }

    SDL_Texture* sdl_texture() {
        return _texture.get();
    }
    
    const std::string& name() const { return _name; }
    int width() const { return _width; }
    int height() const { return _height; }
};

/**
 * Load an image from the disk.
 * @param file : OS path to the image.
 */
std::unique_ptr<Image> load_image_from_disk(std::string file);

} // namespace resources
} // namespace engine
