#pragma once

#include <iostream>
#include <string>
#include "engine/entity.hh"
#include "engine/components/transform.hh"

namespace engine {

class world {
private:
    std::string _name;
    std::vector<std::shared_ptr<entity>> _entities;
public:
    world(std::string name) : _name(name) {};
    ~world() {
        destroy_now();
    }

    const std::string& name() const { return _name; }

    std::shared_ptr<entity> create_entity(std::string name) {
        auto entity = std::make_shared<engine::entity>(this, name);
        _entities.push_back(entity);
        //entity->create_component<transform>();
        return entity;
    }

    void destroy_now() {
        for(auto& entity : _entities) {
            entity->destroy_now();
        }
        _entities.clear();
    }
};

} // namespace engine
