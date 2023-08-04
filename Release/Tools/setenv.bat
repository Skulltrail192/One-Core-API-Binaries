@echo off

cls

for %%d in (%~dp0..\..) do set ProjectDir=%%~fd
echo ProjectDir=%ProjectDir%

ECHO Please choose architecture of you want compress package:
ECHO.
ECHO 1.x86
ECHO 2.x64
ECHO.

set /p a=Type option:
IF %a%==1 (
	REM Call patch bins
	set arch=x86
)

IF %a%==2 (
	REM Call patch bins
	set arch=x64
)

IF NOT %a%==1 (
	IF NOT %a%==2 (
		REM Call patch bins
		ECHO Wrong option, will do nothing
		pause
		goto :EOF
	)
)

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
set base-exe=one-core-api-app-base.exe
set compat-exe=one-core-api-app-compat.exe
set driver-update-exe=one-core-api-app-driver-update.exe
set d3d-exe=one-core-api-d3d.exe
set additional-exe=one-core-api-additional-dll.exe
set branding-exe=one-core-api-branding.exe
set modern-exe=one-core-api-modern-setup.exe

cls