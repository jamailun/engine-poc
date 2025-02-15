app_headers = include_directories('.')

app_sources = [
    'game/configuration/args_reader.cc',

    # Components
    'game/components/army_controller.cc',
    'game/components/clock_component.cc',
    'game/components/fps_displayer.cc',
    'game/components/key_controller.cc',
    'game/components/key_camera_controller.cc',
    'game/components/army_controller.cc',
    'game/components/paintable_region.cc',
    'game/components/soldiers/soldier.cc',
    'game/components/soldiers/soldier_cac.cc',

    # Gameplay
    'game/gameplay/command.cc',

    # Other
    
    'game/army.cc',
    'game/game_state.cc',
    'game/soldiers_container.cc',

    # Main
    'main.cc'
]

executable(
    'engine-test', app_sources,
    dependencies: [engine_dep],
    include_directories: app_headers,
    install: false,
    build_by_default: true
)
