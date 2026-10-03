project('pwatmidi', 'cpp',
    default_options: ['cpp_std=c++20'])

pipewire_dep = dependency('libpipewire-0.3')
executable('pwatmidi', 'src/main.cpp', 
            dependencies: [pipewire_dep])
