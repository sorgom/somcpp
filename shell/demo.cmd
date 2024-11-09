@ECHO OFF
SETLOCAL

cd /d %~dp0

set docopts=..\build\docopts.exe
set fglob=..\build\fglob.exe
set helpTxt=demo_options.txt
set tmpCmd=..\build\tmp.cmd

echo.
echo - docopts
if not exist %docopts% (
    echo first build %docopts% to proceed
    exit /b 1
)

call %docopts% %helpTxt% %* > %tmpCmd%
if %errorlevel% NEQ 0 exit /b %errorlevel%

call %tmpCmd%

if %_h% (
    echo Usage: %~nx0 [options] [args]
    type %helpTxt%
    exit /b 0
)
echo options: -c: %_c% -t: %_t% -H %_Hu% -o: %_o%
echo args: %_args%

echo.
echo - fglob
if not exist %fglob% (
    echo first build %fglob% to proceed
    exit /b 1
)

call %fglob% %_args% > %tmpCmd%
call %tmpCmd%

echo args:
for %%a in (%_args%) do echo %%a

ENDLOCAL
