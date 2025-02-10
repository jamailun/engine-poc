#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <iostream>

#include "engine/component.hh"

namespace engine::detail {

template<component Component>
int z_index_of_component() {
    return 0;
}

class homogeneous_component_registry {
public:
    virtual void remove(void* addr) = 0;
    virtual void clear() = 0;
    virtual size_t size() const = 0;

    virtual void update_all(float elapsed) = 0;
    virtual void render_all() = 0;
    virtual int static_z_index() = 0;

    virtual ~homogeneous_component_registry() = default;
};

// Homogeneous registries are sorted with the Z-index of the class.
bool sort_homogeneous_component_registries(std::shared_ptr<homogeneous_component_registry> a, std::shared_ptr<homogeneous_component_registry> b);

template <component Component>
class homogeneous_component_registry_impl: public homogeneous_component_registry {
private:
    std::vector<std::shared_ptr<Component>> _components;
public:
    virtual ~homogeneous_component_registry_impl() = default;

    void add(std::shared_ptr<Component> cmpt) {
        if(cmpt == nullptr) {
            throw std::runtime_error("Cannot register null component");
        }
        const auto it = std::find(_components.begin(), _components.end(), cmpt);
        if(it == _components.end()) {
            _components.push_back(cmpt);
        }
    }

    void remove(void* addr) override {
        auto it = std::find_if(_components.begin(), _components.end(), [addr](const std::shared_ptr<Component>& cmpt) {
            return addr == static_cast<void*>(cmpt.get());
        });
        if(it != _components.end()) {
            _components.erase(it);
        }
    }

    void clear() override {
        _components.clear();
    }

    inline size_t size() const override {
        return _components.size();
    }

    void update_all(float elapsed) override {
        if constexpr(updatable<Component>) {
            std::for_each(_components.begin(), _components.end(), [elapsed](std::shared_ptr<Component>& cmpt) {
                cmpt->update(elapsed);
            });
        }
    }

    void render_all() override {
        if constexpr(renderable<Component>) {
            std::for_each(_components.begin(), _components.end(), [](std::shared_ptr<Component>& cmpt) {
                cmpt->render();
            });
        }
    }

    int static_z_index() override {
        return z_index_of_component<Component>();
    }
};

} // namespace engine::detail


namespace engine {

class component_registry {
private:
    std::unordered_map<std::type_index, std::shared_ptr<detail::homogeneous_component_registry>> _sub_registries;
    std::vector<std::shared_ptr<detail::homogeneous_component_registry>> _z_index_sorted_registries;
public:
    component_registry() = default;
    ~component_registry() {
        for(auto& reg : _sub_registries) {
            reg.second->clear();
        }
        _sub_registries.clear();
    }

    template <component Component>
    void add(std::shared_ptr<Component> cmpt) {
        using sub_registry_impl = detail::homogeneous_component_registry_impl<Component>;
        const std::type_index tidx = std::type_index(typeid(Component));
        std::shared_ptr<sub_registry_impl> sub_registry = nullptr;
        auto it = _sub_registries.find(tidx);
        if(it != _sub_registries.end()) {
            sub_registry = std::dynamic_pointer_cast<sub_registry_impl>(it->second);
            assert(sub_registry != nullptr);
        } else {
            sub_registry = std::make_shared<sub_registry_impl>();
            _sub_registries.insert({tidx, sub_registry});
            // add to z_index_sorted
            _z_index_sorted_registries.push_back(sub_registry);
            // SORT !
            std::sort(_z_index_sorted_registries.begin(), _z_index_sorted_registries.end(), engine::detail::sort_homogeneous_component_registries);
        }
        sub_registry->add(cmpt);
    }

    template <component Component>
    void remove(std::shared_ptr<Component> cmpt) {
        const std::type_index tidx = std::type_index(typeid(Component));
        auto it = _sub_registries.find(tidx);
        if(it == _sub_registries.end()) {
            return;
        }
        it->second->remove(static_cast<void*>(cmpt.get()));
    }

    void remove(std::type_index index, void* raw) {
        auto& reg = _sub_registries[index];
        reg->remove(raw);
    }

    void update_all(float elapsed) {
        for(auto& e: _sub_registries) {
            e.second->update_all(elapsed);
        }
    }

    void render_all() {
        for(auto& e: _z_index_sorted_registries) {
            e->render_all();
        }
    }

    size_t total_size() const {
        size_t total = 0;
        for(auto& reg : _sub_registries) {
            total += reg.second->size();
        }
        return total;
    }
};

component_registry& get_registry();

} // namespace engine
