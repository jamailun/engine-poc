
#pragma once

#include <cassert>
#include <cstdint>
#include <memory>
#include <typeindex>
#include <unordered_map>

#include "engine/component.hh"



namespace engine::detail {

class homogeneous_component_registry {

public:
    virtual void remove(uint64_t idx) = 0;

    virtual void update_all() = 0;
    virtual void action_all() = 0;

    virtual ~homogeneous_component_registry() = default;

};


template <component Component>
class homogeneous_component_registry_impl: public homogeneous_component_registry {

public:

    uint64_t add(std::shared_ptr<Component> cmpt) {
        if(cmpt == nullptr) {
            throw std::runtime_error("Cannot register null component");
        }
        last_idx++;
        _components.insert({last_idx, cmpt});
        return last_idx;
    }

    void remove(uint64_t idx) override {
        auto it = _components.find(idx);
        if(it != _components.end()) {
            _components.erase(it);
        }
    }

    void update_all() override {
        if constexpr(updatable<Component>) {
            for(auto& e: _components) {
                e.second->update();
            }
        }
    }

    void action_all() override {
        if constexpr(actionable<Component>) {
            for(auto& e: _components) {
                e.second->action();
            }
        }
    }

    virtual ~homogeneous_component_registry_impl() = default;

private:
    static uint64_t last_idx;
    std::unordered_map<uint64_t, std::shared_ptr<Component>> _components;

};

template <component Component>
uint64_t homogeneous_component_registry_impl<Component>::last_idx = 0;

} // namespace engine::detail


namespace engine {

class component_index {

public:
    template <component Component>
    static component_index make_idx(uint64_t registry_idx) {
        return component_index(std::type_index(typeid(Component)), registry_idx);
    }

    const std::type_index& type_idx() const { return _type_idx; };
    uint64_t registry_idx() const { return _registry_idx; }

private:
    component_index(std::type_index tidx, uint64_t ridx): _type_idx(tidx), _registry_idx(ridx) {}

    const std::type_index _type_idx;
    const uint64_t _registry_idx;

};


class component_registry {

public:
    template <component Component>
    component_index add(std::shared_ptr<Component> cmpt) {
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
        }
        const uint64_t registry_idx = sub_registry->add(cmpt);
        return component_index::make_idx<Component>(registry_idx);
    }

    void remove(const component_index& idx) {
        auto it = _sub_registries.find(idx.type_idx());
        if(it == _sub_registries.end()) {
            return;
        }
        it->second->remove(idx.registry_idx());
    }

    void update_all() {
        for(auto& e: _sub_registries) {
            e.second->update_all();
        }
    }

    void action_all() {
        for(auto& e: _sub_registries) {
            e.second->action_all();
        }
    }

private:
    std::unordered_map<std::type_index, std::shared_ptr<detail::homogeneous_component_registry>> _sub_registries;

};

} // namespace engine
