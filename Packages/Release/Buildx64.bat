@echo off

set root=D:\Develop\One-Core\One-Core-API-Binaries\Packages\x64
set output=D:\Develop\One-Core\Output\Binaries\x64
set api-set=API-SET Installer
set base=Base Installer
set compat=App Compat Installer
set driver-update=Driver Installer
set d3d=D3D Installer
set additional=Additional Dlls Installer
REM set shell=Shell Update Installer

set api-set-exe=one-core-api-api-set.exe
set base-exe=one-core-api-app-base.exe
set compat-exe=one-core-api-app-compat.exe
set driver-update-exe=one-core-api-app-driver-update.exe
set d3d-exe=one-core-api-d3d.exe
set additional-exe=one-core-api-additional-dll.exe
REM set shell-exe=one-core-api-shell.exe

SFXCAB.exe "%output%\%api-set-exe%" "%root%\%api-set%" -r:update/update.exe -ipd -iswu
SFXCAB.exe "%output%\%base-exe%" "%root%\%base%" -r:update/update.exe -ipd -iswu
SFXCAB.exe "%output%\%compat-exe%" "%root%\%compat%" -r:update/update.exe -ipd -iswu
SFXCAB.exe "%output%\%driver-update-exe%" "%root%\%driver-update%" -r:update/update.exe -ipd -iswu
SFXCAB.exe "%output%\%d3d-exe%" "%root%\%d3d%" -r:update/update.exe -ipd -iswu
SFXCAB.exe "%output%\%additional-exe%" "%root%\%additional%" -r:update/update.exe -ipd -iswu
REM SFXCAB.exe "%output%\%shell-exe%" "%root%\%shell%" -r:update/update.exe -ipd -iswu

pause