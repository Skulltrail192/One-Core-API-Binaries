@echo off

set root=%~dp0..\x86
set output=%~dp0..\Output\x86
set api-set=API-SET Installer
set base=Base Installer
set compat=App Compat Installer
set driver-update=Driver Installer
set d3d=D3D Installer
set additional=Additional Dlls Installer
set branding=Branding Installer
set modern=Modern Setup Installer
REM set shell=Shell Update Installer

set api-set-exe=one-core-api-api-set.exe
set base-exe=one-core-api-app-base.exe
set compat-exe=one-core-api-app-compat.exe
set driver-update-exe=one-core-api-app-driver-update.exe
set d3d-exe=one-core-api-d3d.exe
set additional-exe=one-core-api-additional-dll.exe
set branding-exe=one-core-api-branding.exe
set modern-exe=one-core-api-modern-setup.exe
REM set shell-exe=one-core-api-shell.exe

SFXCAB.exe "%output%\%api-set-exe%" "%root%\%api-set%" -r:update/update.exe -ipd -iswu
SFXCAB.exe "%output%\%base-exe%" "%root%\%base%" -r:update/update.exe -ipd -iswu
SFXCAB.exe "%output%\%compat-exe%" "%root%\%compat%" -r:update/update.exe -ipd -iswu
SFXCAB.exe "%output%\%driver-update-exe%" "%root%\%driver-update%" -r:update/update.exe -ipd -iswu
SFXCAB.exe "%output%\%d3d-exe%" "%root%\%d3d%" -r:update/update.exe -ipd -iswu
SFXCAB.exe "%output%\%additional-exe%" "%root%\%additional%" -r:update/update.exe -ipd -iswu
SFXCAB.exe "%output%\%branding-exe%" "%root%\%branding%" -r:update/update.exe -ipd -iswu
SFXCAB.exe "%output%\%modern-exe%" "%root%\%modern%" -r:update/update.exe -ipd -iswu
REM SFXCAB.exe "%output%\%shell-exe%" "%root%\%shell%" -r:update/update.exe -ipd -iswu
