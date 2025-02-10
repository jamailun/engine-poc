#pragma once

#include "soldier.hh"

namespace game {

const uint16_t COMPONENT_TYPE__SOLDIER_CAC = 1001;

/**
 * A soldier that attacks by physical contact.
 */
class SoldierCac : public Soldier {
public:
    SoldierCac(entity_ptr entity, guaranteed_ptr<Army> army);
    ~SoldierCac() = default;
};

} // namespace game
