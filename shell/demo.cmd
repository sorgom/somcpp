@ECHO OFF
SETLOCAL

cd /d %~dp0

set docopts=..\build\docopts.exe
set fglob=..\build\fglob.exe
set helpTxt=demo_options.txt
set tmpCmd=tmp.cmd

if not exist %docopts% (
    echo %docopts% not found. Please build it first.
    goto end
)

echo.
echo - docopts
call %docopts% %helpTxt% %* > %tmpCmd%
if %errorlevel% NEQ 0 goto help

call %tmpCmd%

if %_h%==1 goto help
echo options: -c: %_c% -h: %_h% -t: %_t% -o: %_o%
echo args: %_args%

if not exist %fglob% (
    echo %fglob% not found. Please build it first.
    goto end
)

echo.
echo - fglob
call %fglob% %_args% > %tmpCmd%
call %tmpCmd%

echo args:
for %%a in (%_args%) do echo %%a
goto end

:help
echo Usage: %~nx0 [options] [args]
type %helpTxt%

:end
if exist %tmpCmd% del %tmpCmd%

ENDLOCAL
