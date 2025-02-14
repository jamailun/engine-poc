#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <iostream>
#include <string>
#include <sstream>
#include "engine/math.hh"
#include "engine/utils/memory.hh"
#include "engine/sdl/drawer.hh"

namespace engine {

/**
 * A quad-tree, of elements having position.
 */
template<typename T>
class quad_tree {
    using PositionSupplier = std::function<math::Point(const T&)>;

private:
    PositionSupplier _position_supplier;
    math::Rect _bounds;
    size_t _max_elements;
    size_t _max_depth;
    size_t _depth;
    guaranteed_ptr<quad_tree<T>> _parent;

    std::vector<T> _elements;
    std::vector<std::unique_ptr<quad_tree<T>>> _children;

    void add_child(float x, float y, float w, float h) {
        _children.push_back(std::make_unique<quad_tree<T>>(_position_supplier, math::Rect(x, y, w, h), _max_elements, _max_depth, this));
        _children.back()->_depth = _depth + 1;
    }

    // Split into subtrees.
    void split() {
        if(is_split() || _depth >= _max_depth) return;

        float halfWidth = _bounds.w / 2.0f;
        float halfHeight = _bounds.h / 2.0f;

        add_child(_bounds.x, _bounds.y, halfWidth, halfHeight);
        add_child(_bounds.x + halfWidth, _bounds.y, halfWidth, halfHeight);
        add_child(_bounds.x, _bounds.y + halfHeight, halfWidth, halfHeight);
        add_child(_bounds.x + halfWidth, _bounds.y + halfHeight, halfWidth, halfHeight);

        // move elements to children
        for(auto iter = _elements.begin(); iter < _elements.end(); ) {
            insert(*iter);
            iter = _elements.erase(iter);
        }
    }

    void merge() {
        if(!is_split()) return;

        for(auto& child : _children) {
            for(auto& element : child->_elements) {
                _elements.push_back(element);
            }
            child->_elements.clear();
        }
        _children.clear();
    }

    std::string to_string(int index) const {
        std::stringstream ss;
        ss << std::string(index, ' ');
        ss << "-[(" << _bounds.x << ", " << _bounds.y << "; " << _bounds.w << ", " << _bounds.h << ")]";
        if(is_split()) {
            ss << ":" << std::endl;
            for(auto& qt : _children) {
                ss << qt->to_string(index + 2);
            }
        } else {
            ss << " {" << _elements.size() << "}" << std::endl;
        }
        return ss.str();
    }

    void try_merge() {
        if(size() <= _max_elements) {
            merge();
        }
    }

public:
    quad_tree(PositionSupplier position_supplier, math::Rect bounds, size_t max_elements, size_t max_depth, guaranteed_ptr<quad_tree<T>> parent) :
        _position_supplier(position_supplier), _bounds(bounds), _max_elements(max_elements), _max_depth(max_depth), _depth(0), _parent(parent)
    {}
    quad_tree(PositionSupplier position_supplier, math::Rect bounds, size_t max_elements, size_t max_depth) : quad_tree(position_supplier, bounds, max_elements, max_depth, nullptr) {}
    quad_tree(PositionSupplier position_supplier, math::Rect bounds, size_t max_elements) : quad_tree(position_supplier, bounds, max_elements, 4) {}
    quad_tree(PositionSupplier position_supplier, math::Rect bounds) : quad_tree(position_supplier, bounds, 12) {}

    /**
     * Insert an object. If the object is outside the bounds, it is not inserted.
     */
    void insert(const T& object) {
        math::Point point = _position_supplier(object);
        std::cout << "INSERT:: element at (" << point.x << "," << point.y << ")." << std::endl;
        if(is_split()) {
            for(auto& child : _children) {
                if(child->bounds().contains(point)) {
                    child->insert(object);
                    return;
                }
            }
        }
        // Not split
        if(_bounds.contains(point)) {
            _elements.push_back(object);
            if(_elements.size() > _max_elements) {
                split();
            }
            return;
        }
        // to parent ?
        if(_parent.is_valid())
            _parent->insert(object);
    }

    bool remove(const T& object) {
        math::Point point = _position_supplier(object);
        if(is_split()) {
            for(auto& child : _children) {
                if(child->remove(object)) {
                    try_merge();
                    return true;
                }
            }
            return false;
        }
        // Not split
        auto iter = std::find(_elements.begin(), _elements.end(), object);
        if(iter != _elements.end()) {
            std::cout << "removing element at (" << point.x << "," << point.y << ")..." << std::endl;
            _elements.erase(iter);
            std::cout << "rdone" << std::endl;
            return true;
        }
        return false;
    }

    /**
     * Recursive size of the quadtree.
     */
    size_t size() const {
        if(is_split()) {
            size_t s = 0;
            for(auto& child : _children) {
                s += child->size();
            }
            return s;
        }
        return _elements.size();
    }

    /**
     * Test if the tree is split.
     */
    inline bool is_split() const {
        return !_children.empty();
    }

    /**
     * Get the bounds of this quad_tree node.
     */
    inline math::Rect bounds() const {
        return _bounds;
    }

    /**
     * Use to debug : draw the quadtree splits.
     */
    void debug_draw() const {
        drawer::draw_rect(_bounds, colors::semi_random(_depth));
        if(is_split()) {
            for(auto& child : _children) {
                child->debug_draw();
            }
        }
    }

    /**
     * String visualisation.
     */
    std::string to_string() const {
        return to_string(0);
    }

    /**
     * Call this when positions have changed.
     */
    void update_positions() {
        if(is_split()) {
            for(auto& child : _children) {
                child->update_positions();
            }
        }
        for(auto iter = _elements.begin(); iter < _elements.end(); ) {
            math::Point point = _position_supplier(*iter);
            if(_bounds.contains(point)) {
                ++iter;
            } else {
                std::cout << "removing element at (" << point.x << "," << point.y << ")." << std::endl;
                if(_parent.is_valid())
                    _parent->insert(*iter);
                iter = _elements.erase(iter);
                std::cout << "rem.done" << std::endl;
            }
        }
        try_merge();
    }

    /**
     * Find elements belongging to the given rectangle.
     */
    std::vector<T> query(const math::Rect& rect) const {
        std::vector<T> result;
        if(is_split()) {
            for(auto& child : _children) {
                auto res = child->query(rect);
                result.insert(result.end(), res.begin(), res.end());
            }
        } else if(_bounds.intersects(rect)) {
            for(auto& element : _elements) {
                if(rect.contains(_position_supplier(element))) {
                    result.push_back(element);
                }
            }
        }
        return result;
    }

    void foreach(std::function<void(const T&)> func) const {
        if(is_split()) {
            for(auto& child : _children) {
                child->foreach(func);
            }
        } else {
            for(const auto& element : _elements) {
                func(element);
            }
        }
    }

};

} //namespace engine
