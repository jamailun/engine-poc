project('engine-poc', ['c','cpp'], default_options: ['cpp_std=c++2a'])

cpp_warning_flags = [
  '-Wall',
  '-Wextra',
  '-pedantic-errors',
  '-Wdisabled-optimization',
  '-Werror=unused-result',
  '-Werror=return-type'
]

# Spdlog
spdlog = subproject('spdlog', required: true)
spdlog_dep = spdlog.get_variable('spdlog_dep')
add_project_arguments('-DSPDLOG_FMT_EXTERNAL', language: 'cpp')
add_project_arguments('-DSPDLOG_FMT_EXTERNAL', language: 'c')

# Engine Build
subdir('include')
subdir('src')

# Example app
subdir('app')
