@echo off

cls

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

cls