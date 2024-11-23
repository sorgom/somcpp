@ECHO OFF
SETLOCAL

set cDir=%CD%
cd /d %~dp0
set helpTxt=%CD%\demo_options.txt

cd ..
set docopts=%CD%\build\docopts.exe
set glob=%CD%\build\glob.exe
set tmpCmd=%CD%\build\tmp.cmd

echo.
echo - docopts
if not exist %docopts% (
    echo first build %docopts% to proceed
    exit /b 1
)

%docopts% %helpTxt% %* > %tmpCmd%
if %errorlevel% NEQ 0 exit /b %errorlevel%

call %tmpCmd%

if %_h% (
    echo Usage: %~nx0 [options] [args]
    type %helpTxt%
    exit /b 0
)
echo options: -c: %_c% -t: %_t% -H %_Hu% -o: %_o%
echo args: %_args%

cd %cDir%
echo.
echo - glob
if not exist %glob% (
    echo first build %glob% to proceed
    exit /b 1
)

%glob% -p "<X>" -x "echo <X>" %_args% > %tmpCmd%

echo args:
call %tmpCmd%

ENDLOCAL
