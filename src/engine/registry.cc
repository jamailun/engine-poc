#include <engine/registry.hh>

using namespace engine::detail;

bool engine::detail::sort_homogeneous_component_registries(std::shared_ptr<homogeneous_component_registry> a, std::shared_ptr<homogeneous_component_registry> b) {
    return a->static_z_index() < b->static_z_index();
}
