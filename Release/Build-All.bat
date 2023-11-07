@echo off

call Tools\Build-Additional-Dll-Installer.bat
REM call Tools\Build-API-SET-Installer.bat
call Tools\Build-Base-Installer.bat
call Tools\Build-Branding-Installer.bat
call Tools\Build-Compat-Installer.bat
call Tools\Build-D3D-Installer.bat
call Tools\Build-Driver-Update-Installer.bat
REM call Tools\Build-Modern-Setup-Installer.bat