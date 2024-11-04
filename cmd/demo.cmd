@ECHO OFF
SETLOCAL

cd /d %~dp0
set myDir=%cd%

set docopts=..\build\docopts.exe
set helpTxt=%myDir%\demo_help.txt
set tmpCmd=%myDir%\tmp.cmd 

if not exist %docopts% (
    echo docopts.exe not found
    exit /b 1
)

%docopts% %helpTxt% %* > %tmpCmd%
if %errorlevel% neq 0 exit /b 1

call %tmpCmd%
rm -f %tmpCmd%

if %_h%==1 (
    echo Usage: %~nx0 [options] [args]
    cat %helpTxt%
    exit /b 0
)
echo options: -c: %_c% -h: %_h% -t: %_t% -o: %_o%
echo args: %_args%

ENDLOCAL