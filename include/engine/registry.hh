
#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "engine/component.hh"



namespace engine::detail {

class homogeneous_component_registry {

public:
    virtual void remove(void* addr) = 0;

    virtual void update_all() = 0;
    virtual void action_all() = 0;

    virtual ~homogeneous_component_registry() = default;

};


template <component Component>
class homogeneous_component_registry_impl: public homogeneous_component_registry {

public:

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

    void update_all() override {
        if constexpr(updatable<Component>) {
            std::for_each(_components.begin(), _components.end(), [](std::shared_ptr<Component>& cmpt) {
                cmpt->update();
            });
        }
    }

    void action_all() override {
        if constexpr(actionable<Component>) {
            std::for_each(_components.begin(), _components.end(), [](std::shared_ptr<Component>& cmpt) {
                cmpt->action();
            });
        }
    }

    virtual ~homogeneous_component_registry_impl() = default;

private:
    std::vector<std::shared_ptr<Component>> _components;

};

} // namespace engine::detail


namespace engine {

class component_registry {

public:
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
