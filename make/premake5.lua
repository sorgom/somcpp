--  ============================================================
--  premake5 build rules for covbr2html
--  ============================================================

buildoptions_vs = '/std:c++17 /MP /W4 /wd4100 /wd4103'
-- /wd4244 illegal conversion
buildoptions_gcc = '-std=c++17 -O3 -pedantic-errors -Wall'

workspace 'somcpp'

    configurations { 'ci', 'debug' }
    language 'C++'
    targetdir '../build'
    objdir  '../build/%{_TARGET_OS}/%{cfg.name}'
    files { '../src/*.cpp' }

    includedirs { '../include' }

    filter { 'action:vs*' }
        warnings 'high'
        buildoptions { buildoptions_vs }

    filter { 'action:gmake*' }
        buildoptions { buildoptions_gcc }

    filter { 'configurations:ci' }
        defines { 'NDEBUG' }
        optimize 'Speed'

    filter { 'configurations:debug' }
        defines { 'DEBUG' }
        symbols 'On'

    project 'somcpp'
        kind 'staticlib'
  
    project 'test'
        kind 'consoleapp'
        files { '../test/*.cpp' }
