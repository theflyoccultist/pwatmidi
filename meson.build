project('pwatmidi', 'cpp',
    default_options: ['cpp_std=c++20'])

cmake = import('cmake')

sources = [
  'src/main.cpp',
  'src/AudioEngine.cpp',
  'src/UI.cpp'
  ]

opts = cmake.subproject_options()

midifile_subproj = cmake.subproject('midifile', options: opts)
midifile_dep = midifile_subproj.dependency('midifile')

pipewire_dep = dependency('libpipewire-0.3')
ncurses_dep = dependency('ncurses')

executable('pwatmidi', sources, 
            dependencies: [pipewire_dep, ncurses_dep, midifile_dep])
