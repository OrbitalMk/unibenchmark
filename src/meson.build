unibenchmark_sources = [
    'main.c',
    'gtkobjects/unibenchmark-window.c',
    'gtkobjects/info-unibenchmark.c',
    'utils/benchmark.c',
    'utils/system-info.c',
    'utils/wmi.c',
]

unibenchmark_deps = [
    dependency('gio-2.0', version: '>= 2.50'),
    dependency('gtk+-3.0', version: '>= 3.22'),
]

gnome = import('gnome')

unibenchmark_sources += gnome.compile_resources('unibenchmark-resources',
    'unibenchmark.gresource.xml',
    c_name: 'unibenchmark'
)

exe = executable('unibenchmark', unibenchmark_sources,
    dependencies: unibenchmark_deps,
    install: true,
    link_args: ['-lwbemuuid', '-mwindows']
)
