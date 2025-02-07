engine_sources = [
    'engine/engine.cc'
]

engine_lib = static_library(
    'engine_static', engine_sources,
    dependencies: [spdlog_dep],
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
