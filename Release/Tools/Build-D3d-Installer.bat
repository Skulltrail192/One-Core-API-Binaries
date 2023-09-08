@echo off

cls

if "%arch%" == "" (
	call choose-arch.bat
	call setenv.bat
)

%ProjectDir%\Release\Tools\SFXCAB.exe "%output%\%d3d-exe%" "%root%\%d3d%" -r:update/update.exe -ipd -iswu

ECHO Done!

timeout /T 3>nul

cls