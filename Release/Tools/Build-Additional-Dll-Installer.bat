@echo off

cls

if "%arch%" == "" (
	call setenv.bat
)

%ProjectDir%\Release\Tools\SFXCAB.exe "%output%\%additional-exe%" "%root%\%additional%" -r:update/update.exe -ipd -iswu

ECHO Done!

timeout 2>nul

cls

