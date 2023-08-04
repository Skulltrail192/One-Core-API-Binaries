@echo off

cls

if "%arch%" == "" (
	call setenv.bat
)

%ProjectDir%\Release\Tools\SFXCAB.exe "%output%\%modern-exe%" "%root%\%modern%" -r:update/update.exe -ipd -iswu
