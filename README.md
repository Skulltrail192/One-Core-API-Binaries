**Welcome to One-Core-API binaries!**
***
**Language:**    
[English](README.md) | [简体中文](README_CN.md) | [Русский](README_RU.md) | [Українська](README_UK.md) | [日本語](README_JP.md) | [Português-Brasil](README_BR.md)
***

This repositry contains binary releases of the One-Core-API project. These are compatible with Windows Server 2003 SP2, Windows XP SP3 and Windows XP
x64 SP2.

**Official Discord Server**: https://discord.gg/eRcGuFtn6p

**NOTE**:
-Windows XP Unofficial SP4 and maybe Integral Edition are not supported!

**Language Support**
Language support for almost Windows parts for: Portuguese-Brazil (my language), Portuguese (Portugual), Turkish, Chinese (Traditional and Simplified), French, Italian, Hungarian, Ukranian (partially), Spanish, Polish, Russian and Korean;

**Folders in this repository:**
- Documents: Project documentation, known bugs, sfxcab usage (for making installers), etc.
- Packages\x86 and Packages\x64: Binary releases categorized by package. You can download and install/update the packages directly from here (i.e. by going to Packages\x86\Base installer\update and runnning update.exe).
- Todo: To do tasks
- Test: Some binaries and documents for testing;
- Release: Scripts to generate a new binaries release;
- Output: Binaries output, you can generate using scripts on Release folder;

**The One-Core-API Binaries project consists of the following packages:**
Warning: Always if OCA package require restart, do it. If you install all packages and restart only on the last, Windows will be corrupted.  
- **Base**: Main package of One-Core-API and is required by all other packages, except App Compat and Driver Update, and contains all wrappers used in this project (like kernelbase and ntext);
- **Additional Dlls**: Several new dlls introduced in later versions of Windows.
- **D3d**: D3D runtime (Mainly DX10 and DX11, based on WineD3D);
- **App Compat**: Application compatbility settings, backported from later versions of Windows.
- **Driver Update**: Bring updated acpi driver what support ACPI 2.0, new drivers like Storachi (for AHCI driver controllers), NVME (for NVME M.2 driver controllers) and USBXHCI (USB 3.0); 
- **Branding**: New branding system introduced on Windows Vista, required to D3D and Modern Setup packages;

**Installation order of packages:**
- **Common order**: Base-> Additional Dlls -> Branding -> D3d -> Driver Update -> App Compat;

- **Standalone packages (don't require other packages together, only if you want)**
  - **App Compat only:** This package can be installed without any other packages; 
  - **Driver Update Only**: This package can be installed without any other packages; 
  - **AppCompat first**: This package can be installed without any other packages. 

**Main Features**:
- Increase memory support for 128 GB for x86 and 2 TB for x64 by default;
- Allow support for run newer programs designed for modern Windows OS;
- Allow support for new hardware with new drivers controllers;

**One-Core-API allows you to run:**
- JetBrains WebStorm 2018;
- Intellij 2018 (Other versions may work too);
- JetBrains WebStorm 2023.x.x (Windows XP x64 only for now)
- JetBrains IntelliJ 2023.x.x (Windows XP x64 only for now)
- Adobe Photoshop CC 2018;
- Filezilla (latest);
- Discord 0.309.0
- Visual Studio Code up to 1.83.1;
- Chrome up to version 123;
- Opera up to version 106;
- Firefox up to version 122 (with some bugs);
- Microsoft Edge up to version 122;
- Brave up to version 1.62.153 (Chromium 121);
- Yandex Browser latest version;
- Thorium browser up to 109;
- Supermium up to version 117;
- Seamonkey up to version 2.53.10;
- Thunderbird up to version 121;
- Maxthon up to version 7.1.6;
- Vivaldi up to the latest version;
- JDK 1.8 (only Windows XP x64 for now);
- Java Alternative JDK or OpenJDK until version 21 (maybe others versions works). You can download from: https://bell-sw.com/pages/downloads/#/java-11-lts;
- Epic Browser 120;
- Python 3.6;
- .Net Framework up to 4.8;
- Geekbench 4.2;
- Performance Test;
- Adobe Reader DC (2018);
- Windows 7 games;
- Windows 7 Paint;
- Windows 7 Wordpad;
- Windows Vista native Applications;
- Spotify Windows 7 and Windows 10 version;
- Yandex browser (latest);
- Zoom;
- Several other applications;
- Node 10.24;
- Telegram Desktop;
- 
- Winrar 6.24 (latest);
- Directx 9EX, 10 and 11 games: 
  - Need for Speed Most Wanted 2012;
  - Need for Speed The Run;
  - Street Figther V;
  - Injustice: Gods among us;
  - Assassign Creed Black Flag;
  - Crysis 1,2, and 3 (directx 10-11 mode);
- Kate 23.08.1 (Windows XP x64 only for now)
  
**Known Limitations:**
- Open/Save Dialogs from several applications not working fully because are from Vista implementation, called IFileDialog. Working in progress;
- Running Firefox 56+ you may face page loading problems. To resolve this, from version 55 to 72, you must create an environment variable called MOZ_FORCE_DISABLE_E10S . It will be the key of the user's environment variable, and the value will be 1. For versions from 73 onwards, the value will be the version number, that is, 73.0.0, for example (check the version number).
- Some places in dialogs or windows keep only in english. Internacionalization is working in progress;
- New applications installers may not work, like Chrome, Maxthon, Discord, Team viewer, etc crash and application isn't installed. Need 
use a pre-installed version, copied from other operating system;
- Fixed in version 3.0.4 ~~Some installers like VS Code and Skype genereate a erro "Class is not supported" on shortcut phase creation;~~
- Fixed in OCA 3.0.3/3.0.4 ~~Chrome has stability and crash randomy. Extension isn't supported for now. Downloads crash application or has network error, depend of version;~~
- Firefox 55-116 working with some limitations;
- Firefox since version 54 (32 bit) doesn't work on Windows XP x64. Chrome 61+ does not work either;
- The packages cannot be integrate with nlite to Windows Iso, because use a tool called "SFXCAB Substitute", not standard Microsoft version;
- Standard .Net Framework Installers starting 4.6 ins't supported for now. You need a repacked version like did here: https://github.com/abbodi1406/dotNetFx4xW7. And avaliable like here: https://www.wincert.net/forum/topic/13805-microsoft-net-framework-472-full-x86x64-incl-language-packs-by-ricktendo/#comment-123251. Other versions are avaliable too, search on forum topics;
- Palemoon version 27-31 not download anything. Download not start;
- Opera 39 - 46 may need follow parameters to start: --disable-gpu (to prevent black screen) and --single-process(to prevent eternally first page loading);
- Qt5 application what use Dwrite crash on startup. Has some glitches like black screen if you deleted dwrite (application start and can be used, however)

**Issues Report**
For help to reproduce issue, is recommend **always** follow this template:
- Description whats happened.
  Ex: Always get BSOD when try load Windows. Windows stuck on Boot Screen. Windows always get a Black screen, etc;
- VM/PC config
  Ex: Vmware 10, virtualBox 6.1.0 or Core 2 Duo 8400, 2Gb DDR2, IDE/SATA Hard Disk;
- Windows Edition and configuration
  Ex: Windows XP Service Pack 3 with POSReady 2009 updates installed, with this program list: Adobe, office, etc;

Some screenshots of Applicaions running on XP/Server 2003:

**Chrome 122**
![Chrome 122](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/6442a5b0-036b-48e0-a6e8-3624825d3882)

**Edge 122**
![Edge122](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/734954f4-2540-4657-9a2d-ce6aed809bf5)

**Opera 106**
![Opera106](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/db509ccf-4e66-4e2b-ad4b-fd8512495333)

**Firefox 122**
![Firefox122](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/db647daf-0960-4ace-ad2f-63469dbf3881)

**Basilisk**
![image_2022_04_08T21_38_17_976Z](https://user-images.githubusercontent.com/5159776/178077859-079bfca4-bdb6-402e-8991-b88e7dfe387c.png)

**Vivaldi**
![vivaldi](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/86d5895f-977a-414f-b0d5-0e877a658676)

**Spotify (For Windows 7)**
![Spotify-Windows7](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/09de7c20-8670-45dc-9471-a6db9349abd0)

**Visual Studio Code 1.81**
![VisualCode](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/b21748b9-25bb-412d-95b3-2219d2efdf42)

**Microsoft Chess 3d**
![Chess3d](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/bd1ad0c6-edde-4ff2-a6e0-074c7379fab6)

**Telegram 4.14**
![Telegram-Desktop](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/73e13167-49b8-4282-81cb-969435046dde)

**Libre Office 24 (latest)**
![LibreOffice](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/11fd191d-270c-428d-8d41-0498e8fafb3b)
![Writer-LibreOffice](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/e389a39b-febd-45f6-9c6f-25f64e460142)

**Discord 0.309**
![Discord-Login](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/8a4c12b5-19fc-454d-b02a-a1db807d3900)
![Discord](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/eb673541-4e66-4c76-867e-346edbaaa0af)

**Telegram Desktop**
![Telegram-Desktop](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/d23b9add-629d-45a3-a8e1-c331271bc0d3)

**Zoom meeting**
![Zoom](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/d002cf1b-c5f4-4c0c-b629-00e031a56765)

**Java 11**
![Capturar](https://user-images.githubusercontent.com/5159776/178078132-da504607-a1ca-4f8d-ae25-6a7eb367bdaa.PNG)

**Avast and Chromium 68**
![Avast](https://user-images.githubusercontent.com/5159776/178078208-c13b3448-ee6a-4c56-9d94-d0c62d51949e.PNG)

**Windows 7 Sticky Notes**
![StickyNotes](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/669ba3e4-b831-4a96-ad40-d87e3e9531e2)

**Windows 7 Paint**
![Paint](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/81728a44-c9e7-41e8-b68b-8ea7b119ebba)

**Windows 7 Wordpad**
![Wordpad](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/9dac02c7-7139-47fe-8732-ccd9ef91090b)
