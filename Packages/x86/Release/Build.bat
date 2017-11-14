@echo off

set source=../Base Installer
set output=one-core-api-base.exe

SFXCAB.exe %output% %source% -r:update/update.exe -ipd -iswu

pause