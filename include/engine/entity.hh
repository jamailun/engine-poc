#pragma once

#include <iostream>
#include <string>
#include "engine/registry.hh"
#include "engine/component.hh"
#include "engine/guaranteed_ptr.hh"

namespace engine {
namespace detail {

class homogeneous_component_slot {
protected:
    virtual ~homogeneous_component_slot() = default;
public:
    virtual void remove() = 0;
    virtual void* get_raw() = 0;
};

template <component Component>
class homogeneous_component_slot_impl : public homogeneous_component_slot {
private:
    std::shared_ptr<Component> _component;
public:
    homogeneous_component_slot_impl() = default;
    ~homogeneous_component_slot_impl() = default;

    void set(std::shared_ptr<Component> component) {
        _component = component;
    }
    
    void remove() override {
        _component = nullptr;
    }

    std::shared_ptr<Component> get() {
        return _component;
    }

    void* get_raw() {
        return (void*) (_component.get());
    }
};
} // namespace detail

// FWD
class world;
class transform;
namespace math { class Point; }

/**
 * An entity, or "game object" is something that hold components inside a world.
 */
class entity {
    friend class world;
private:
    guaranteed_ptr<world> _world;
    std::string _name;
    
    std::unordered_map<std::type_index, std::shared_ptr<detail::homogeneous_component_slot>> _components_slots;

    bool _destroy_has_been_requested = false;

    // only for the world
    void destroy_now() {
        for(auto& slot : _components_slots) {
            // remove on registry
            engine::get_registry().remove(slot.first, slot.second->get_raw());
            // remove on self
            slot.second->remove();
        }
        _components_slots.clear();
    }
public:
    entity(guaranteed_ptr<world> world, std::string name);
    entity(guaranteed_ptr<world> world) : entity(world, "entity") {}
    ~entity() {
        destroy_now();
        std::cout << "entity " << _name << " deleted" << std::endl;
    }

    inline const std::string& name() const {
        return _name;
    }

    template <component Component, typename... _Args>
    std::shared_ptr<Component> create_component(_Args&&... args) {
        std::shared_ptr<Component> cmpt = std::make_shared<Component>(this, std::forward<_Args>(args)...);
        register_component(cmpt);
        engine::get_registry().add(cmpt);
        return cmpt;
    }

    template <component Component>
    void register_component(std::shared_ptr<Component> cmpt) {
        using cmpt_slot_impl = detail::homogeneous_component_slot_impl<Component>;
        const std::type_index tidx = std::type_index(typeid(Component));
        std::shared_ptr<cmpt_slot_impl> slot_ptr = nullptr;
        auto it = _components_slots.find(tidx);
        if(it != _components_slots.end()) {
            slot_ptr = std::dynamic_pointer_cast<cmpt_slot_impl>(it->second);
            assert(slot_ptr != nullptr);
        } else {
            slot_ptr = std::make_shared<cmpt_slot_impl>();
            _components_slots.insert({tidx, slot_ptr});
        }
        slot_ptr->set(cmpt);
    }

    template <component Component>
    std::shared_ptr<Component> get_component() {
        using cmpt_slot_impl = detail::homogeneous_component_slot_impl<Component>;
        const std::type_index tidx = std::type_index(typeid(Component));
        std::shared_ptr<cmpt_slot_impl> slot_ptr = nullptr;
        auto it = _components_slots.find(tidx);
        if(it != _components_slots.end()) {
            slot_ptr = std::dynamic_pointer_cast<cmpt_slot_impl>(it->second);
            assert(slot_ptr != nullptr);
        }
        return slot_ptr->get();
    }

    std::shared_ptr<transform> get_transform();
    math::Point get_world_pos();

    guaranteed_ptr<world> get_world() {
        return _world;
    }

    void request_destroy();
};

};
