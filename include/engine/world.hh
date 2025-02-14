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

    void destroy_entity_now(entity* entity_ptr) {
        for(auto it = _entities.begin(); it != _entities.end(); it++) {
            if(it->get() == entity_ptr) {
                _entities.erase(it);
                return;
            }
        }
    }

    void destroy_now() {
        for(auto& entity : _entities) {
            entity->destroy_now();
        }
        _entities.clear();
    }
    
    // nul
    std::vector<std::shared_ptr<entity>> get_entities_around(math::Point position, float distance);

};

} // namespace engine
