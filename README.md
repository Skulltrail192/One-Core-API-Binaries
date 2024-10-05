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
![Chrome 122 avif](https://github.com/user-attachments/assets/8e924c47-1e1e-4b3f-8af2-ce85a1a7e531)

**Edge 122**
![Edge122 avif](https://github.com/user-attachments/assets/1304149e-0a49-4d97-afd1-db255f531ae4)

**Opera 106**
![Opera106 avif](https://github.com/user-attachments/assets/858f49f3-fd9d-4733-bac3-c6f1bd8615ce)

**Firefox 122**
![Firefox122 avif](https://github.com/user-attachments/assets/9e2da289-8b9f-47f0-a4dd-6d612a3bcfec)

**Basilisk**
![image_2022_04_08T21_38_17_976Z avif](https://github.com/user-attachments/assets/1c4a8514-b1d9-4bb0-a8b8-748b9b29ed0a)

**Vivaldi**
![vivaldi avif](https://github.com/user-attachments/assets/67f94258-8678-483e-bff3-c540fa70d641)

**Spotify (For Windows 7)**
![Spotify-Windows7 avif](https://github.com/user-attachments/assets/8462bad0-2b7a-4317-afac-d4e14e206185)

**Visual Studio Code 1.81**
![VisualCode avif](https://github.com/user-attachments/assets/6d940e51-9da0-47e6-b54f-661f6f0aede7)

**Microsoft Chess 3d**
![Chess3d avif](https://github.com/user-attachments/assets/b809078a-90fd-4893-a9f1-bbe620ea8351)

**Telegram 4.14**
![Telegram-Desktop avif](https://github.com/user-attachments/assets/59a6ca62-5d33-4cfe-a81a-082acd085ede)

**Libre Office 24 (latest)**
![LibreOffice avif](https://github.com/user-attachments/assets/23b1f25d-b50b-4f1e-a772-cfcc6356ab7a)
![Writer-LibreOffice avif](https://github.com/user-attachments/assets/41a07593-c523-4f27-8a02-96a29f4f7bcb)

**Discord 0.309**
![Discord-Login avif](https://github.com/user-attachments/assets/eeabd262-0ac4-4343-8a8a-3de485b3a324)
![Discord avif](https://github.com/user-attachments/assets/c0c31aa6-db58-44fe-bc93-6a9de9a44ba0)

**Telegram Desktop**
![Telegram-Desktop avif](https://github.com/user-attachments/assets/59a6ca62-5d33-4cfe-a81a-082acd085ede)

**Zoom meeting**
![Zoom meeting avif](https://github.com/user-attachments/assets/792b6ced-ff3f-4317-b8b0-ad8a46c3d076)

**Java 11**
![Capturar avif](https://github.com/user-attachments/assets/ce3e549d-127f-4628-a804-c0fc5af31885)

**Avast and Chromium 68**
![Avast avif](https://github.com/user-attachments/assets/b5c7f1e4-2aef-4204-b5f4-4b18d16c8fb2)

**Windows 7 Sticky Notes**
![StickyNotes avif](https://github.com/user-attachments/assets/bea5c7bc-3515-4913-9b7d-9a2b117b6822)

**Windows 7 Paint**
![Paint avif](https://github.com/user-attachments/assets/f1f9f45b-ba54-4b03-92d3-7bda8d69ceeb)

**Windows 7 Wordpad**
![Wordpad avif](https://github.com/user-attachments/assets/c364d244-4101-4312-8b24-8292ca84f706)
