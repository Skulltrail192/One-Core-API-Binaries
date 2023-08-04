@echo off

cls

if "%arch%" == "" (
	call setenv.bat
)

%ProjectDir%\Release\Tools\SFXCAB.exe "%output%\%base-exe%" "%root%\%base%" -r:update/update.exe -ipd -iswu

ECHO Done!

timeout 3>nul
