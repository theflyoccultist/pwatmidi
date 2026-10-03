project('pwatmidi', 'cpp',
    default_options: ['cpp_std=c++20'])

sources = [
  'src/main.cpp',
  'src/AudioEngine.cpp',
  'src/UI.cpp'
  ]

pipewire_dep = dependency('libpipewire-0.3')
ncurses_dep = dependency('ncurses')

executable('pwatmidi', sources, 
            dependencies: [pipewire_dep, ncurses_dep])
