project('engine-poc', ['c','cpp'], default_options: ['cpp_std=c++2a'])

cpp_warning_flags = [
  '-Wall',
  '-Wextra',
  '-pedantic-errors',
  '-Wdisabled-optimization',
  '-Werror=unused-result',
  '-Werror=return-type',
  '-DSPDLOG_COMPILED_LIB'
]

# Dependencies
spdlog_dep = dependency('spdlog', required: true)
sdl2_dep = dependency('SDL2', required: true)
sdl2_image_dep = dependency('SDL2_image', required: true)

# Engine Build
subdir('include')
subdir('src')

# Example app
subdir('app')
