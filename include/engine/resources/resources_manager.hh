#pragma once

#include <map>
#include <optional>
#include "engine/guaranteed_ptr.hh"
#include "nlohmann/json.hpp"
#include <SDL2/SDL_ttf.h>

namespace engine {
namespace resources {

using resource_id = std::string;
template<typename T>
using resource_ptr = std::unique_ptr<T>;
template<typename T>
using resources_map = std::map<resource_id, resource_ptr<T>>;
template<typename T>
using resource_ref = guaranteed_ptr<T>;

class configuration;
class Image;
class TileSet;
class TileMap;

/**
 * A data-type, used by the resources_manager.
 */
enum class resource_type : int {
    none = 0,

    // A texture to be displayed
    // Class: Image.
    sprite = 1,

    // Meta-data of a tile-set resource. Linked to an Image.
    tile_set = 10,
    // The data of a tile-map. Requires a tile-set.
    tile_map = 11,

    // A font resource
    font = 100,
};
inline resource_type parse_DataType(int id) {
    return static_cast<resource_type>(id);
}

/**
 * Manages the resources of the engine.
 */
class resources_manager {
private:
    resources_map<Image> _images;
    resources_map<TileSet> _tile_sets;
    resources_map<TileMap> _tile_maps;
    std::map<std::string, std::string> _font_files; // name -> path of fonts.

    template<typename T>
    void load_resource(resources_map<T>& map, std::string& id, std::function<resource_ptr<T>(void)> supplier) {
        resource_ptr<T> object = supplier();
        // resource_ptr<T> object = std::move(supplier());
        if(object != nullptr)
            map[id] = std::move(object);
    }

public:
    resources_manager() = default;
    ~resources_manager();

    bool load_path_from_disk(std::string path);

    resource_ref<Image> find_image(std::string& id) const;
    resource_ref<TileSet> find_tile_set(std::string& id) const;
    resource_ref<TileMap> find_tile_map(std::string& id) const;
    resource_ref<TTF_Font> load_font(std::string font_name, int font_size) const;
};

/*
 * Get the global instance of the Resources Manager.
 * Will try to initialize manager.
*/
resources_manager& get_manager();

} // resources manager
} // namespace engine
