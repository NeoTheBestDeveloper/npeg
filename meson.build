project('npeg', 'c',
  version: '0.0.0',
  default_options: [
    'warning_level=3',
    'c_std=c11',
    'buildtype=debug',
    'b_sanitize=address,undefined',
    'optimization=3',
    'b_lundef=false'
])

subdir('src')
incdir = include_directories('include')

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
  '-Wno-format-extra-args',
  '-Wno-missing-prototypes',
  '-Wno-padded',
  '-Wno-missing-declarations',
  '-Wno-suggest-attribute=const',
  '-Wno-declaration-after-statement',
]

avx_flags = [
  '-mfma',
  '-mavx2',
]

debug_flags = [
  '-fsanitize=address,undefined'
]

if get_option('buildtype').startswith('debug')
  add_project_arguments('-DDEBUG', language : 'c')
  add_project_arguments(cc.get_supported_arguments(debug_flags), language : 'c')
endif

add_project_arguments(cc.get_supported_arguments(warning_flags), language: 'c')
add_project_arguments(cc.get_supported_arguments(avx_flags), language: 'c')

cc = meson.get_compiler('c')
math_dep = cc.find_library('m', required : true)

executable('npeg', sources, include_directories: incdir, dependencies: math_dep)
shared_library('npeg', npeg_sources, include_directories: incdir, dependencies: math_dep)
