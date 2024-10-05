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
几乎所有Windows的语言：葡萄牙语-巴西（我的语言），葡萄牙语（葡萄牙），土耳其语，中文（繁体和简体），法语，意大利语，匈牙利语，乌克兰语（部分支持），西班牙语，波兰语，俄语和韩语；

**此存储库中的文件夹:**  
- Documents（文档）：项目文档、已知错误、sfxcab使用（用于制作安装程序）等。
- Packages\x86 和 Packages\x64：按包分类的二进制发布版本。您可以直接从这里下载和安装/更新这些包（例如，转到Packages\x86\Base installer\update并运行update.exe）。
- Todo（待办事项）：待办任务
- Test（测试）：一些用于测试的二进制文件和文档；
- Release（发布）：生成新二进制版本的脚本；
- Output（输出）：二进制输出，您可以使用发布文件夹中的脚本生成；

**One-Core-API二进制项目由以下包组成：**  
警告：如果OCA包需要重新启动，请执行。如果您安装了所有包并且仅在最后一个包上重新启动，则Windows将损坏。
- **Base（基础）**：One-Core-API的主要包，是所有其他包（除了App Compat和Driver Update）所必需的，其中包含此项目中使用的所有包装（如kernelbase和ntext）；
- **Additional Dlls（额外的DLL）**：引入了Windows后期版本的多个新dll。
- **D3d（D3D）**：D3D运行时（主要是基于WineD3D的DX10和DX11）；
- **App Compat（应用程序兼容性）**：应用程序兼容性设置，从Windows后期版本向后兼容。
- **Driver Update（驱动程序更新）**：带来更新的acpi驱动器，支持ACPI 2.0，新驱动器如Storachi（适用于AHCI驱动器控制器），NVME（适用于NVME M.2驱动器控制器）和USBXHCI（USB 3.0）；
- **Branding（品牌）**：引入了Windows Vista上的新品牌系统，D3D和Modern Setup包都需要此包。

**包的安装顺序：**  
- **常规顺序**：Base-> Additional Dlls -> API-SET -> Branding -> D3d -> Modern Setup -> Driver Update -> App Compat；
- **仅Base包（不需要其他包一起安装，除非您需要）**
  - **仅App Compat（应用程序兼容性）**：此包可以单独安装；
  - **仅Driver Update（驱动程序更新）**：此包可以单独安装；
  - **AppCompat first（首先安装AppCompat）**：此包可以单独安装。

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
- JetBrains Applications until 2021.3;
- JetBrains WebStorm 2021;
- Intellij 2021 (Other versions may work too);
- Adobe Photoshop CC 2018;
- Filezilla (latest);
- LibreOffice 24.0.x (最新);
- Discord 0.309.0
- Visual Studio 2012;
- Visual Studio Code最新到1.83.1;
- Codium最新到1.89.x;
- Chrome最新到version 129;
- Opera最新到version 106;
- Firefox最新到130 beta (with some bugs);
- Microsoft Edge最新到128;
- Brave最新到1.x (Chromium 129);
- Yandex Browser最新版本;
- Thorium browser最新到109;
- Supermium最新到124;
- Seamonkey最新到2.53.10;
- Thunderbird最新到121;
- 傲游浏览器最新到7.1.6;
- Vivaldi最新版本;
- JDK 1.8 (目前仅限于Windows XP x64);
- Java 替代 JDK 或 OpenJDK 直到版本 21（也许其他版本也可以）。您可以从下载: https://bell-sw.com/pages/downloads/#/java-11-lts;
- Epic Browser 120;
- Python 3.6;
- .Net Framework最新到4.8;
- Geekbench 4.2;
- Performance Test;
- Adobe Reader DC (2018);
- Foxit PDF Reader (2023);
- Windows 7游戏;
- Windows 7画画;
- Windows 7写字板;
- Windows Vista应用程序;
- Spotify Windows 7;
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
- Directx 9EX、10和11游戏：
  - 极品飞车17：最高通缉(Need for Speed Most Wanted 2012);
  - 极品飞车16：亡命天涯(Need for Speed The Run);
  - 街头霸王5(Street Figther V);
  - 不义联盟：人间之神(njustice: Gods among us);
  - 刺客信条4：黑旗(Assassin's Creed IV Black Flag);
  - 孤岛危机1,2,和3 (directx 10-11模式);
- Kate 23.08.1 (目前仅适用于Windows XP x64)

**已知限制：**  
- 运行 Firefox 56+ 您可能会遇到页面加载问题。要解决此问题，从版本 55 到 72，您必须创建一个名为 MOZ_FORCE_DISABLE_E10S 的环境变量。它将作为用户环境变量的键，值为1。对于73以上的版本，该值为版本号，例如73.0.0（检查版本号）。
- 对话框或窗口中的某些地方仅保留英文。国际化进程正在进行中。已安装的 mui 包的一些问题；
- 新的应用程序安装程序可能无法工作，例如 Chrome、傲游浏览器、Discord、Team Viewer等崩溃且应用程序未安装。需要 
使用从其他操作系统复制的预安装版本；
- 该软件包无法使用nlite集成到Windows Iso，因为使用名为“SFXCAB Substitute”的工具，而不是标准Microsoft版本；
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

**Vivaldi**
![vivaldi](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/86d5895f-977a-414f-b0d5-0e877a658676)

**Spotify (Windows 7)**
![Spotify-Windows7 avif](https://github.com/user-attachments/assets/9ad0f15c-2e64-49dd-9aaf-2e60ad7aaefb)

**Visual Studio Code 1.81**
![VisualCode avif](https://github.com/user-attachments/assets/de358b56-f736-4f61-b1d0-6b0d25002ff8)

**Microsoft Chess 3d**
![Chess3d avif](https://github.com/user-attachments/assets/4b8770be-7a28-4c13-9b40-dd7ab779063d)

**Telegram Desktop 4.14**
![Telegram-Desktop avif](https://github.com/user-attachments/assets/a9203e1a-147d-4160-a5bd-4c42337ec7c2)

**Libre Office 24 (latest)**
![LibreOffice avif](https://github.com/user-attachments/assets/f318b833-7362-47d7-94db-f34f33c3ff29)
![Writer-LibreOffice avif](https://github.com/user-attachments/assets/2af88351-eaa2-4b46-895c-b5073141a386)

**Discord 0.309**
![Discord-Login avif](https://github.com/user-attachments/assets/c8f6b4ff-93b8-4b5a-a7db-e4332a2289c2)
![Discord avif](https://github.com/user-attachments/assets/bcb4288c-5c85-4e95-a3ca-ad414136bd2f)

**Telegram桌面**
![Telegram-Desktop avif](https://github.com/user-attachments/assets/a9203e1a-147d-4160-a5bd-4c42337ec7c2)

**Zoom会议**
![Zoom meeting avif](https://github.com/user-attachments/assets/dac5c8be-ca16-4e47-ac4a-b736f66d57aa)

**Java 11**
![Capturar avif](https://github.com/user-attachments/assets/b12fd34f-9ecf-4506-98a8-bf9fd572ca8b)

**Avast和Chromium 68**
![Avast avif](https://github.com/user-attachments/assets/70aac117-0159-4c73-9d7d-cc65ee8b6265)

**Windows 7桌面便签**
![StickyNotes avif](https://github.com/user-attachments/assets/70566c6e-41c4-49a5-a7d8-e54a03b3e0ad)

**Windows 7画画(小畫家)**
![Wordpad avif](https://github.com/user-attachments/assets/1eb8246f-0b29-4a6b-b2b3-fd8e33e4cd4a)

**Windows 7写字板**
![Wordpad avif](https://github.com/user-attachments/assets/1eb8246f-0b29-4a6b-b2b3-fd8e33e4cd4a)
