--  ============================================================
--  premake5 build rules for covbr2html
--  ============================================================

buildoptions_vs = '/std:c++17 /MP /W4 /wd4100 /wd4103'
-- /wd4244 illegal conversion
buildoptions_gcc = '-std=c++17 -O3 -pedantic-errors -Wall'

workspace 'somcpp'

    configurations { 'ci', 'trace_on', 'trace_all' }
    language 'C++'
    targetdir '../build'
    objdir  '../build/%{_TARGET_OS}/%{cfg.name}'
    files { '../src/*.cpp' }

    defines { 'NDEBUG' }
    optimize 'Speed'
    includedirs { '../include' }

    filter { 'action:vs*' }
        warnings 'high'
        buildoptions { buildoptions_vs }

    filter { 'action:gmake*' }
        buildoptions { buildoptions_gcc }

    filter { 'configurations:ci' }

    filter { 'configurations:trace_on' }
        defines { 'TRACE_ON' }

    filter { 'configurations:trace_all' }
        defines { 'TRACE_ALL' }

    project 'somcpp'
        kind 'staticlib'
  
    project 'test'
        kind 'consoleapp'
        files { '../test/*.cpp' }

    project 'docopts'
        kind 'consoleapp'
        files { '../docopts/*.cpp' }
    