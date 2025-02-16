#pragma once

#include "engine/math.hh"
#include "engine/utils/memory.hh"
#include "engine/guaranteed_ptr.hh"

#include <iostream>
#include <unordered_map>
#include <vector>

#ifndef _SPATIAL_GRID_DEBUG
#define _SPATIAL_GRID_DEBUG 1
#endif
#ifdef _SPATIAL_GRID_DEBUG
#include "engine/sdl/drawer.hh"
#endif

namespace engine {

/**
 * Uniform space distribution.
 */
template<typename K, typename T>
class spatial_grid {
    using KeyGetter = std::function<K(const T&)>;
    using BoundsGetter = std::function<math::Rect(const T&)>;
private:
    struct Node {
        size_t col, row;
        math::Rect bounds;
        std::vector<Node*> neighbors;
        std::unordered_map<K, T> elements;

#ifdef _SPATIAL_GRID_DEBUG
        void debug_draw() const {
            math::Rect draw_bounds = bounds;
            draw_bounds.w -= 1;
            draw_bounds.h -= 1;
            engine::drawer::draw_rect(draw_bounds, elements.empty() ? engine::colors::yellow : engine::colors::red);
        }
#endif
    };

    void update_node_positions(Node& node) {
        for(auto iter = node.elements.begin(); iter != node.elements.end(); ) {
            math::Rect pos = _bounds_getter(iter->second);
            if(node.bounds.intersects(pos)) {
                ++iter;
                continue;
            }
            // move to another location
            auto new_node = locate(pos);
            if(new_node.is_valid()) {
                new_node->elements[iter->first] = iter->second;
            } else {
                // element is now OOB !
                //TODO handle oob
            }
            iter = node.elements.erase(iter);
        }
    }

    // Nodes array
    math::Rect _bounds;
    size_t _size_cols, _size_rows;
    float _cell_width, _cell_height;
    std::unique_ptr<Node[]> _nodes;
    KeyGetter _key_getter;
    BoundsGetter _bounds_getter;

    Node& at(size_t column, size_t row) {
        return _nodes[column + _size_rows * row];
    }

    guaranteed_ptr<Node> locate(const math::Rect& bounds) {
        int local_x = (int) (bounds.x - _bounds.x);
        int local_y = (int) (bounds.y - _bounds.y);
        int col = local_x / _cell_width;
        int row = local_y / _cell_height;
        if(col < 0 || col >= (int)_size_cols || row < 0 || row >= (int)_size_rows)
            return nullptr;
        return &at((size_t)col, (size_t)row);
    }

    void build() {
        _cell_width = _bounds.w / _size_cols;
        _cell_height = _bounds.h / _size_rows;
        _nodes = std::unique_ptr<Node[]>(new Node[_size_cols * _size_rows]);
        for(size_t col = 0; col < _size_cols; ++col) {
            for(size_t row = 0; row < _size_rows; ++row) {
                auto& node = at(col, row);
                node.col = col;
                node.row = row;
                node.bounds = math::Rect(col * _cell_width + _bounds.x, row * _cell_height + _bounds.y, _cell_width, _cell_height);

                // Neighbors in line
                if(col > 0)
                    node.neighbors.push_back(&at(col-1, row));
                if(row > 0)
                    node.neighbors.push_back(&at(col, row-1));
                if(col < _size_cols - 1)
                    node.neighbors.push_back(&at(col+1, row));
                if(row < _size_rows - 1)
                    node.neighbors.push_back(&at(col, row+1));

                // Neighbors in diagonal
                if(col > 0 && row > 0)
                    node.neighbors.push_back(&at(col-1, row-1));
                if(col > 0 && row < _size_rows - 1)
                    node.neighbors.push_back(&at(col-1, row+1));
                if(col < _size_cols && row > 0)
                    node.neighbors.push_back(&at(col-1, row-1));
                if(col < _size_cols && row < _size_rows - 1)
                    node.neighbors.push_back(&at(col+1, row+1));
            }
        }
    }

    void foreach(consumer<Node&>& consumer) {
        for(size_t i = 0; i < _size_cols * _size_rows; ++i) {
            consumer(_nodes[i]);
        }
    }

public:
    spatial_grid(math::Rect bounds, const KeyGetter& key_getter, const BoundsGetter& bounds_getter, size_t size_cols, size_t size_rows)
        : _bounds(bounds), _size_cols(size_cols), _size_rows(size_rows), _key_getter(key_getter), _bounds_getter(bounds_getter)
    {
        build();
    }
    ~spatial_grid() = default;

    // 0(1) insertion.
    void add(T value) {
        K key = _key_getter(value);
        math::Rect elem_bounds = _bounds_getter(value);
        auto target_node = locate(elem_bounds);
        if(!target_node.is_valid()) {
            //TODO out of bounds
            std::cout << "out of bounds :(" << std::endl;
            return;
        }
        target_node->elements[key] = value;
    }

    // O(1) removal
    void remove(T value) {
        K key = _key_getter(value);
        math::Rect elem_bounds = _bounds_getter(value);
        auto target_node = locate(elem_bounds);
        if(!target_node.is_valid()) {
            //TODO out of bounds
            std::cout << "out of bounds :(" << std::endl;
            return;
        }
        target_node->elements.erase(key);
    }

    void update_positions() {
        for(size_t i = 0; i < _size_cols * _size_rows; i++) {
            update_node_positions(_nodes[i]);
        }
    }

    std::vector<T> query(math::Rect query) {
        std::vector<T> output;
        // to local
        math::Rect local_query = math::Rect(
            query.x - _bounds.x, query.y - _bounds.y,
            query.w, query.h
        );
        // on itère dans les Nodes contenant la query.
        size_t min_index_col = (size_t) (local_query.x / _cell_width);
        size_t min_index_row = (size_t) (local_query.y / _cell_height);
        size_t max_index_col = (size_t) ((local_query.x + local_query.w) / _cell_width);
        size_t max_index_row = (size_t) ((local_query.y + local_query.h) / _cell_height);
        if(max_index_col >= _size_cols) max_index_col = _size_cols - 1;
        if(max_index_row >= _size_rows) max_index_row = _size_rows - 1;
        for(size_t col = min_index_col; col <= max_index_col; col++) {
            for(size_t row = min_index_row; row <= max_index_row; row++) {
                for(auto& en : at(col, row).elements) {
                    if(query.intersects(_bounds_getter(en.second))) {
                        output.push_back(en.second);
                    }
                }
            }
        }
        return output;
    }

#ifdef _SPATIAL_GRID_DEBUG
    void debug_draw() const {
        engine::drawer::draw_rect(_bounds, engine::colors::pink);
        for(size_t i = 0; i < _size_cols * _size_rows; i++) {
            _nodes[i].debug_draw();
        }
        // foreach([](Node& node) {
        //     node.debug_draw();
        // });
    }
#endif
};

} // namespace engine
