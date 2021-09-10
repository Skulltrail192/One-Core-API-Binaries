Welcome to One-Core-API binaries!

Here you can found binaries of One-Core-API project. It's compatible with Windows Server 2003 SP2, Windows XP SP3 and Windows XP
x64 SP2. You can do amazing things installing binaries.

Now we will describe folders present on this repository:
- Packages (x86 and x64, each subfolder contain folder what binaries are placed, and each folder is a installer. 
You can download and install binaries for example: going to x86\Base installer\update and run update.exe);
- Decrepated: Installer no more used
- Documents: Some documents about this project, related with know bug, how use sfxcab (to do installers) and others/
- Legacy/Repository: Old repository, to remove on next releases;
- Todo:  To do tasks
- Test: Some binaries and documents to do tests

Packages are divided in:
API-SET Installer: contain all api-set implemented on wine. API-SET dlls are introduced on Windows 7 and are required by several 
applications today;
App Compat Installer: 
Base Installer: a base of all one-core-api. This installer is the pre-requisite to all others installers, except Kernel Update 
Standalone installer. And contain all wrappers used on this project (like kernelbase and ntext)
D3d Installer: contain d3d runtime (Directx dlls, mainly directx 10 and directx 11, based on wine d3d, a converter from directx 
to opengl calls);
Kernel Installer

You can run several programs with One-Core-API
