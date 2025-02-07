#pragma once

#include <iostream>
#include <string>
#include "engine/registry.hh"
#include "engine/component.hh"

namespace engine {
namespace detail {

class homogeneous_component_slot {
protected:
    virtual ~homogeneous_component_slot() = default;
public:
    virtual void remove() = 0;
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
};
}

class entity {
private:
    std::string _name;
public:
    entity(std::string name) : _name(name) {
        std::cout << "Nouvelle entity: '" << name << "'." << std::endl;
    }
    entity() : entity("entity") {}
    ~entity() {
        destroy_now();
        std::cout << "entity " << _name << " deleted" << std::endl;
    }

    void destroy_now() {
        for(auto& slot : _components_slots) {
            slot.second->remove();
        }
        _components_slots.clear();
    }

    inline const std::string& name() const {
        return _name;
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

private:
    std::unordered_map<std::type_index, std::shared_ptr<detail::homogeneous_component_slot>> _components_slots;
};

};
