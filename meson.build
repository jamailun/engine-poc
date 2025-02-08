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

# Spdlog

# spdlog = subproject('spdlog', required: true)
# spdlog_dep = spdlog.get_variable('spdlog_dep')
spdlog_dep = dependency('spdlog', required: true)

# Engine Build
subdir('include')
subdir('src')

# Example app
subdir('app')
