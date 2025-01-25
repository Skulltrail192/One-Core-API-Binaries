<div align="center">
  <a href="https://github.com/One-Core-API/One-Core-API/releases">
    <img src="Assets/banner.png" width="100%" height="auto" alt="Banner">
  <h6>衷心感谢 @pashtetusss777 提供的横幅</h6>
</div>

***
**语言:**    
[English](README.md) | [简体中文](README_CN.md) | [Русский](README_RU.md) | [Українська](README_UK.md) | [日本語](README_JP.md)
***

该存储库包含One-Core-API项目的二进制发布版本。这些版本兼容于Windows Server 2003 SP2、Windows XP SP3和Windows XP x64 SP2。

<!-- **官方Discord服务器**: <h2>https://discord.gg/eRcGuFtn6p</h2> -->

- [主要功能](#主要功能)
  - [使用该软件前](#使用该软件前)
- [应用程序兼容性](#应用程序兼容性)
- [已知限制](#已知限制)
- [在提交问题之前](#在提交问题之前)
- [仓库结构](#仓库结构)
- [更多信息和链接](#更多信息和链接)
  - [官方Discord服务器](#官方discord服务器)
- [展示/概念验证](#展示概念验证)

<!-- **报告问题的主要指南，主要是 BSOD（蓝屏死机）**

- 电脑配置： 是虚拟机还是真机？安装的处理器和内存是多少？硬盘控制器是 AHCI、IDE、NVME 还是 SCSI？
- Windows 配置： 版本是什么？是否是自定义 ISO/构建？安装了哪些更新？服务包是什么？安装了哪些程序？
- 使用的 ISO 是什么？一定要提供链接；
- 重现 BSOD 的步骤。例如：我安装了 OCA base，同时安装了 XP Integral Edition，并安装/删除了所有选项。或者：我安装了 Avast 或其他杀毒软件，然后安装了 OCA base；
- 请启用完整的内存转储并上传。
- 给 BSOD 拍张照片。如果是自动重启，请在 Windows 启动时按 F8，然后选择 “禁用自动重启”。" -->

<!--**此存储库中的文件夹:** -->


<!--**One-Core-API二进制项目由以下包组成：**  
警告：如果OCA包需要重新启动，请执行。如果您安装了所有包并且仅在最后一个包上重新启动，则Windows将损坏。
- **软件包安装程序(Pack Installer)**：One-Core-API的主要包，是所有其他包（除了App Compat和Driver Update）所必需的，其中包含此项目中使用的所有包装（如kernelbase和ntext）; -->

<!--**包的安装顺序：**  
- **共同的安装顺序**：只需运行当前平台的 One-Core-api-Pack.exe 即可： x86 或 x64；-->

## 主要功能
- 默认情况下，x86 版支持 128 GB 内存，x64 版支持 2 TB 内存;
- 支持运行为现代 Windows 操作系统设计的更新程序；
- 支持带有新驱动程序控制器的新硬件；
- 为所有 Windows XP 和 Windows Server 2003 支持的语言提供多语言支持；

### 使用该软件前
> 该软件使用了各系统中修改过的文件，还包括仍处于测试或实验阶段的文件，而且<b>是由一个人</b>开发的。因此，预测各种计算机配置或虚拟机中可能出现的所有情况并不可行。
> 
> <h4>Windows XP/2003 和 Vista 之间的过渡标志着新 API、技术开发和现有 API 修改方面的重大飞跃。因此，要在 NT 5.x 和 NT 6.x 系统之间实现相同程度的兼容性是一项挑战。</h4>
> 
> 我恳请你们保持冷静和谨慎。
> 在得出软件有缺陷或 “质量差 ”的结论之前，" <b>请通过 Github Issues 或 [Discord 服务器](https://discord.gg/eRcGuFtn6p)</b>.
> 
> 请注意，我不能保证所有问题都能得到解决。但是，我会对问题进行分析，并尽一切努力予以纠正。
> 
> 您的帮助是宝贵的，投诉或负面反馈无助于产品的改进。


## 应用程序兼容性
- 最新版本的 JetBrains（2024）;
- Android Studio 最新版本;
- NetBeans 最新版本;
- Eclipse IDE 最新版本;
- Filezilla (最新);
- LibreOffice 24.0.x (最新);
- Discord 0.309.0；
- Legocord（Discord 分支）最新;
- Visual Studio 2012 和 Visual Studio 2013；
- 最新版本的 Visual Studio Code（以及 Codium 等fork）;
- 最新版本的 Chromium 浏览器（Chrome、Opera、Edge 等）;
- 基于 Gecko 的浏览器（Firefox、Zen 浏览器）的最新版本（不过，YouTube 只能运行到 Firefox 130 版本）;
- Seamonkey最新到2.53.10;
- Thunderbird 最新版本;
- 傲游浏览器最新到7.1.6;
- Vivaldi最新版本;
- JDK 1.8 (目前仅限于Windows XP x64);
- Java JDK 和替代 JDK 或 OpenJDK，最高版本为 24（其他版本可能也适用）。您可以从以下网站下载: https://bell-sw.com/pages/downloads/#/java-11-lts;
- Epic 浏览器 120;
- Python 3.6(3.8/3.9 版本也可以使用，只是需要[修改](https://mega.nz/folder/KxExlAiC#L9rAQ5kTCtlHgZUwaxMpgw) 版本);
- .Net Framework最新到4.8;
- .NET 6.0
- Geekbench 4.2;
- Performance Test;
- Adobe Reader DC (2024);
- Foxit PDF Reader (2023);
- Windows 7游戏;
- Windows 7画画;
- Windows 7写字板;
- Windows Vista应用程序;
- Spotify Windows 7和Windows 10;
- Zoom;
- 其他几个应用程序;
- Node 10.24;
- Telegram Desktop;
- Winrar 7.0 (最新);
- Postman;
- Insomnia;
- Eclise IDE;
- Dbeaver;
- TeamViewer 14;
- Directx 9EX、10和11游戏：
  - 极品飞车17：最高通缉(Need for Speed Most Wanted 2012);
  - 极品飞车16：亡命天涯(Need for Speed The Run);
  - 街头霸王5(Street Fighter V);
  - 不义联盟：人间之神(Injustice: Gods Among Us);
  - 刺客信条4：黑旗(Assassin's Creed Black Flag);
  - 孤岛危机1,2,和3 (directx 10-11模式);
  - GTA 三部曲 – 最终版;
  - GTA V;
- Kate 23.08.1

## 已知限制
- Youtube 在火狐 131 上无法运行。因此，我们建议使用 115 或 128 ESR 版本
- 某些应用程序的安装程序可能不起作用，如某些电子应用程序（MS teams）、Office 2013 或 GIMP 3.0 RC2 等会崩溃，应用程序无法安装。有些安装程序和应用程序需要操作系统支持 AVX，而操作系统不支持 AVX。因此，请使用从其他操作系统复制的预装版本。；
- 这些软件包无法用 nlite 整合到 Windows ISO，因为使用的是名为 “SFXCAB Substitute ” 的工具，而不是标准的微软版本。
- 目前不支持从4.6开始的标准.Net Framework安装程序。您需要一个重新打包的版本，如下所示：https://github.com/abbodi1406/dotNetFx4xW7。可用如下：https://www.wincert.net/forum/topic/13805-microsoft-net-framework-472-full-x86x64-incl-language-packs-by-ricktendo/#comment-123251。其他版本也可用，搜索论坛主题；
- 新版本的palemoon可能面临Manifest的错误问题。
- Opera 39-50可能需要以下参数才能启动：--disable-gpu（防止黑屏）和 --single-process（以防止永远加载第一页）；

## 在提交问题之前

**在报告问题之前，我强烈建议检查 [One-Core-API-Canary](https://github.com/shorthorn-project/One-Core-API-Binaries-Canary) 中是否存在该问题，并仔细查看现有的 [Issues](https://github.com/Skulltrail192/One-Core-API-Binaries/issues) 是否已经报告过您的问题。**

如果问题**确实存在于 Canary 中，且尚未列入当前问题，请提供一份详细的问题报告，包括以下信息：** 

**1. 系统配置：** **
*   **类型**（物理硬件/虚拟机）
*   **Windows 操作系统版本**（例如 Windows XP Professional Service Pack 3）
*   **Service Pack Post更新：** *.
    *  是否安装了任何 Service Pack Post更新？ (是/否)
       *  如果是，请说明是在安装 One-Core-API 之前还是之后安装的。
*    **安装的软件：** 列出可能与问题有关的任何相关软件（如 Adobe Photoshop CC 2018、Firefox 132 等）。
*   **技术规格**：
    *   **如果是物理硬件：** 提供处理器、内存（类型、数量）、硬盘类型和容量（如 IDE 磁盘，120 GB）的详细信息。
    *   **如果是虚拟机：** 指定虚拟机配置详细信息（如 Oracle VirtualBox 6.1.0、2 GB 内存、120 GB 磁盘、AHCI 模式）。
*   **重现错误的步骤**

> **重要：** 如果可能，请**将任何相关日志**到问题中。这将大大有助于更快地找到解决方案。

> 如果可行，最好还能附上问题的视频记录。
>
> **请注意，如果问题无法重现，该问题将被关闭。**

## 仓库结构
- Documents（文档）：项目文档、已知错误、sfxcab使用（用于制作安装程序）等。
- Packages\x86 和 Packages\x64：按包分类的二进制发布版本。您可以直接从这里下载和安装/更新这些包（例如，转到Packages\x86\Base installer\update并运行update.exe）。
- Todo（待办事项）：待办任务
- Test（测试）：一些用于测试的二进制文件和文档；
- Release（发布）：生成新二进制版本的脚本；
- Output（输出）：二进制输出，您可以使用发布文件夹中的脚本生成；

## 更多信息和链接
**基于 One-Core-API 的系统的扩展功能:**

<b><a href="https://github.com/shorthorn-project/One-Core-API-Extras" style="font-size: 18px">https://github.com/shorthorn-project/One-Core-API-Extras</a></b>

**基于 One-Core-API 系统的新部署系统工具:**

<b><a href="https://github.com/Skulltrail192/One-Core-API-Tools" style="font-size: 18px">https://github.com/Skulltrail192/One-Core-API-Tools</a></b>

### 官方Discord服务器

**如果您想加入我们的 One-Core-API 官方 Discord 服务器，请点击此处:**

<b><a href="https://discord.gg/eRcGuFtn6p" style="font-size: 25px">https://discord.gg/eRcGuFtn6p</a></n>

## 展示/概念验证
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
