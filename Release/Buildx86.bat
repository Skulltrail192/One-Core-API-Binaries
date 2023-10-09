@echo off

set arch=x86
call Tools\setenv.bat

call Tools\Build-Additional-Dll-Installer.bat
call Tools\Build-API-SET-Installer.bat
call Tools\Build-Base-Installer.bat
call Tools\Build-Branding-Installer.bat
call Tools\Build-Compat-Installer.bat
call Tools\Build-D3D-Installer.bat
call Tools\Build-Driver-Update-Installer.bat
call Tools\Build-Modern-Setup-Installer.bat
