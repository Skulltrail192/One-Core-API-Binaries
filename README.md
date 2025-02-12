<div align="center">
  <a href="https://github.com/One-Core-API/One-Core-API/releases">
    <img src="Assets/banner.png" width="100%" height="auto" alt="Banner">
  </a>
  <h6>Huge Thanks to @pashtetusss777 for the banner</h6>
</div>

***
**Language:**    
[English](README.md) | [简体中文](README_CN.md) | [Русский](README_RU.md) | [Українська](README_UK.md) | [日本語](README_JP.md) | [Português-Brasil](README_BR.md)
***

**This repositry contains binary releases of the One-Core-API project. These are compatible with Windows Server 2003 RTM, SP1 and SP2, Windows XP RTM, SP1, SP2 and SP3 and Windows XP
x64 SP1/SP2. However, it is highly recommended to use the system with its latest Service Pack update and all available updates.**

<!-- **Official Discord Server**: <h2>https://discord.gg/eRcGuFtn6p</h2> -->

- [Main Features](#main-features)
  - [Before Using This Software](#before-using-this-software)
- [How to install One-Core-API?](#how-to-install-one-core-api)
- [How to uninstall One-Core-API?](#how-to-uninstall-one-core-api)
- [Application Compatibility](#application-compatibility)
- [Known Limitations](#known-limitations)
- [Before submitting an issue...](#before-submitting-an-issue)
- [Repository Structure](#repository-structure)
- [Extra Information and Links](#extra-information-and-links)
  - [Official Discord Server](#official-discord-server)
- [Showcase / Proof of Concept](#showcase--proof-of-concept)


<!-- **Main information and guid to report of issue and mainly, BSOD (Blue Screen of the Death)**
   
- PC configuration: Is VM or Real? What is the processor, ram installed. Is AHCI, IDE, NVME or SCSI?
- Windows Configuration: What is the edition? Is a custom ISO/build? What installed the updates? What is the service pack? What are the installed programs?
- What is the iso used? Always provide the link;
- Steps to reproduce the BSOD. Ex: i installed OCA base, with XP Integral Edition with all options installed/seleted. Or: i installed Avast, or some other Antivirus, then i installed OCA base;
- Please, enable the complete dump of memory and upload to some drive.
- Take a picture of the BSOD. If restart automatically, press F8 on windows start, and select "Disable automatic restart" -->
<!-- **Folders in this repository:** -->

<!-- **The One-Core-API Binaries project consists of the following packages:**
Warning: Always if OCA package require restart, do it. If you install all packages and restart only on the last, Windows will be corrupted.  
- **Pack Installer**: Main package of One-Core-API and is required by One-Core-API extras; -->

<!-- **Installation order of packages:**
- **Common order**: just run One-Core-api-Pack.exe for your current platform: x86 or x64; -->

## Main Features
- **Increase memory support for 128 GB for x86 and 2 TB for x64 by default;**
- **Allow support for run newer programs designed for modern Windows OS;**
- **Allow support for new hardware with new drivers controllers;**
- **Multi-language support for all Windows XP and Windows Server 2003 supported languages;**

### Before Using This Software
> This software utilizes modified files from the respective systems, and also includes files that are still in the testing or experimental phase, and <b>is developed by a single person</b>. Consequently, predicting all possible scenarios across various computer configurations or virtual machines is not feasible. 
> 
> <h4>The transition between Windows XP/2003 and Vista marked a significant leap in the development of new APIs, technologies, and modifications to existing APIs. This makes it challenging to achieve the same level of compatibility between NT 5.x and NT 6.x systems.</h4>
> 
> I kindly ask you to remain calm and cautious. 
> Before concluding that the software is flawed or "of poor quality," <b>please report any issues you encounter through the Github Issues or [Discord server](https://discord.gg/eRcGuFtn6p)</b>.
> 
> Please note that I cannot guarantee that all issues will be resolved. However, I will analyze them and make every effort to correct them. 
> 
> Your help is valuable, and complaints or negative feedback do not contribute to the improvement of the product.


## How to install One-Core-API?

The One-Core-API uses the hotfix installation technology used up to Windows NT 5.x. In this way, you will notice that it is very similar to the installation of a Service Pack. You should go to the [Releases](https://github.com/Shorthorn-project/One-Core-API-Binaries/releases) section, choose the version you want to test and download the zipped file (.zip). After downloading, unzip the file and you will see that there are two folders, one for each supported platform: x64 and x86. Inside each folder, there will be an executable file with the name: One-Core-API-Pack.exe. Double-click on this file and follow the installation steps, which are basically: "Next, select accept the license, Next and finish". It's that simple.

**Download:**

![image](https://github.com/user-attachments/assets/09322142-2655-47d2-9723-26fe6fb67494)

**Extract:**
![image](https://github.com/user-attachments/assets/7fbba140-5a87-45b3-bec0-a5236a676b04)

**Open the extracted content and choose your architecture. If the banner haven't x64, it is x86**
![image](https://github.com/user-attachments/assets/6bdd8a39-9aac-4ee1-88fd-9fda4db144ea)

**Double click on executable:**
![image](https://github.com/user-attachments/assets/e5e03ff2-4de5-475a-bbd8-755df687b187)

**Next:**
![image](https://github.com/user-attachments/assets/2fd62bd9-b1a8-4e1d-8769-92b9bbcf2a6b)

**Accept and Next:**
![image](https://github.com/user-attachments/assets/ca62c9a2-9995-45cd-929e-b7613f9b389e)

**Wait for:**
![image](https://github.com/user-attachments/assets/06b6fa4c-67dd-4149-9b97-bdee52c60bdb)

**Finish and enjoy!**
![image](https://github.com/user-attachments/assets/8210f667-5f51-4d36-a4a5-7b5a4f24b278)


## How to uninstall One-Core-API?

As mentioned, One-Core-API uses the Hotfix installation technology of the Windows NT family up to version 5.x. Therefore, to uninstall it, you must go to the Control Panel -> Add/Remove Programs and select the "Show updates" option. After selecting this option, several updates will be displayed (if installed), including One-Core-API. Scroll down the window until One-Core-API is displayed, and click on it. An "Remove" button will appear. Click on the button and follow the uninstallation steps (basically, Next->Finish)

**Go to Control Panel:**
![image](https://github.com/user-attachments/assets/ceaf9dc2-135c-4f6b-8b22-ce5eb3f8d421)

**Click on Add Or Remove Programs:**
![image](https://github.com/user-attachments/assets/b0d6406a-db6b-4ca8-b2b9-cc020df17950)

**Mark "Show Updates:"**
![image](https://github.com/user-attachments/assets/83bdef02-9704-4e77-a0b0-cba70a4de80a)

**The update will show up. Scroll down until show One-Core-API:**
![image](https://github.com/user-attachments/assets/2ff6137f-b621-4dff-9516-b45f83c3a013)

**Click on One-Core-API Pack and then, Remove button:**
![image](https://github.com/user-attachments/assets/c66909ae-2e3a-4871-a320-e60c66210db9)

**The uninstall Windows will show up. Click in Next:**
![image](https://github.com/user-attachments/assets/71343989-6e16-48b4-982d-173a4b15774d)

**Wait for:**
![image](https://github.com/user-attachments/assets/01401f4f-e4be-4e8a-82d2-3480f143fedd)

**Click in finish and then the Windows will restart:**
![image](https://github.com/user-attachments/assets/b6f06465-786f-4503-b71b-30e9224ad9fc)

## Application Compatibility
- JetBrains IDE up to the latest releases (2024);
- Android Studio up to the latest version;
- NetBeans up to the latest version;
- Eclipse IDE up to the latest version;
- Adobe Products (Photoshop, Illustrator, Dreamweaver, etc.) up to the 2019 version;
- Filezilla (latest version);
- LibreOffice 24.0.x (latest version);
- Discord 0.309.0;
- Legocord (Discord fork) latest;
- Visual Studio 2012 and Visual Studio 2013;
- Visual Studio Code (and forks like Codium) up to the latest version;
- Chromium browsers (Chrome, Opera, Edge, and others) up to the latest version;
- Gecko based (Firefox, Zen Browser) up to the latest versions (however, YouTube only works up to Firefox version 130);
- Seamonkey up to version 2.53.10;
- Thunderbird up to the latest version;
- Maxthon up to version 7.1.6;
- Vivaldi up to the latest version;
- JDK 1.8 (Windows XP x64 only for now);
- Java JDK and alternative JDK or OpenJDK up to version 24 (maybe other versions work as well). You can download from: https://bell-sw.com/pages/downloads/#/java-11-lts;
- Epic Browser 120;
- Python 3.6 (3.8/3.9 may also work, only the [mod](https://mega.nz/folder/KxExlAiC#L9rAQ5kTCtlHgZUwaxMpgw) version);
- .Net Framework up to 4.8;
- .NET 6.0
- Geekbench 4.2;
- Performance Test;
- Adobe Reader DC (up to 2024);
- Foxit PDF Reader (2023);
- Windows 7 games;
- Windows 7 Paint;
- Windows 7 Wordpad;
- Native applications from Windows Vista;
- Spotify for Windows 7 and for Windows 10;
- Line;
- Zoom;
- Node 10.24;
- Telegram Desktop;
- Winrar 7.0 (latest);
- Postman;
- Insomnia;
- Dbeaver;
- TeamViewer 14;
- Directx 9EX, 10, and 11 games:
  - Need for Speed Most Wanted 2012;
  - Need for Speed The Run;
  - Street Fighter V;
  - Injustice: Gods Among Us;
  - Assassin's Creed Black Flag;
  - Crysis 1, 2, and 3 (directx 10-11 mode);
  - GTA Trilogy Definitive Edition;
  - GTA V;
  - Minecraft 1.21.x
  - Resident Evil 5 dx10 mode;
  - Lost Planet;
  - Far Cry 4;
  - Far Cry Primal;
  - Tropico 5;
  - Metro Last Night;
  - Cuphead;
  - Horizon Turbo;
- Kate 23.08.1
  
## Known Limitations
- Youtube on Firefox Starting version 131 not work. So, we recommend use version 115 or 128 ESR.
- Some applications installers may not work, like some electron based apps (MS teams), Office 2013 or GIMP 3.0 RC2 etc crash and application isn't installed. Some installers and applications require AVX support by OS, what isn't supported at no. The, use a pre-installed version, copied from other operating system;
- The packages cannot be integrate with nlite to Windows ISO, because use a tool called "SFXCAB Substitute", not standard Microsoft version;
- Standard .Net Framework Installers starting 4.6 ins't supported for now. You need a repacked version like did here: https://github.com/abbodi1406/dotNetFx4xW7. And avaliable like here: https://www.wincert.net/forum/topic/13805-microsoft-net-framework-472-full-x86x64-incl-language-packs-by-ricktendo/#comment-123251. Other versions are avaliable too, search on forum topics;
- New versions of palemoon may face side by side configuration error issue.;
- Opera 39 - 50 may need follow parameters to start: --disable-gpu (to prevent black screen) and --single-process(to prevent eternally first page loading);

## Before submitting an issue...

**Before reporting an issue, I strongly recommend checking if the problem exists in [One-Core-API-Canary](https://github.com/shorthorn-project/One-Core-API-Binaries-Canary) and carefully reviewing the existing [Issues](https://github.com/Skulltrail192/One-Core-API-Binaries/issues) to see if your problem has already been reported.**

**If the problem *does* exist in Canary and is not already listed in the current Issues, please provide a detailed report of your issue, including the following information:**

**1. System Configuration:**
*   **Type** (Physical Hardware/VM)
*   **Windows OS Edition** (e.g., Windows XP Professional Service Pack 3)
*   **Post-SP Updates:**
    *  Were any Post-Service Pack updates installed? (Yes/No)
       *  If yes, specify if they were installed **before** or **after** installing One-Core-API.
*    **Installed Software:** List any relevant software that might be related to the issue (e.g., Adobe Photoshop CC 2018, Firefox 132, etc.).
*   **Technical Specifications**:
    *   **If Physical Hardware:**  Provide details on the processor, RAM (type, amount), hard drive type, and capacity (e.g., IDE disk, 120 GB).
    *   **If a Virtual Machine:** Specify the virtual machine configuration details (e.g., Oracle VirtualBox 6.1.0, 2 GB RAM, 120 GB disk, AHCI mode).
*   **Steps to reproduce the error**


> **IMPORTANT:** If possible, please also **attach any relevant logs** to the issue. This will greatly assist in identifying a solution faster.

>  It is also advisable to attach a video recording of the problem, if feasible.
>
> **Please note that the issue will be closed if the problem is not reproducible.**

## Repository Structure
- Documents: Project documentation, known bugs, sfxcab usage (for making installers), etc.
- Packages\x86 and Packages\x64: Binary releases categorized by package. You can download and install/update the packages directly from here (i.e. by going to Packages\x86\Base installer\update and runnning update.exe).
- Todo: To do tasks
- Test: Some binaries and documents for testing;
- Release: Scripts to generate a new binaries release;
- Output: Binaries output, you can generate using scripts on Release folder;

## Extra Information and Links
**Extended Features for One-Core-API based Systems:**

<b><a href="https://github.com/shorthorn-project/One-Core-API-Extras" style="font-size: 18px">https://github.com/shorthorn-project/One-Core-API-Extras</a></b>

**Tools for new Deployment System for One-Core-API based Systems:**

<b><a href="https://github.com/Skulltrail192/One-Core-API-Tools" style="font-size: 18px">https://github.com/Skulltrail192/One-Core-API-Tools</a></b>

### Official Discord Server

**If you want to join our Official Discord Server of One-Core-API, you can join here:**

<b><a href="https://discord.gg/eRcGuFtn6p" style="font-size: 25px">https://discord.gg/eRcGuFtn6p</a></n>

## Showcase / Proof of Concept
Some screenshots of Applicaions running on XP/Server 2003:

**Minecraft 1.21**
![image](https://github.com/user-attachments/assets/cfd05f13-617e-49a0-b416-67906d42840b)

**Chrome 132**
![image](https://github.com/user-attachments/assets/84e83d53-ea8e-47b9-a566-e0986c91b812)

**Edge 134 (Dev Preview)**
![image](https://github.com/user-attachments/assets/f0b6a47c-dc37-45b0-beaf-c85002e37386)

**Opera 116**
![image](https://github.com/user-attachments/assets/ee962193-8de6-458e-8d35-769638e9fbde)

**Firefox 122**
![Firefox122](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/db647daf-0960-4ace-ad2f-63469dbf3881)

**Thunderbird 132**
![image](https://github.com/user-attachments/assets/1ccdd59f-849a-4f1c-86e0-bcc9e1ce02e2)

**Basilisk**
![image_2022_04_08T21_38_17_976Z](https://user-images.githubusercontent.com/5159776/178077859-079bfca4-bdb6-402e-8991-b88e7dfe387c.png)

**Vivaldi**
![image](https://github.com/user-attachments/assets/580966ab-f170-42a9-9f9d-3c15fe2ec8b2)


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
