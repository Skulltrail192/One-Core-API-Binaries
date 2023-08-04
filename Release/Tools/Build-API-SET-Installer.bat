@echo off

cls

if "%arch%" == "" (
	call setenv.bat
)

%ProjectDir%\Release\Tools\SFXCAB.exe "%output%\%api-set-exe%" "%root%\%api-set%" -r:update/update.exe -ipd -iswu

ECHO Done!

timeout 2>nul

cls
