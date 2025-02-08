#include <engine/resources/resources_manager.hh>
#include <engine/resources/tile_set.hh>
#include <engine/sdl/game_window.hh>

#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>
#include "nlohmann/json.hpp"

#include <SDL.h>

using namespace engine;
using namespace resources;

TileSet::TileSet(std::string name) : _name(name) {}

TileSet::~TileSet() {
    std::cout << "Destroying TileSet " << _name << "." << std::endl;
}

void TileSet::initialize(nlohmann::json& data) {
    if(is_valid()) {
        spdlog::warn("Should not initialize a TileSet twice !");
        return;
    }

    // Read JSON
    std::string texture_name = data["texture"];
    _cell_width  = data["cell_size"]["width"];
    _cell_height = data["cell_size"]["height"];

    // Get image
    _image_ref = get_manager().find_image(texture_name);
    if(!_image_ref.is_valid()) {
        spdlog::error("Could not find asset file/{} !", texture_name);
        return;
    }

    // check image is properly sized
    if(_image_ref->width() % _cell_width != 0) {
        spdlog::warn("Invalid image size ({}). Width of {}, cell-size of {}.", _image_ref->name(), _image_ref->width(), _cell_width);
    }
    if(_image_ref->height() % _cell_height != 0) {
        spdlog::warn("Invalid image size ({}). Height of {}, cell-size of {}.", _image_ref->name(), _image_ref->height(), _cell_height);
    }

    // Count cells
    _count_cols = _image_ref->width() / _cell_width;
    _count_lines = _image_ref->height() / _cell_height;

    // Init array
    _matrix_rects = std::make_unique<SDL_Rect[]>(_count_cols * _count_lines);

    // Fill the matrixes
    for (int idx_line = 0; idx_line < _count_lines; idx_line++) {
        for (int idx_col = 0; idx_col < _count_cols; idx_col++) {
            SDL_Rect& rect = _matrix_rects[idx_col + _count_cols * idx_line];
            rect.x = idx_col * _cell_width;
            rect.y = idx_line * _cell_height;
            rect.w = _cell_width;
            rect.h = _cell_height;
        }
    }

    spdlog::debug("Loaded tileset image. Count=({}/{}).", _count_cols, _count_lines);
}

SDL_Rect& TileSet::get(int id) {
    return _matrix_rects[id];
}

std::unique_ptr<TileSet> engine::resources::load_tileset_from_disk(std::string file) {
    std::ifstream ifs( file );
    nlohmann::json json_data = nlohmann::json::parse(ifs);
    
    auto ts = std::make_unique<TileSet>(file);
    ts->initialize(json_data);

    return ts;
}

