@echo off

cls

for %%d in (%~dp0..\..) do set ProjectDir=%%~fd
echo ProjectDir=%ProjectDir%

REM ECHO Please choose architecture of you want compress package:
REM ECHO.
REM ECHO 1.x86
REM ECHO 2.x64
REM ECHO.

REM set /p a=Type option:
REM IF %a%==1 (
	REM REM Call patch bins
	REM set arch=x86
REM )

REM IF %a%==2 (
	REM REM Call patch bins
	REM set arch=x64
REM )

REM IF NOT %a%==1 (
	REM IF NOT %a%==2 (
		REM REM Call patch bins
		REM ECHO Wrong option, will do nothing
		REM pause
		REM goto :EOF
	REM )
REM )

set root=%ProjectDir%\Packages\%arch%
set output=%ProjectDir%\Output\%arch%
set api-set=API-SET Installer
set base=Base Installer
set compat=App Compat Installer
set driver-update=Driver Installer
set d3d=D3D Installer
set additional=Additional Dlls Installer
set branding=Branding Installer
set modern=Modern Setup Installer

set api-set-exe=one-core-api-api-set.exe
set base-exe=one-core-api-base.exe
set compat-exe=one-core-api-app-compat.exe
set driver-update-exe=one-core-api-driver-update.exe
set d3d-exe=one-core-api-d3d.exe
set additional-exe=one-core-api-additional-dll.exe
set branding-exe=one-core-api-branding.exe
set modern-exe=one-core-api-modern-setup.exe

cls