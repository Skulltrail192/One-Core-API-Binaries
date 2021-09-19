Welcome to One-Core-API binaries!

This repositry contains binary releases of the One-Core-API project. These are compatible with Windows Server 2003 SP2, Windows XP SP3 and Windows XP
x64 SP2.

Folders in this repository:
- Deprecated: Old installer (no longer used).
- Documents: Project documentation, known bugs, sfxcab usage (for making installers), etc.
- Legacy\Repository: Old repository, to be removed in subsequent commits.
- Packages\x86 and Packages\x64: Binary releases categorized by package. You can download and install/update the packages directly from here (i.e. by going to Packages\x86\Base installer\update and runnning update.exe).
- Todo: To do tasks.
- Test: Some binaries and documents for testing.

The One-Core-API project consists of the following packages:
- Base: One-Core-API base. This package is required by all other packages, except Kernel Standalone, and contains all wrappers used in this project (like kernelbase and ntext).
- New Dlls: Several new dlls introduced in later versions of Windows.
- D3D: D3D runtime (Mainly DX10 and DX11, based on WineD3D).
- API-SET: Contains UCRT ddls based on wine.
- Kernel: Experimental kernel update. Depends on the Base package.
- Kernel Standalone: Experimental kernel update. Does not depend on the Base package. This package cannot be installed alongside any other packages, except App Compat.
- App Compat: Application compatbility settings, backported from later versions of Windows.

One-Core-API allows you to run:
- Web Storm 2018
- Intelliji 2018 (Other versions may work too)
- Filezilla (latest)
- Visual Studio Code 1.18
- Chrome up to version 60
- Opera up to version 38
- Firefox up to version 53
- JDK 1.8
- Maxthon 5.1
- Python 3.6
- .Net Framework up to 4.8
- Geekbench 4.2
- Performance Test
- Adobe Reader DC (2021)
- Windows 7 games
- Vista Applications
- Several other applications

Installation order of packages: \
Kernel Standalone -> App Compat: You can't install any other packages alongside the Kernel Standalone package. \
App Compat: This package can be installed without any other packages. \
Base -> New Dlls -> D3D -> API-SET -> App Compat -> Kernel \
App Compat -> Base -> New Dlls -> D3D -> API-SET -> Kernel
