REM set target path
set TARGETPATH_x86=.\bin\x86
set TARGETPATH_x64=.\bin\x64

REM set DDK path
set DDKDIR=C:\WinDDK\7600.16385.1

REM remove and recreate target path
rmdir %TARGETPATH_x86% /S /Q
rmdir %TARGETPATH_x64% /S /Q
mkdir %TARGETPATH_x86%
mkdir %TARGETPATH_x64%

REM build x86 driver
setlocal
call %DDKDIR%\bin\setenv.bat %DDKDIR% fre x86 WNET
cd /d %~dp0
if not exist obj%BUILD_ALT_DIR% mkdir obj%BUILD_ALT_DIR%
build -cewgZ /jpath obj%BUILD_ALT_DIR%
endlocal
if errorlevel 1 goto :error

REM build x64 driver
setlocal
call %DDKDIR%\bin\setenv.bat %DDKDIR% fre x64 WNET
cd /d %~dp0
if not exist obj%BUILD_ALT_DIR% mkdir obj%BUILD_ALT_DIR%
build -cewgZ /jpath obj%BUILD_ALT_DIR%
endlocal
if errorlevel 1 goto :error

REM copy driver files to bin directory
copy objfre_wnet_x86\i386\nvme.sys %TARGETPATH_x86%\
copy objfre_wnet_AMD64\amd64\nvme.sys %TARGETPATH_x64%\
copy nvme.inf %TARGETPATH_x86%\
copy nvme.inf %TARGETPATH_x64%\

REM sign driver files
setlocal
PATH=%PATH%;%DDKDIR%\bin\selfsign;%DDKDIR%\bin\x86
inf2cat /driver:%TARGETPATH_x86% /os:Server2003_X86,Server2003_X64 /verbose
makecert -$ individual -r -pe -ss MY -n CN="nvme" %TARGETPATH_x86%\nvme86.cer
signtool.exe sign /v /s MY /n "nvme" %TARGETPATH_x86%\*.sys %TARGETPATH_x86%\*.cat
certmgr -del -c -n "nvme" -s -r currentUser MY
inf2cat /driver:%TARGETPATH_x64% /os:Server2003_X86,Server2003_X64 /verbose
makecert -$ individual -r -pe -ss MY -n CN="nvme" %TARGETPATH_x64%\nvme64.cer
signtool.exe sign /v /s MY /n "nvme" %TARGETPATH_x64%\*.sys %TARGETPATH_x64%\*.cat
certmgr -del -c -n "nvme" -s -r currentUser MY
endlocal
if errorlevel 1 goto :error

REM cleanup
rmdir "objfre_wnet_x86" /S /Q
rmdir "objfre_wnet_amd64" /S /Q
del %TARGETPATH_x86%\nvme86.cer
del %TARGETPATH_x64%\nvme64.cer
REM delete also the catalog files for the unused platform, these get created
REM based on our INF file
del %TARGETPATH_x86%\nvme64.cat
del %TARGETPATH_x64%\nvme86.cat
goto :end

REM show error message
:error
color 0C
echo Build errors occurred
pause
exit

:end
color 0A
echo Build succeeded
pause