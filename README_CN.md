**欢迎使用One-Core-API二进制文件！**
***
**语言:**    
[English](README.md) | [简体中文](README_CN.md) | [Русский](README_RU.md) | [Українська](README_UK.md) | [日本語](README_JP.md)
***

该存储库包含One-Core-API项目的二进制发布版本。这些版本兼容于Windows Server 2003 SP2、Windows XP SP3和Windows XP x64 SP2。

**官方Discord服务器**: https://discord.gg/eRcGuFtn6p  

**注意**：  
- 该软件使用各个系统中的修改文件，包含仍处于测试或实验阶段的其他文件，并且只有一名开发人员。换句话说，不可能预测所有可能类型的计算机或虚拟机上的所有可能场景。 XP/2003和Vista之间，新的API、新技术以及对现有API的修改都有最大的飞跃，因此NT5很难达到与NT6相同水平的兼容性。保持冷静，谨慎，在说这个软件不好或“垃圾”之前，请尽快报告问题中的缺陷，我们将对其进行分析，并尝试纠正问题。帮助我，抱怨或诽谤该软件对任何人都没有任何好处。

**实验性One-Core-API版本**:
https://github.com/shorthorn-project/One-Core-API-Binaries-Canary

**语言支持**：  
支持所有 Windows XP 和 Windows Server 2003 支持的语言。

**此存储库中的文件夹:**  
- Documents（文档）：项目文档、已知错误、sfxcab使用（用于制作安装程序）等。
- Packages\x86 和 Packages\x64：按包分类的二进制发布版本。您可以直接从这里下载和安装/更新这些包（例如，转到Packages\x86\Base installer\update并运行update.exe）。
- Todo（待办事项）：待办任务
- Test（测试）：一些用于测试的二进制文件和文档；
- Release（发布）：生成新二进制版本的脚本；
- Output（输出）：二进制输出，您可以使用发布文件夹中的脚本生成；

**One-Core-API二进制项目由以下包组成：**  
警告：如果OCA包需要重新启动，请执行。如果您安装了所有包并且仅在最后一个包上重新启动，则Windows将损坏。
- **软件包安装程序(Pack Installer)**：One-Core-API的主要包，是所有其他包（除了App Compat和Driver Update）所必需的，其中包含此项目中使用的所有包装（如kernelbase和ntext）；

**包的安装顺序：**  
- **共同的安装顺序**：只需运行当前平台的 One-Core-api-Pack.exe 即可： x86 或 x64；

**主要功能**：  
- 默认情况下，增加对128GB（x86）和2TB（x64）内存的支持；
- 允许运行为现代Windows OS设计的新程序；
- 支持具有新驱动程序控制器的新硬件；
- 允许在任何计算机上安装Windows，并且支持通用化硬件；
**Modern Setup - 详细**  
这是一个设计用于准备Windows能够安装到其他计算机的包。如果您不想捕获Windows安装，就不需要安装这个包。
- 安装完One-Core-API Modern Setup并重新启动计算机后，您可以运行Sysprep，一个准备Windows成为通用化安装到任何硬件的工具；
- Sysprep位于：Windows\System32\Sysprep，就像Windows Vista一样；
- 这个工具与Vista上的相似。为了在XP/2003上达到与Vista或更高系统上安装到新计算机相同的行为，您必须运行sysprep.exe并选择"进入系统外箱体验（OOBE）"的操作。推荐同时选择“通用化”。选择操作和标记后，您可以在关闭选项中选择是否仅退出程序，关闭或重新启动系统。按下“确定”后，sysprep将运行清理插件并执行所选的关机选项；
- 关闭系统后，在下次启动时，将运行位于Windows\System32\oobe\setup.exe的setup.exe，并检测硬件，检测硬件后将重新启动并进入msoobe；
- 要修补Windows Vista - Win11设置并捕获Windows XP/2003/Longhorn/Reactos镜像，请使用此工具：https://github.com/Skulltrail192/One-Core-API-Tools

![Sysprep](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/615ada04-a036-43c4-ac54-824cade0b5c2)

**One-Core-API允许您运行：**  
- 最新版本的 JetBrains 应用程序（2024）;
- Android Studio 最新版本;
- NetBeans 最新版本;
- Eclipse IDE 最新版本;
- Filezilla (最新);
- LibreOffice 24.0.x (最新);
- Discord 0.309.0；
- Legocord（Discord 分支）最后
- Visual Studio 2012 和 Visual Studio 2013；
- Visual Studio Code 最新版本；
- Codium 至最新版本；
- Chrome 浏览器至最新版本
- 最新版 Opera
- 火狐浏览器（Firefox）最新版本（不过，Youtube 在 130 版之前都能正常运行）；
- Microsoft Edge 最新版本；
- Brave 至最新版本；
- Yandex 浏览器至最新版本；
- Thorium 浏览器至最新版本；
- Supermium 最新版本；
- Seamonkey最新到2.53.10;
- Thunderbird 最新版本;
- 傲游浏览器最新到7.1.6;
- Vivaldi最新版本;
- JDK 1.8 (目前仅限于Windows XP x64);
- Java JDK 和 JDK 替代品或 OpenJDK，直到JDK版本 24（也许其他版本也可以）。您可以从以下网址下载: https://bell-sw.com/pages/downloads/#/java-11-lts;
- Epic 浏览器 120;
- Python 3.6;
- .Net Framework最新到4.8;
- Geekbench 4.2;
- Performance Test;
- Adobe Reader DC (2014);
- Foxit PDF Reader (2023);
- Windows 7游戏;
- Windows 7画画;
- Windows 7写字板;
- Windows Vista应用程序;
- Spotify Windows 7和Windows 10;
- Yandex browser (最新);
- Zoom;
- 其他几个应用程序;
- Node 10.24;
- Telegram Desktop;
- Winrar 7.0 Beta 4 (最新);
- Postman;
- Insominia;
- Eclise IDE;
- Dbeaver;
- TeamViewer 14;
- Directx 9EX、10和11游戏：
  - 极品飞车17：最高通缉(Need for Speed Most Wanted 2012);
  - 极品飞车16：亡命天涯(Need for Speed The Run);
  - 街头霸王5(Street Figther V);
  - 不义联盟：人间之神(njustice: Gods among us);
  - 刺客信条4：黑旗(Assassin's Creed IV Black Flag);
  - 孤岛危机1,2,和3 (directx 10-11模式);
  - GTA 三部曲 – 最终版;
  - GTA V;
- Kate 23.08.1 (目前仅适用于Windows XP x64)

**已知限制：**  
- Youtube 在火狐 131 上无法运行。因此，我们建议使用 115 或 128 ESR 版本
- 某些应用程序的安装程序可能不起作用，如某些电子应用程序（MS teams）、Office 2013 或 GIMP 3.0 RC2 等会崩溃，应用程序无法安装。有些安装程序和应用程序需要操作系统支持 AVX，而操作系统不支持 AVX。因此，请使用从其他操作系统复制的预装版本。；
- 这些软件包无法用 nlite 整合到 Windows ISO，因为使用的是名为 “SFXCAB Substitute ” 的工具，而不是标准的微软版本。
- 目前不支持从4.6开始的标准.Net Framework安装程序。您需要一个重新打包的版本，如下所示：https://github.com/abbodi1406/dotNetFx4xW7。可用如下：https://www.wincert.net/forum/topic/13805-microsoft-net-framework-472-full-x86x64-incl-language-packs-by-ricktendo/#comment-123251。其他版本也可用，搜索论坛主题；
- 新版本的palemoon可能面临Manifest的错误问题。
- Opera 39-50可能需要以下参数才能启动：--disable-gpu（防止黑屏）和 --single-process（以防止永远加载第一页）；

**问题报告**  
要帮助重现问题，强烈建议**始终**遵循此模板：
- 描述发生的情况。
  例如：尝试加载Windows时总是出现蓝屏。Windows卡在启动屏幕上。Windows总是出现黑屏等；
- 虚拟机/计算机配置
  例如：Vmware 10、VirtualBox 6.1.0或Core 2 Duo 8400、2Gb DDR2、IDE/SATA硬盘；
- Windows版本和配置
  例如：已安装Windows XP Service Pack 3与POSReady 2009更新，并带有此程序列表：Adobe、Office等；

XP/Server 2003:上运行的应用程序的一些屏幕截图
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

**Spotify (对于Windows 7)**
![Spotify-Windows7](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/09de7c20-8670-45dc-9471-a6db9349abd0)

**Visual Studio Code 1.81**
![VisualCode](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/b21748b9-25bb-412d-95b3-2219d2efdf42)

**Microsoft Chess 3d**
![Chess3d](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/bd1ad0c6-edde-4ff2-a6e0-074c7379fab6)

**Telegram Desktop 4.14**
![Telegram-Desktop](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/73e13167-49b8-4282-81cb-969435046dde)

**Libre Office 24 (latest)**
![LibreOffice](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/11fd191d-270c-428d-8d41-0498e8fafb3b)

**Discord 0.309**
![Discord-Login](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/8a4c12b5-19fc-454d-b02a-a1db807d3900)
![Discord](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/eb673541-4e66-4c76-867e-346edbaaa0af)

**Zoom会议**
![Zoom](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/d002cf1b-c5f4-4c0c-b629-00e031a56765)

**Java 11**
![Capturar](https://user-images.githubusercontent.com/5159776/178078132-da504607-a1ca-4f8d-ae25-6a7eb367bdaa.PNG)

**Avast和Chromium 68**
![Avast](https://user-images.githubusercontent.com/5159776/178078208-c13b3448-ee6a-4c56-9d94-d0c62d51949e.PNG)

**Windows 7桌面便签**
![StickyNotes](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/669ba3e4-b831-4a96-ad40-d87e3e9531e2)

**Windows 7画画(小畫家)**
![Windows7画画](https://user-images.githubusercontent.com/5159776/192194273-de70c23e-8731-4fb6-96c1-9bee98947269.PNG)
