**Welcome to One-Core-API binaries!**

This repositry contains binary releases of the One-Core-API project. These are compatible with Windows Server 2003 SP2, Windows XP SP3 and Windows XP
x64 SP2.

**Folders in this repository:**
- Documents: Project documentation, known bugs, sfxcab usage (for making installers), etc.
- Packages\x86 and Packages\x64: Binary releases categorized by package. You can download and install/update the packages directly from here (i.e. by going to Packages\x86\Base installer\update and runnning update.exe).
- Todo:  To do tasks
- Test: Some binaries and documents for testing.

**The One-Core-API Binaries project consists of the following packages:**
- Base: One-Core-API base. This package is required by all other packages, except Kernel Standalone, and contains all wrappers used in this project (like kernelbase and ntext);
- Additional Dlls: Several new dlls introduced in later versions of Windows.
- API-SET: Contains UCRT dlls based on wine.
- D3d: D3D runtime (Mainly DX10 and DX11, based on WineD3D);
- Kernel Standalone: Experimental kernel update. Does not depend on the Base package. This package cannot be installed alongside any other packages, except App Compat.
- App Compat: Application compatbility settings, backported from later versions of Windows.

**One-Core-API allows you to run:**
- Web Storm 2018;
- Intelliji 2018 (Other versions may work too);
- Filezilla (latest);
- Visual Studio Code 1.18;
- Chrome up to version 102!;
- Opera up to version 39;
- Firefox up to version 54;
- JDK 1.8;
- Maxthon 6;
- Epic Browser 94;
- Python 3.6;
- .Net Framework up to 4.8;
- Geekbench 4.2;
- Performance Test;
- Adobe Reader DC (2017);
- Windows 7 games;
- Vista native Applications;
- Spotify after Windows XP/Vista end of support, version 1.0.60;
- Yandex browser (latest);
- Zoom;
- Several other applications;
- Directx 9EX, 10 and 11 games: 
  - Need for Speed Most Wanted 2012;
  - Need for Speed The Run;
  - Street Figther V;
  - Injustice: Gods among us;
  - Assassign Creed Black Flgs;
  - Crysis 1,2, and 3 (directx 10-11 mode);
  
**Known Limitations:**
- The unique language on binaries for now is English. You can install on Windows with another language, however, several dialogs and strings will be displayed in engilsh;
- New applications installers may not work, like Chrome, Maxthon, Utorrent, Discord, Team vieweretc crash and application isn't installed. Need 
use a pre-installed version, copied from other operation system;
- Some installers like VS Code and Skype genereate a erro "Class is not supported" on shortcut phase creation;
- Chrome has stability and crash randomy. Extension isn't supported for now. Downloads crash application;
- Firefox 55 or above is working, only mypal 68;
- Firefox 54 32 bits not working on Windows XP x64. Chrome 61+ not working too;
- The packages cannot be integrate with nlite to Windows Iso, because use a tool called "SFXCAB Substitute", not standard Microsoft version;
- Standard .Net Framework Installers starting 4.6 ins't supported for now. You need a repacked version like did here: https://github.com/abbodi1406/dotNetFx4xW7. And avaliable like here: https://www.wincert.net/forum/topic/13805-microsoft-net-framework-472-full-x86x64-incl-language-packs-by-ricktendo/#comment-123251. Other versions are avaliable too, search on forum topics;

**Installation order of packages:**
- **App Compat only:** This package can be installed without any other packages;

- **Base first**: Base-> D3d -> API-SET -> Driver Update -> App Compat-> Additional Dlls; 

- **AppCompat first**: App Compat -> Base -> Driver Update -> D3d -> API-SET -> Additional Dlls.
