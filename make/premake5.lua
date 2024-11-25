--  ============================================================
--  premake5 build rules
--  ============================================================

buildoptions_vs = '/std:c++17 /MP /W4 /wd4100 /wd4103'
-- /wd4244 illegal conversion
buildoptions_gcc = '-std=c++17 -O3 -pedantic-errors -Wall'

workspace 'somcpp'

    configurations { 'ci', 'trace_on', 'trace_all', 'glob_linux', 'glob_linux_trace_all' }
    language 'C++'
    targetdir '../build'
    objdir  '../build/%{_TARGET_OS}'
    kind 'consoleapp'

    defines { 'NDEBUG' }
    optimize 'Speed'
    includedirs { '../include' }

    filter { 'action:vs*' }
        warnings 'high'
        buildoptions { buildoptions_vs }
        defines { '_CRT_SECURE_NO_WARNINGS' }

    filter { 'action:gmake*' }
        buildoptions { buildoptions_gcc }

    filter { 'configurations:ci' }

    filter { 'configurations:trace_on' }
        defines { 'TRACE_ON' }

    filter { 'configurations:trace_all' }
        defines { 'TRACE_ALL' }

    filter { 'configurations:glob_linux' }
        defines { 'GLOB_LINUX' }

    filter { 'configurations:glob_linux_trace_all' }
        defines { 'GLOB_LINUX', 'TRACE_ALL' }

    project 'docopts'
        files { '../src/fio.cpp', '../src/docopts.cpp', '../runtime/docoptsMain.cpp' }

    project 'fglob'
        files { '../src/fglob.cpp', '../runtime/fglobMain.cpp' }

    project 'glob'
        files { '../src/Glob.cpp', '../src/GlobProcessors.cpp', '../src/docopts.cpp', '../runtime/globMain.cpp' }

        -- project 'lab'
        -- files { '../lab/*.cpp', '../src/*.cpp' }
        -- includedirs { '../lab' }

    project 'somcpp'
        kind 'staticlib'
        files { '../src/*.cpp' }
