**Welcome to One-Core-API binaries!**

This repositry contains binary releases of the One-Core-API project. These are compatible with Windows Server 2003 SP2, Windows XP SP3 and Windows XP
x64 SP2.

**Official Discord Server**: https://discord.gg/eRcGuFtn6p

**Folders in this repository:**
- Documents: Project documentation, known bugs, sfxcab usage (for making installers), etc.
- Packages\x86 and Packages\x64: Binary releases categorized by package. You can download and install/update the packages directly from here (i.e. by going to Packages\x86\Base installer\update and runnning update.exe).
- Todo: To do tasks
- Test: Some binaries and documents for testing;
- Release: Scripts to generate a new binaries release;
- Output: Binaries output, you can generate using scripts on Release folder;

**The One-Core-API Binaries project consists of the following packages:**
Warning: Always if OCA package require restart, do it. If you install all packages and restart only on last, Windows will be corrupted.  
- **Base**: Main package of One-Core-API and is required by all other packages, except App Compat, Modern Setup (x86 only) and Driver Update, and contains all wrappers used in this project (like kernelbase and ntext);
- **Additional Dlls**: Several new dlls introduced in later versions of Windows.
- **API-SET**: Contains UCRT dlls based on wine.
- **D3d**: D3D runtime (Mainly DX10 and DX11, based on WineD3D);
- **App Compat**: Application compatbility settings, backported from later versions of Windows.
- **Driver Update**: Bring updated acpi driver what support ACPI 2.0, new drivers like Storachi (for AHCI driver controllers), NVME (for NVME M.2 driver controllers) and USBXHCI (USB 3.0); 
- **Branding**: New branding system introduced on Windows Vista, required to D3D and Modern Setup packages;
- **Modern Setup**: bring support to turn Windows XP/2003 installation installable using Windows Vista or above Setup engines on newers computers. This package is only if you install Windows on VM or real computer and want install to other computer, using a tool called SYSPREP. Don't need install if you don't want do it;

**Installation order of packages:**
- **Common order**: Base-> Additional Dlls -> API-SET -> Branding -> D3d -> Modern Setup -> Driver Update -> App Compat;

- **Modern Setup**:
  - **With others packages together**: Branding -> Modern Setup -> Base-> API-SET -> D3d -> Driver Update -> App Compat-> Additional Dlls;
  - **Without other packages together, only for install on newer computer(Also Recommended install Driver Update too)**:
    - **x86**: Branding -> Modern Setup;
    - **x64**: Base -> Branding -> Modern Setup;

- **Standalone packages (don't require other packages together, only if you want)**
  - **App Compat only:** This package can be installed without any other packages; 
  - **Driver Update Only**: This package can be installed without any other packages; 
  - **AppCompat first**: This package can be installed without any other packages. 

**Main Features**:
- Increase memory support for 128 GB for x86 and 2 TB for x64 by default;
- Allow support for run newer programs designed for modern Windows OS;
- Allow support for new hardware with new drivers controllers;
- Allow support for install Windows on any computers, with generalized hardware;

**Modern Setup - Detailed**
This is a package designed to prepare Windows to get ability to be installed in others computers. If you don't want capture the Windows installation, don't need install this package.
- After install One-Core-API Modern Setup and restart computer, you can run Sysprep, a tool to prepare Windows to be generalize Windows Installation to any hardware;
- Sysprep is placed on: Windows\System32\Sysprep, like as Windows Vista have;
- The tools is very similar what is present on Vista. For same behavior on prepare XP/2003 what is presented on Vista or above systems to install on new computers, you must run sysprep.exe and select Action to "Enter System Out-of-Box Experience (OOBE)". Is recommeded check "Generalize" too. After choose Action and mark, you can choose on Shutdown Options if only quit program, shutdown or reboot system. On press "Ok", sysprep will run cleanup plugins and do the shutdown option selected;
- After shutdown system, on the next boot, will run setup.exe placed on Windows\System32\oobe\setup.exe and will detect hardware and after detection the hardware, will restart and enter on msoobe;
- For patch Windows Vista - Win11 Setup and capture a Windows XP/2003/Longhorn/Reactos Image, use this tools:
  https://github.com/Skulltrail192/One-Core-API-Tools

![Sysprep](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/615ada04-a036-43c4-ac54-824cade0b5c2)

**One-Core-API allows you to run:**
- Web Storm 2018;
- Intelliji 2018 (Other versions may work too);
- Filezilla (latest);
- Visual Studio Code 1.81.1;
- Chrome up to version 109!;
- Opera up to version 95;
- Firefox up to version 70~80 (with bugs and need workarounds);
- Microsoft Edge up version 109;
- Brave up version 117;
- Yandex latest version;
- Thorium browser up 109;
- Supermium up version 117;
- Seamonkey up version 2.53.10;
- Thunderbird up version 70x;
- Maxthon up version 7.1.6;
- Vivaldi up latest version;
- JDK 1.8 (only Windows XP x64 for now);
- Java Alternative JDK or OpenJDK until version 21 (maybe others versions works). You can download from: https://bell-sw.com/pages/downloads/#/java-11-lts;
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
- Node 10.24;
- Winrar 6.11 (latest);
- Directx 9EX, 10 and 11 games: 
  - Need for Speed Most Wanted 2012;
  - Need for Speed The Run;
  - Street Figther V;
  - Injustice: Gods among us;
  - Assassign Creed Black Flgs;
  - Crysis 1,2, and 3 (directx 10-11 mode);
  
**Known Limitations:**
- Open/Save Dialogs from several applications not working fully because are from Vista implementation, called IFileDialog. Working in progress;
- Some places in dialogs or windows keep only in english. Internacionalization is working in progress;
- New applications installers may not work, like Chrome, Maxthon, Discord, Team viewer, etc crash and application isn't installed. Need 
use a pre-installed version, copied from other operation system;
- Some installers like VS Code and Skype genereate a erro "Class is not supported" on shortcut phase creation;
- Chrome has stability and crash randomy. Extension isn't supported for now. Downloads crash application or has network error, depend of version;
- Firefox 55 -78 working with some limitations;
- Firefox 54 32 bits not working on Windows XP x64. Chrome 61+ not working too;
- The packages cannot be integrate with nlite to Windows Iso, because use a tool called "SFXCAB Substitute", not standard Microsoft version;
- Standard .Net Framework Installers starting 4.6 ins't supported for now. You need a repacked version like did here: https://github.com/abbodi1406/dotNetFx4xW7. And avaliable like here: https://www.wincert.net/forum/topic/13805-microsoft-net-framework-472-full-x86x64-incl-language-packs-by-ricktendo/#comment-123251. Other versions are avaliable too, search on forum topics;
- Palemoon version 27-31 not download anything. Download not start;
- Opera 39 - 46 may need follow parameters to start: --disable-gpu (to prevent black screen) and --single-process(to prevent eternally first page loading);
- Qt5 application what use Dwrite crash on startup. Has some glitches like black screen if you delete dwrite (application start and can be used, however)

**Issues Report**
For help to reproduce issue, is recommend **always** follow this template:
- Description whats happened.
  Ex: Always get BSOD when try load Windows. Windows stuck on Boot Screen. Windows always get a Black screen, etc;
- VM/PC config
  Ex: Vmware 10, virtualBox 6.1.0 or Core 2 Duo 8400, 2Gb DDR2, IDE/SATA Hard Disk;
- Windows Edition and configuration
  Ex: Windows XP Service Pack 3 with POSReady 2009 updates installed, with this program list: Adobe, office, etc;

Some screenshots of Applicaions running on XP/Server 2003:
**Chrome 58**
![image_2022_04_07T03_35_23_052Z](https://user-images.githubusercontent.com/5159776/178077754-de45d085-7e32-4080-b577-29f67a777076.png)
**Advanced Chrome 64**
![image_2022_04_07T03_42_36_918Z](https://user-images.githubusercontent.com/5159776/178077817-e58fd872-f0fb-431d-aaad-c4a15510ed87.png)
**Basilisk**
![image_2022_04_08T21_38_17_976Z](https://user-images.githubusercontent.com/5159776/178077859-079bfca4-bdb6-402e-8991-b88e7dfe387c.png)
Firefox Dev 54**
![image_2022_04_08T14_03_12_014Z](https://user-images.githubusercontent.com/5159776/178077897-676267bd-31c2-451d-8d81-951c0223bac4.png)
**Visual Code 1.22**
![image_2022_04_07T03_54_26_696Z](https://user-images.githubusercontent.com/5159776/178077980-31788372-84e3-43b9-8bd3-d3204a375197.png)
**Chedot (chrome fork) 86
![image_2022_04_07T22_34_04_707Z](https://user-images.githubusercontent.com/5159776/178078013-9ccc115e-f6f6-44d0-937f-1a73fa5c6dee.png)
**Java 11**
![Capturar](https://user-images.githubusercontent.com/5159776/178078132-da504607-a1ca-4f8d-ae25-6a7eb367bdaa.PNG)
**Avast and Chromium 68**
![Avast](https://user-images.githubusercontent.com/5159776/178078208-c13b3448-ee6a-4c56-9d94-d0c62d51949e.PNG)
**Visual Code 1.70**
![VisualCode1 70](https://user-images.githubusercontent.com/5159776/192194220-9f4f324d-b0d8-4c40-a378-2c25c81eff16.PNG)
Windows 7 Paint
![Windows7 Paint](https://user-images.githubusercontent.com/5159776/192194273-de70c23e-8731-4fb6-96c1-9bee98947269.PNG)



