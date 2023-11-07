@echo off

title One-Core-API Binaries Maker

call Tools\choose-arch.bat
call Tools\setenv.bat

call Build-All.bat
