
engine_sources = []

engine_lib = static_library(
    'engine_static', engine_sources,
    dependencies: [],
    cpp_args: cpp_warning_flags,
    link_args: [],
    include_directories: engine_headers_dir,
    install: false,
    build_by_default: true
)

engine_dep = declare_dependency(
    link_with : engine_lib,
    dependencies: [],
    include_directories : engine_headers_dir
)
