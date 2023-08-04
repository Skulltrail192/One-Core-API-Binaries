@echo off

cls

if "%arch%" == "" (
	call setenv.bat
)

ECHO %root%
ECHO %output%
ECHO %branding-exe%
ECHO %branding%

%ProjectDir%\Release\Tools\SFXCAB.exe "%output%\%branding-exe%" "%root%\%branding%" -r:update/update.exe -ipd -iswu

pause
