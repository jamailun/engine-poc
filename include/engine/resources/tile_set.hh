#pragma once

#include <string>
#include <optional>
#include <memory>

#include "engine/resources/image.hh"
#include "engine/guaranteed_ptr.hh"
#include "nlohmann/json_fwd.hpp"

class SDL_Rect;

namespace engine {
namespace resources {

/**
 * Describe the meta-data of a tileset.
 */
class TileSet {
    friend std::unique_ptr<TileSet> load_tileset_from_disk(std::string file);
private:
    std::string _name;
    guaranteed_ptr<Image> _image_ref;

    int _cell_width, _cell_height;        // parameter
    uint16_t _count_cols, _count_lines;   // computed
    
    std::unique_ptr<SDL_Rect[]> _matrix_rects;
    
public:
    TileSet(std::string name);
    ~TileSet();
    
    TileSet(TileSet&& mE) = default;
    TileSet& operator=(TileSet&& mE) = default;

    inline bool is_valid() const { return _image_ref.is_valid(); }
    void initialize(nlohmann::json& data);

    inline std::string& get_name() { return _name; }
    inline std::string get_name() const { return _name; }
    inline int get_cell_width() const { return _cell_width; }
    inline int get_cell_height() const { return _cell_height; }

    SDL_Rect& get(int id);

    inline guaranteed_ptr<Image> image() { return _image_ref; }

};

std::unique_ptr<TileSet> load_tileset_from_disk(std::string file);

} // namespace resources
} // namespace engine
