@echo off

cls

if "%arch%" == "" (
	call setenv.bat
)

%ProjectDir%\Release\Tools\SFXCAB.exe "%output%\%d3d-exe%" "%root%\%d3d%" -r:update/update.exe -ipd -iswu

ECHO Done!

timeout 3>nul
