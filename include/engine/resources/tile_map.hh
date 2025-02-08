#pragma once

#include <string>
#include <optional>
#include <memory>

#include "engine/resources/image.hh"
#include "engine/guaranteed_ptr.hh"
#include "nlohmann/json_fwd.hpp"

namespace engine {
namespace resources {
    
class TileSet;

/**
 * Describe the mapping of tiles to a TileSet.
 */
class TileMap {
    friend std::unique_ptr<TileMap> load_tilemap_from_disk(std::string file);
private:
    std::string _name;
    guaranteed_ptr<TileSet> _tileset_ref;
    uint16_t _size_width, _size_height;
    std::unique_ptr<uint16_t[]> _matrix_ids;
    
    void initialize(nlohmann::json& data);

public:
    TileMap(std::string name);
    ~TileMap();
    
    TileMap(TileMap&& mE) = default;
    TileMap& operator=(TileMap&& mE) = default;

    inline bool is_valid() const { return _tileset_ref.is_valid(); }

    inline std::string& get_name() { return _name; }
    inline std::string get_name() const { return _name; }

    void draw_call(int x, int y);
};

std::unique_ptr<TileMap> load_tilemap_from_disk(std::string file);

} // namespace resources
} // namespace engine
