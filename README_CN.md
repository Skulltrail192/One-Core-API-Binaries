**欢迎使用One-Core-API二进制文件！**
***
**语言:**    
[English](README.md) | [简体中文](README_CN.md) | [Русский](README_RU.md) | [Українська](README_UK.md)
***

该存储库包含One-Core-API项目的二进制发布版本。这些版本兼容于Windows Server 2003 SP2、Windows XP SP3和Windows XP x64 SP2。

**官方Discord服务器**: https://discord.gg/eRcGuFtn6p  

**注意**：  
- 可能不支持Windows XP的非官方SP4以及Integral Edition!

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
- 默认情况下，增加对128 GB（x86）和2 TB（x64）内存的支持；
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
- JetBrains WebStorm 2018；
- JetBrains Intellij 2018（其他版本也可能可用)；
- JetBrains WebStorm 2023.x.x（目前仅适用于Windows XP x64）
- JetBrains IntelliJ 2023.x.x （目前仅适用于Windows XP x64）
- Adobe Photoshop CC 2018;
- Filezilla（最新版本）；
- Visual Studio Code 1.81.1；
- Chrome版本最高到109！；
- Opera版本最高到95；
- Firefox版本最高116（存在一些问题，需要解决方案)；
- Microsoft Edge版本109以上；
- Brave版本117以上；
- Yandex最新版本；
- Thorium浏览器版本109以上；
- Supermium版本117以上；
- Seamonkey版本2.53.10以上；
- Thunderbird版本116x以上；
- Maxthon版本7.1.6以上；
- Vivaldi最新版本；
- JDK 1.8（目前仅适用于Windows XP x64)；
- Java Alternative JDK或OpenJDK直到版本21（其他版本可能也有效）。您可以从以下网址下载：https://bell-sw.com/pages/downloads/#/java-11-lts；
- Maxthon 6；
- Epic Browser 94；
- Python 3.6；
- .Net Framework最高到4.8；
- Geekbench 4.2；
- Performance Test；
- Adobe Reader DC（2017年版）；
- Windows 7游戏；
- Vista原生应用程序；
- Spotify在Windows XP/Vista结束支持后，版本1.0.60；
- Yandex浏览器（最新版本）；
- Zoom；
- 其他应用程序；
- Node 10.24；
- Winrar 6.24（最新版本）；
- Directx 9EX、10和11游戏：
  - 极品飞车17：最高通缉(Need for Speed Most Wanted 2012);
  - 极品飞车16：亡命天涯(Need for Speed The Run);
  - 街头霸王5(Street Figther V);
  - 不义联盟：人间之神(njustice: Gods among us);
  - 刺客信条4：黑旗(Assassin's Creed IV Black Flag);
  - 孤岛危机1,2,和3 (directx 10-11模式);
- Kate 23.08.1 (目前仅适用于Windows XP x64)

**已知限制：**  
- 多个应用程序的打开/保存对话框因为使用了Vista实现的IFileDialog而无法完全工作。正在进行中的工作；
- 运行Firefox 56+可能会遇到页面加载问题。要解决此问题，从版本55到72，您必须创建一个名为MOZ_FORCE_DISABLE_E10S的环境变量。它将是环境变量的键，值为1。对于73版本及更高版本，值将是版本号，例如73.0.0（请检查版本号）；
- 对话框或窗口中的某些地方仅保留英文。国际化工作正在进行中；
- 新应用程序安装程序可能不起作用，例如Chrome、Maxthon、Discord、TeamViewer等崩溃并且应用程序未安装。需要使用从其他操作系统复制的预安装版本；
- 一些安装程序如VS Code和Skype在创建快捷方式阶段会生成“类不受支持”的错误；
- Chrome随机稳定性问题和崩溃。当前不支持扩展。下载可能会崩溃应用程序或出现网络错误，具体取决于版本；
- Firefox 55-78存在一些限制；
- Firefox 54 32位版本在Windows XP x64上无法运行。Chrome 61+也不起作用；
- 无法将这些包集成到Windows Iso中，因为使用了一个称为“SFXCAB Substitute”的工具，而不是标准的Microsoft版本；
- 标准的.NET Framework安装程序从4.6开始目前不受支持。您需要像这里做的一样重新打包版本：https://github.com/abbodi1406/dotNetFx4xW7  
  也可以在此处找到其他版本：https://www.wincert.net/forum/topic/13805-microsoft-net-framework-472-full-x86x64-incl-language-packs-by-ricktendo/#comment-123251  
  也有其他版本，请在论坛主题中搜索；
- Palemoon版本27-31无法下载任何内容。下载不起作用；
- Opera 39-46可能需要遵循以下参数才能启动：--disable-gpu（以防止黑屏）和--single-process（以防止永远加载首页）；
- 使用Dwrite的Qt5应用程序在启动时崩溃。存在一些问题，例如删除dwrite后出现的黑屏（应用程序可以启动并且可以使用，但是会有黑屏等）。

**问题报告**  
要帮助重现问题，强烈建议**始终**遵循此模板：
- 描述发生的情况。
  例如：尝试加载Windows时总是出现蓝屏。Windows卡在启动屏幕上。Windows总是出现黑屏等；
- 虚拟机/计算机配置
  例如：Vmware 10、VirtualBox 6.1.0或Core 2 Duo 8400、2Gb DDR2、IDE/SATA硬盘；
- Windows版本和配置
  例如：已安装Windows XP Service Pack 3与POSReady 2009更新，并带有此程序列表：Adobe、Office等；
    
（如果需要对特定部分进行翻译或有其他问题，请告诉我！）  

XP/Server 2003:上运行的应用程序的一些屏幕截图
**Chrome 58**
[image_2022_04_07T03_35_23_052Z](https://user-images.githubusercontent.com/5159776/178077754-de45d085-7e32-4080-b577-29f67a777076.png)
**Advanced Chrome 64**
![image_2022_04_07T03_42_36_918Z](https://user-images.githubusercontent.com/5159776/178077817-e58fd872-f0fb-431d-aaad-c4a15510ed87.png)
**Basilisk**
![image_2022_04_08T21_38_17_976Z](https://user-images.githubusercontent.com/5159776/178077859-079bfca4-bdb6-402e-8991-b88e7dfe387c.png)
Firefox Dev 54**
![image_2022_04_08T14_03_12_014Z](https://user-images.githubusercontent.com/5159776/178077897-676267bd-31c2-451d-8d81-951c0223bac4.png)
**Visual Code 1.22**
![image_2022_04_07T03_54_26_696Z](https://user-images.githubusercontent.com/5159776/178077980-31788372-84e3-43b9-8bd3-d3204a375197.png)
**Chedot (chrome fork) 86**
![image_2022_04_07T22_34_04_707Z](https://user-images.githubusercontent.com/5159776/178078013-9ccc115e-f6f6-44d0-937f-1a73fa5c6dee.png)
**Java 11**
![Capturar](https://user-images.githubusercontent.com/5159776/178078132-da504607-a1ca-4f8d-ae25-6a7eb367bdaa.PNG)
**Avast和Chromium 68**
![Avast](https://user-images.githubusercontent.com/5159776/178078208-c13b3448-ee6a-4c56-9d94-d0c62d51949e.PNG)
**Visual Studio Code 1.70**
![VisualCode1 70](https://user-images.githubusercontent.com/5159776/192194220-9f4f324d-b0d8-4c40-a378-2c25c81eff16.PNG)
**Windows 7画画(小畫家)**
![Windows7画画](https://user-images.githubusercontent.com/5159776/192194273-de70c23e-8731-4fb6-96c1-9bee98947269.PNG)
