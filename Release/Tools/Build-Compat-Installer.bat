@echo off

cls

if "%arch%" == "" (
	call setenv.bat
)

%ProjectDir%\Release\Tools\SFXCAB.exe "%output%\%compat-exe%" "%root%\%compat%" -r:update/update.exe -ipd -iswu

pause
