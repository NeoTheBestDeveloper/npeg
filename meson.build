project('npeg', 'c',
  version: '0.0.0',
  default_options: [
    'warning_level=3',
    'c_std=c11',
    'buildtype=debug',
    'b_sanitize=address,undefined',
    'b_lundef=false'
])

subdir('src')
incdir = include_directories('src/include')

cc = meson.get_compiler('c')
warning_flags = [
  '-Wfloat-equal', 
  '-Wundef', 
  '-Wshadow', 
  '-Wpointer-arith', 
  '-Wcast-align',
  '-Wwrite-strings',
  '-Wcast-qual',
  '-Wswitch-default',
  '-Wswitch-enum',
  '-Wconversion',
  '-Wformat=2',
  '-Wpedantic', 
  '-Wformat-security',
  '-Wtype-limits',
  '-Wunused-result',
  '-Wvariadic-macros',
]


add_project_arguments(cc.get_supported_arguments(warning_flags), language: 'c')

cc = meson.get_compiler('c')
math_dep = cc.find_library('m', required : true)

executable('npeg', sources, include_directories: incdir, dependencies : math_dep)
