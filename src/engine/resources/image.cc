#include <engine/resources/image.hh>
#include <engine/sdl/game_window.hh>

#include <SDL2/SDL_image.h>
#include <spdlog/spdlog.h>

using namespace engine;
using namespace resources;

std::unique_ptr<Image> engine::resources::load_image_from_disk(std::string file) {
    SDL_Texture* texture_ptr = IMG_LoadTexture(engine::sdl::get_renderer(), file.c_str());
    if(!texture_ptr) {
        spdlog::error("Could not load texture from disk (file='{}')", file);
        spdlog::error("SDL_Error: {}", SDL_GetError());
        return nullptr;
    }

    // Metadata
    int width = 0;
    int height = 0;
    SDL_QueryTexture(texture_ptr, NULL, NULL, &width, &height);
    
    // Wrap the texture in a custom unique_ptr
    deleted_unique_ptr<SDL_Texture> texture = deleted_unique_ptr<SDL_Texture>(texture_ptr, [](SDL_Texture*ptr){SDL_DestroyTexture(ptr);});

    // create
    return std::make_unique<Image>(std::move(texture), width, height, file);
}

#include <iostream>
Image::~Image() {
    std::cout << "Destroying Image " << _name << ")" << std::endl;
//    SDL_DestroyTexture(_texture.get());
}
