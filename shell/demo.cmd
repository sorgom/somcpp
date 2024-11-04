@ECHO OFF
SETLOCAL

cd /d %~dp0

set docopts=..\build\docopts.exe
set helpTxt=demo_help.txt
set tmpCmd=tmp.cmd 

if not exist %docopts% (
    echo docopts.exe not found. Please build it first.
    exit /b 1
)

%docopts% %helpTxt% %* > %tmpCmd%
if %errorlevel% NEQ 0 (
    rm -f %tmpCmd%
    echo Usage: %~nx0 [options] [args]
    type %helpTxt%
    exit /b 1
)

call %tmpCmd%
rm -f %tmpCmd%

if %_h%==1 (
    echo Usage: %~nx0 [options] [args]
    type %helpTxt%
    exit /b 0
)
echo options: -c: %_c% -h: %_h% -t: %_t% -o: %_o%
echo args: %_args%

ENDLOCAL
