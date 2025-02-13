#pragma once

#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include "engine/math.hh"

namespace engine {

template <typename T>
concept position_holder = requires(T cmpt) {
    { cmpt.get_pos() } -> std::same_as<math::Point>;
};

/**
 * A quad-tree, of elements having position.
 */
template<position_holder T>
class quad_tree {
private:
    math::Rect _bounds;
    size_t _max_elements;
    size_t _max_depth;
    size_t _depth;

    std::vector<T> _elements;
    std::vector<std::unique_ptr<quad_tree<T>>> _children;

    // Split into subtrees.
    void split() {
        if(is_split() || _depth >= _max_depth) return;

        float halfWidth = _bounds.w / 2.0f;
        float halfHeight = _bounds.h / 2.0f;

        _children.push_back(std::make_unique<quad_tree<T>>(math::Rect(_bounds.x, _bounds.y, halfWidth, halfHeight), _max_elements));
        _children.push_back(std::make_unique<quad_tree<T>>(math::Rect(_bounds.x + halfWidth, _bounds.y, halfWidth, halfHeight), _max_elements));
        _children.push_back(std::make_unique<quad_tree<T>>(math::Rect(_bounds.x, _bounds.y + halfHeight, halfWidth, halfHeight), _max_elements));
        _children.push_back(std::make_unique<quad_tree<T>>(math::Rect(_bounds.x + halfWidth, _bounds.y + halfHeight, halfWidth, halfHeight), _max_elements));
        for(auto& child : _children) child->_depth = _depth + 1;


        // move elements to children
        for(auto iter = _elements.begin(); iter < _elements.end(); ) {
            insert(std::move(*iter));
            iter = _elements.erase(iter);
        }
    }

    void merge() {
        if(!is_split()) return;

        for(auto& child : _children) {
            for(auto& element : child->_elements) {
                _elements.push_back(std::move(element));
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

public:
    quad_tree(math::Rect bounds, size_t max_elements, size_t max_depth) : _bounds(bounds), _max_elements(max_elements), _max_depth(max_depth), _depth(0) {}
    quad_tree(math::Rect bounds, size_t max_elements) : quad_tree(bounds, max_elements, 4) {}
    quad_tree(math::Rect bounds) : quad_tree(bounds, 12) {}

    /**
     * Insert an object. If the object is outside the bounds, it is not inserted.
     */
    void insert(const T& object) {
        math::Point point = object.get_pos();
        if(is_split()) {
            for(auto& child : _children) {
                if(child->bounds().contains(point)) {
                    child->insert(std::move(object));
                    return;
                }
            }
            return;
        }
        // Not split
        if(_bounds.contains(point)) {
            _elements.push_back(std::move(object));
            if(_elements.size() > _max_elements) {
                split();
            }
        }
    }

    bool remove(const T& object) {
        math::Point point = object.get_pos();
        if(is_split()) {
            for(auto& child : _children) {
                if(child->bounds().contains(point)) {
                    bool removed = child->remove(std::move(object));
                    if(removed) {
                        if(size() < _max_elements) {
                            merge();
                        }
                    }
                    return true;
                }
            }
            return false;
        }
        // Not split
        if(_bounds.contains(point)) {
            auto iter = std::find(_elements.begin(), _elements.end(), object);
            if(iter != _elements.end()) {
                _elements.erase(iter);
                return true;
            }
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

    inline math::Rect bounds() const {
        return _bounds;
    }

    /**
     * Use to debug : draw the quadtree splits.
     */
    void debug_draw() const {
        drawer::draw_rect(_bounds, colors::red);
    }

    std::string to_string() const {
        return to_string(0);
    }

};

} //namespace engine
