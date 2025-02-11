#include <engine/entity.hh>

#include "game/army.hh"
#include "./soldier_cac.hh"

using namespace game;

const float CAC_SPEED = 100;
const float CAC_RADIUS = 6;

SoldierCac::SoldierCac(entity_ptr entity, guaranteed_ptr<Army> army)
    : Soldier(entity, army, CAC_SPEED, CAC_RADIUS)
{}
