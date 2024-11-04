@ECHO OFF
SETLOCAL

cd /d %~dp0

set docopts=..\build\docopts.exe

if not exist %docopts% (
    echo docopts.exe not found
    exit /b 1
)

%docopts% %*

@REM set _args=alpha beta gamma
@REM for %%a in (%_args%) do (
@REM     echo %%a
@REM )
