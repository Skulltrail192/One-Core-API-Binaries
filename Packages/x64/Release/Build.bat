@echo off

set root=F:\Develop\One-Core\One-Core-API-Binaries\Packages\x64
set api-set=API-SET Installer
set base=Base Installer
set compat=App Compat Installer
set d3d=D3D Installer
set new=New Dlls Installer
set shell=Shell Update Installer

set api-set-exe=one-core-api-api-set.exe
set base-exe=one-core-api-app-base.exe
set compat-exe=one-core-api-app-compat.exe
set d3d-exe=one-core-api-d3d.exe
set new-exe=one-core-api-new.exe
set shell-exe=one-core-api-shell.exe

SFXCAB.exe "%api-set-exe%" "%root%\%api-set%" -r:update/update.exe -ipd -iswu
SFXCAB.exe "%base-exe%" "%root%\%base%" -r:update/update.exe -ipd -iswu
SFXCAB.exe "%compat-exe%" "%root%\%compat%" -r:update/update.exe -ipd -iswu
SFXCAB.exe "%d3d-exe%" "%root%\%d3d%" -r:update/update.exe -ipd -iswu
SFXCAB.exe "%new-exe%" "%root%\%new%" -r:update/update.exe -ipd -iswu
SFXCAB.exe "%shell-exe%" "%root%\%shell%" -r:update/update.exe -ipd -iswu

pause