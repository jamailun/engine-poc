#include <engine/resources/resources_manager.hh>
#include <engine/resources/image.hh>
#include <engine/resources/tile_set.hh>
#include <engine/resources/tile_map.hh>
#include <engine/utils/fs_utils.hh>
#include <engine/sdl/game_window.hh>

#include <filesystem>
#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

using namespace engine;
using namespace resources;

#define CLEANUP_MAP(MAP_VAR) for(auto& entry : MAP_VAR) { delete entry.second; } MAP_VAR.clear();

resources_manager::~resources_manager() {
    std::cout << "Destroying resources_manager." << std::endl;

    //Freeing data
    // CLEANUP_MAP(_images);
    // CLEANUP_MAP(_tile_sets);
    // CLEANUP_MAP(_tile_maps);

    std::cout << "resources_manager destroyed." << std::endl;
}

// 

bool resources_manager::load_path_from_disk(std::string path) {
    if( ! engine::sdl::get_window().initialized()) {
        spdlog::critical("Cannot load files without a renderer to be able to compute textures. Initialize the SDL window first.");
        return false;
    }

    spdlog::debug("Loading path '{}'.", path);
    if(!fs::is_directory(path)) {
        spdlog::critical("Path '" + path + "' is not a directory.");
        return false;
    }

    // Assets JSON
    fs::directory_entry assets = get_child_file(path, "_assets.json");
    if(!assets.exists()) {
        spdlog::critical("Assets file '{}' is missing.", assets.path().string());
        return false;
    }

    std::ifstream f( assets.path().string() );
    nlohmann::json all_data = nlohmann::json::parse(f);
    nlohmann::json assets_array = all_data["assets"];

    // Read JSON
    // This intermediary read allow us to sort the assets by data-type.
    std::map<resource_type, std::vector<std::string>> loaded_resources {};
    for (auto& entry : assets_array.items()) {
        resource_type data_type = parse_DataType(entry.value());
        std::string id = entry.key();
        if(data_type != resource_type::none) {
            loaded_resources[data_type].push_back(id);
        }
    }

    // List all
    int count = 0;
    for(auto& rsrc : loaded_resources) {
        count += rsrc.second.size();
        for(auto& id : rsrc.second) {
            std::string file_path = get_child_path(path, id);
            switch(rsrc.first) {
                case resource_type::sprite:
                    spdlog::trace("Loading sprite '{}'.", file_path);
                    load_resource<Image>(_images, id, [file_path](){ return load_image_from_disk(file_path); });
                    break;

                case resource_type::tile_set:
                    spdlog::trace("Loading tile-set '{}'.", file_path);
                    load_resource<TileSet>(_tile_sets, id, [file_path](){ return load_tileset_from_disk(file_path); });
                    break;

                case resource_type::tile_map:
                    spdlog::trace("Loading tile-map '{}'.", file_path);
                    load_resource<TileMap>(_tile_maps, id, [file_path](){ return load_tilemap_from_disk(file_path); });
                    break;

                case resource_type::font:
                    spdlog::trace("Found font '{}'.", file_path);
                    _font_files[id] = file_path;
                    break;

                default:
                    spdlog::warn("Unhandled data-type.");
                    break;
            };
        }
    }

    spdlog::info("Resources Manager loaded {} assets successfully.", count);
    return true;
}

#define RM__FIND_DATA(T, method, map) \
resource_ref<T> resources_manager::method(std::string& id) const { \
    auto ref = map.find(id); \
    if(ref == map.end()) \
        return nullptr; \
    return (ref->second.get()); \
}

RM__FIND_DATA(Image, find_image, _images);
RM__FIND_DATA(TileSet, find_tile_set, _tile_sets);
RM__FIND_DATA(TileMap, find_tile_map, _tile_maps);

resource_ref<TTF_Font> resources_manager::load_font(std::string font_name, int font_size) const {
    // Check it exists
    auto lookup = _font_files.find(font_name);
    if(lookup == _font_files.end()) {
        spdlog::warn("Unknown font: '{}'.", font_name);
        std::stringstream ss;
        bool f = true;
        for(const auto& en : _font_files) {
            if(f) f = false; else ss << ", ";
            ss << en.first;
        }
        spdlog::warn("Existing fonts: [{}].", ss.str());
        return nullptr;
    }
    spdlog::info("Will load font at {}.", lookup->second.c_str());
    return TTF_OpenFont(lookup->second.c_str(), font_size);
}

// static instance

resources_manager& engine::resources::get_manager() {
    static resources_manager MANAGER;
    return MANAGER;
}
