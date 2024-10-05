**Welcome to One-Core-API binaries!**
***
**Language:**    
[English](README.md) | [简体中文](README_CN.md) | [Русский](README_RU.md) | [Українська](README_UK.md) | [日本語](README_JP.md) | [Português-Brasil](README_BR.md)
***

This repositry contains binary releases of the One-Core-API project. These are compatible with Windows Server 2003 SP2, Windows XP SP3 and Windows XP
x64 SP2.

**Official Discord Server**: https://discord.gg/eRcGuFtn6p

**WARNING**
This is software that uses modified files from the respective systems, contains other files still in the testing or experimental phase and has only one developer. In other words, it is impossible to predict all possible scenarios on all possible types of computers or virtual machines. Between XP/2003 and Vista there was the biggest leap in new APIs, new technologies and modifications to existing APIs, so it is very difficult to have the same level of compatibility in NT5 as in NT6. Be calm, be prudent and before saying that this software is bad or "crap", report the defect in the issues and as soon as possible, it will be analyzed and I will try to correct the problem. Help me, complaining or defaming the software doesn't do anyone any good.

**Experimental One-Core-API versions**:
https://github.com/shorthorn-project/One-Core-API-Binaries-Canary

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
- JetBrains Applications until 2021.3;
- JetBrains WebStorm 2021;
- Intellij 2021 (Other versions may work too);
- Adobe Photoshop CC 2018;
- Filezilla (latest);
- LibreOffice 24.0.x (latest);
- Discord 0.309.0
- Visual Studio 2012;
- Visual Studio Code up to 1.83.1;
- Codium up to 1.89.x;
- Chrome up to version 129;
- Opera up to version 106;
- Firefox up to version 130 beta (with some bugs);
- Microsoft Edge up to version 128;
- Brave up to version 1.x (Chromium 129);
- Yandex Browser latest version;
- Thorium browser up to 109;
- Supermium up to version 124;
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
- Foxit PDF Reader (2023);
- Windows 7 games;
- Windows 7 Paint;
- Windows 7 Wordpad;
- Windows Vista native Applications;
- Spotify Windows 7;
- Yandex browser (latest);
- Zoom;
- Several other applications;
- Node 10.24;
- Telegram Desktop;
- Winrar 7.0 Beta 4 (latest);
- Postman;
- Insominia;
- Eclise IDE;
- Dbeaver;
- Directx 9EX, 10 and 11 games: 
  - Need for Speed Most Wanted 2012;
  - Need for Speed The Run;
  - Street Figther V;
  - Injustice: Gods among us;
  - Assassign Creed Black Flag;
  - Crysis 1,2, and 3 (directx 10-11 mode);
- Kate 23.08.1 (Windows XP x64 only for now)
  
**Known Limitations:**
- Running Firefox 56+ you may face page loading problems. To resolve this, from version 55 to 72, you must create an environment variable called MOZ_FORCE_DISABLE_E10S . It will be the key of the user's environment variable, and the value will be 1. For versions from 73 onwards, the value will be the version number, that is, 73.0.0, for example (check the version number).
- Some places in dialogs or windows keep only in english. Internacionalization is working in progress. Some issues with installed mui packs;
- New applications installers may not work, like Chrome, Maxthon, Discord, Team viewer, etc crash and application isn't installed. Need 
use a pre-installed version, copied from other operating system;
- The packages cannot be integrate with nlite to Windows Iso, because use a tool called "SFXCAB Substitute", not standard Microsoft version;
- Standard .Net Framework Installers starting 4.6 ins't supported for now. You need a repacked version like did here: https://github.com/abbodi1406/dotNetFx4xW7. And avaliable like here: https://www.wincert.net/forum/topic/13805-microsoft-net-framework-472-full-x86x64-incl-language-packs-by-ricktendo/#comment-123251. Other versions are avaliable too, search on forum topics;
- New versions of palemon may face side by side configuration error issue.;
- Opera 39 - 50 may need follow parameters to start: --disable-gpu (to prevent black screen) and --single-process(to prevent eternally first page loading);

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
![Chrome 122 avif](https://github.com/user-attachments/assets/af3f8957-46f4-44d9-b220-4b99b325a62c)

**Edge 122**
![Edge122 avif](https://github.com/user-attachments/assets/5fe1f161-f280-43f3-b4a1-42a31de7724f)

**Opera 106**
![Opera106 avif](https://github.com/user-attachments/assets/4bf39b47-5fff-47fa-a1c7-8ac3d699db90)

**Firefox 122**
![Firefox122 avif](https://github.com/user-attachments/assets/75fbd338-450e-4750-8d0b-2ae60af4137f)

**Basilisk**
![image_2022_04_08T21_38_17_976Z avif](https://github.com/user-attachments/assets/af02d241-8261-4ae4-85f2-a66b5e640508)

**Vivaldi**
![vivaldi avif](https://github.com/user-attachments/assets/67f94258-8678-483e-bff3-c540fa70d641)

**Spotify (For Windows 7)**
![Spotify-Windows7 avif](https://github.com/user-attachments/assets/9ad0f15c-2e64-49dd-9aaf-2e60ad7aaefb)

**Visual Studio Code 1.81**
![VisualCode avif](https://github.com/user-attachments/assets/de358b56-f736-4f61-b1d0-6b0d25002ff8)

**Microsoft Chess 3d**
![Chess3d avif](https://github.com/user-attachments/assets/4b8770be-7a28-4c13-9b40-dd7ab779063d)

**Telegram 4.14**
![Telegram-Desktop avif](https://github.com/user-attachments/assets/a9203e1a-147d-4160-a5bd-4c42337ec7c2)

**Libre Office 24 (latest)**
![LibreOffice avif](https://github.com/user-attachments/assets/f318b833-7362-47d7-94db-f34f33c3ff29)
![Writer-LibreOffice avif](https://github.com/user-attachments/assets/2af88351-eaa2-4b46-895c-b5073141a386)

**Discord 0.309**
![Discord-Login avif](https://github.com/user-attachments/assets/c8f6b4ff-93b8-4b5a-a7db-e4332a2289c2)
![Discord avif](https://github.com/user-attachments/assets/bcb4288c-5c85-4e95-a3ca-ad414136bd2f)

**Telegram Desktop**
![Telegram-Desktop avif](https://github.com/user-attachments/assets/a9203e1a-147d-4160-a5bd-4c42337ec7c2)

**Zoom meeting**
![Zoom meeting avif](https://github.com/user-attachments/assets/dac5c8be-ca16-4e47-ac4a-b736f66d57aa)

**Java 11**
![Capturar avif](https://github.com/user-attachments/assets/b12fd34f-9ecf-4506-98a8-bf9fd572ca8b)

**Avast and Chromium 68**
![Avast avif](https://github.com/user-attachments/assets/70aac117-0159-4c73-9d7d-cc65ee8b6265)

**Windows 7 Sticky Notes**
![StickyNotes avif](https://github.com/user-attachments/assets/70566c6e-41c4-49a5-a7d8-e54a03b3e0ad)

**Windows 7 Paint**
![Paint avif](https://github.com/user-attachments/assets/ae2a13e1-9b6c-481e-81d0-2caf4a5194c5)

**Windows 7 Wordpad**
![Wordpad avif](https://github.com/user-attachments/assets/1eb8246f-0b29-4a6b-b2b3-fd8e33e4cd4a)
