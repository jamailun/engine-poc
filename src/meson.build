engine_sources = [
    'engine/entity.cc',
    'engine/engine.cc',
    'engine/engine_loop.cc',
    'engine/engine_loop_event.cc',
    'engine/game_camera.cc',
    'engine/math.cc',
    'engine/registry.cc',
    'engine/world.cc',

    'engine/components/image_renderer.cc',
    'engine/components/text_renderer.cc',
    'engine/components/transform.cc',
    'engine/components/tilemap_renderer.cc',
    'engine/components/ui/ui_button.cc',
    'engine/components/ui/ui_text.cc',

    'engine/resources/resources_manager.cc',
    'engine/resources/image.cc',
    'engine/resources/tile_map.cc',
    'engine/resources/tile_set.cc',

    'engine/sdl/colors.cc',
    'engine/sdl/configuration.cc',
    'engine/sdl/drawer.cc',
    'engine/sdl/inputs.cc',
    'engine/sdl/game_window.cc',
    
    'engine/utils/fs_utils.cc'
]

engine_lib = static_library(
    'engine_static', engine_sources,
    dependencies: [spdlog_dep, sdl2_dep, sdl2_image_dep, sdl2_text_dep],
    cpp_args: cpp_warning_flags,
    link_args: [],
    include_directories: engine_headers_dir,
    install: false,
    build_by_default: true
)

engine_dep = declare_dependency(
    link_with : engine_lib,
    compile_args: cpp_warning_flags,
    dependencies: [],
    include_directories : engine_headers_dir
)
