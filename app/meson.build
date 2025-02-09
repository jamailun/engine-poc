app_headers = include_directories('.')

app_sources = [
    # Components
    'components/army_controller.cc',
    'components/key_controller.cc',
    'components/key_camera_controller.cc',
    'components/army_controller.cc',
    'components/paintable_region.cc',
    'components/soldiers/soldier.cc',
    'components/soldiers/soldier_cac.cc',

    # Gameplay
    'gameplay/command.cc',

    # "Interfaces"
    'interfaces/livable.cc',

    # Other
    
    'army.cc',
    'game_state.cc',
    'soldier_container.cc',

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
