#pragma once

#include "engine/math.hh"
#include "engine/utils/memory.hh"
#include "engine/guaranteed_ptr.hh"

#include <cmath>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

#ifndef _SPATIAL_GRID_DEBUG
#define _SPATIAL_GRID_DEBUG 1
#endif
#ifdef _SPATIAL_GRID_DEBUG
#include "engine/sdl/drawer.hh"
#endif

#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

namespace engine {

/**
 * Uniform space distribution.
 * This time, we have a dynamic amount of nodes.
 */
template<typename K, typename T>
class dynamic_spatial_grid {
    using KeyGetter = std::function<K(const T&)>;
    using PositionGetter = std::function<math::Point(const T&)>;
private:
    struct Node {
        int64_t x, y; // signed
        math::Rect bounds;
        //std::vector<Node*> neighbors;
        std::unordered_map<K, T> elements;

        void remove(const K key) {
            auto it = elements.find(key);
            if(it != elements.end())
                elements.erase(it);
        }

#ifdef _SPATIAL_GRID_DEBUG
        void debug_draw() const {
            math::Rect draw_bounds = bounds;
            if(draw_bounds.x>0)draw_bounds.x -= 1; else draw_bounds.x += 1;
            if(draw_bounds.y>0)draw_bounds.y -= 1; else draw_bounds.y += 1;
            draw_bounds.w -= 2;
            draw_bounds.h -= 2;
            engine::drawer::draw_rect(draw_bounds, elements.empty() ? engine::colors::yellow : engine::colors::red);
        }
#endif
    };

    inline int64_t world_dim_to_index(float value) const {
        return std::floor(value / _node_size);
    }

    void update_node_positions(Node* node) {
        for(auto iter = node->elements.begin(); iter != node->elements.end(); ) {
            math::Point pos = _position_getter(iter->second);
            if(node->bounds.contains(pos)) {
                ++iter;
                continue;
            }
            // move to another location
            Node* new_node = try_locate(pos);
            if(new_node == nullptr) {
                new_node = create_new_node(pos);
            }
            new_node->elements[iter->first] = iter->second;
            _reversed_nodes[iter->first] = new_node;
            iter = node->elements.erase(iter);
        }
    }

    // Data and getters
    float _node_size;
    KeyGetter _key_getter;
    PositionGetter _position_getter;

    // Data structures
    std::map<int64_t, std::map<int64_t, Node*>> _nodes; // position -> Node, for ADD and READ access
    std::vector<Node*> _all_nodes; // All created nodes, for total-clear
    std::unordered_map<K, Node*> _reversed_nodes; // key -> node, for DELETE access
    bool _nodes_may_have_changed = false;

    /**
     * Get the node at a specific index.
     * @return nullptr if not found.
     */
    Node* at(int64_t x, int64_t y) {
        auto p = _nodes[x].find(y);
        return p == _nodes[x].end() ? nullptr : p->second;
    }

    bool has_node(int64_t x, int64_t y) {
        return at(x, y) != nullptr;
    }

    Node* try_locate(const math::Point position) {
        int64_t x = world_dim_to_index(position.x);
        int64_t y = world_dim_to_index(position.y);
        // spdlog::debug("({},{}) -> [{}][{}].", position.x, position.y, x, y);
        return at(x, y);
    }

    Node* create_new_node(const math::Point position) {
        int64_t x = world_dim_to_index(position.x);
        int64_t y = world_dim_to_index(position.y);
        spdlog::debug("Creating node at {}/{}...", x, y);
        if(has_node(x, y)) {
            spdlog::error("Call to #create_new_node, but node already exists! ({}, {})", x, y);
            return at(x, y);
        }
        Node* node = new Node();
        node->x = x;
        node->y = y;
        node->bounds = math::Rect(x * _node_size, y * _node_size, _node_size, _node_size);
        _nodes[x][y] = node;
        _all_nodes.push_back(node);
        spdlog::debug("Ok, created node.");
        _nodes_may_have_changed = true;
        return node;
    }

public:
    dynamic_spatial_grid(const KeyGetter& key_getter, const PositionGetter& position_getter, float node_size)
        : _node_size(node_size), _key_getter(key_getter), _position_getter(position_getter)
    {}

    ~dynamic_spatial_grid() {
        // Avoid memory leaks
        for(Node* n : _all_nodes) {
            delete n;
        }
    }

    // 0(1) insertion.
    void add(T value) {
        math::Point elem_pos = _position_getter(value);
        Node* target_node = try_locate(elem_pos);
        if(target_node == nullptr) {
            target_node = create_new_node(elem_pos);
        }
        K key = _key_getter(value);
        target_node->elements[key] = value;
        _reversed_nodes[key] = target_node;
    }

    // O(1) removal
    void remove(T value) {
        K key = _key_getter(value);
        auto it = _reversed_nodes.find(key);
        if(it != _reversed_nodes.end()) {
            Node* node = it->second;
            node->remove(key);
            _reversed_nodes.erase(it);
        }
    }

    // Update all positions of all elements.
    void update_positions() {
        _nodes_may_have_changed = false;
        for(Node* node : _all_nodes) {
            update_node_positions(node);
            if(_nodes_may_have_changed) return;
        }
    }

    /*
     * Query the grid to find all elements contained in a rectangle query.
    */
    std::vector<T> query(math::Rect query) {
        std::vector<T> output;
        // on itère dans les Nodes contenant la query.
        int64_t min_index_x = world_dim_to_index(query.x);
        int64_t min_index_y = world_dim_to_index(query.y);
        int64_t max_index_x = world_dim_to_index(query.x + query.w);
        int64_t max_index_y = world_dim_to_index(query.y + query.h);
        for(int64_t x = min_index_x; x <= max_index_x; x++) {
            for(int64_t y = min_index_y; y <= max_index_y; y++) {
                Node* node = at(x,y);
                if(node != nullptr) {
                    for(auto& en : node->elements) {
                        // TODO optimisation : si la Node est complètement dans le query, alors on
                        // peut directement mettre tout le contenu.
                        if(query.contains(_position_getter(en.second))) {
                            output.push_back(en.second);
                        }
                    }
                }
            }
        }
        return output;
    }


#ifdef _SPATIAL_GRID_DEBUG
    void debug_draw() const {
        for(Node* node : _all_nodes) {
            node->debug_draw();
        }
    }
#else
    void debug_draw() const {/* Rien. */}
#endif
};

} // namespace engine
