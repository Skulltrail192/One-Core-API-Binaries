<div align="center">
  <a href="https://github.com/One-Core-API/One-Core-API/releases">
    <img src="Assets/banner.png" width="100%" height="auto" alt="Banner">
  </a>
  <h6>バナーを提供してくれた@pashtetusss777に心から感謝します</h6>
</div>

***
**言語：**
[English](README.md) | [简体中文](README_CN.md) | [Русский](README_RU.md) | [Українська](README_UK.md) | [日本語](README_JP.md) | [Português-Brasil](README_BR.md)
***


**日本語版README（README_JP.md）の最終更新日 2025年2月5日<br>2025年2月1日時点のREADME.mdを基に翻訳**


 **この日本語訳は機械翻訳をベースに誤訳などをできる限り修正したものです 間違いが含まれる可能性があります。<br>また、元のREADMEの更新に追従できてない場合がありますので、更新日を確認してから読むのを推奨します。**<br><br><br><br>
**このリポジトリには、One-Core-API プロジェクトのバイナリ リリースが含まれています。これらは、Windows Server 2003 RTM、SP1、SP2、Windows XP RTM、SP1、SP2、SP3、および Windows XP
x64 SP1/SP2 と互換性があります。ただし、最新のサービス パック更新と利用可能なすべての更新を適用したシステムを使用することを強くお勧めします。**

<!-- **Official Discord Server**: <h2>https://discord.gg/eRcGuFtn6p</h2> -->

- [主な特徴](#主な特徴)
  - [このソフトウェアを使用する前に](#このソフトウェアを使用する前に)
- [アプリケーションの互換性](#アプリケーションの互換性)
- [既知の制限](#既知の制限)
- [問題を提出する前に…](#問題を提出する前に)
- [リポジトリの構造](#リポジトリの構造)
- [追加情報とリンク](#追加情報とリンク)
  - [公式Discordサーバー](#公式discordサーバー)
- [ショーケース / 概念実証](#ショーケース--概念実証)


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

## 主な特徴
- **デフォルトで x86 のメモリ サポートを 128 GB、x64 のメモリ サポートを 2 TB に増やします。**
- **最新の Windows OS 用に設計された新しいプログラムの実行をサポートします。**
- **新しいドライバー コントローラーを備えた新しいハードウェアをサポートします。**
- **Windows XP および Windows Server 2003 でサポートされているすべての言語に対する多言語サポート。**

### このソフトウェアを使用する前に
> このソフトウェアは、各システムから変更されたファイルを利用しており、テスト段階または実験段階のファイルも含まれており、<b>1 人の人物によって開発されています</b>。そのため、さまざまなコンピュータ構成や仮想マシンで起こり得るすべてのシナリオを予測することは不可能です。
>
> <h4>Windows XP/2003 から Vista への移行は、新しい API、テクノロジ、および既存の API の変更の開発において大きな飛躍を示しました。このため、NT 5.x と NT 6.x システム間で同じレベルの互換性を実現することは困難です。</h4>
>
> 落ち着いて慎重に行動してください。
> ソフトウェアに欠陥がある、または「品質が悪い」と結論付ける前に、<b>Github Issues または [Discord サーバー](https://discord.gg/eRcGuFtn6p) を通じて、遭遇した問題を報告してください</b>。
>
> すべての問題が解決されることを保証することはできませんのでご了承ください。ただし、私はそれらを分析して修正するために全力を尽くします。
>
> あなたの協力は貴重なものであり、苦情や否定的なフィードバックは製品の改善に寄与しません。

## アプリケーションの互換性
- JetBrains IDE 最新リリース (2024) まで;
- Android Studio 最新バージョンまで;
- NetBeans 最新バージョンまで;
- Eclipse IDE 最新バージョンまで
- Adob​​e 製品 (Photoshop、Illustrator、Dreamweaver など) 2019 バージョンまで;
- Filezilla (最新バージョン);
- LibreOffice 24.0.x (最新バージョン);
- Discord 0.309.0;
- Legocord (Discord フォーク) 最新版;
- Visual Studio 2012 および Visual Studio 2013;
- Visual Studio Code (および Codium などのフォーク) 最新バージョンまで;
- Chromium ブラウザー (Chrome、Opera、Edge など) 最新バージョンまで;
- Gecko ベース (Firefox、Zen Browser) 最新バージョンまで (ただし、YouTube は Firefox バージョン 130 までしか動作しません);
- Seamonkey バージョン 2.53.10 まで;
- Thunderbird 最新バージョンまで;
- Maxthon バージョン 7.1.6 まで;
- Vivaldi 最新バージョンまで;
- JDK 1.8 (現時点では Windows XP x64 のみ);
- Java JDK および代替 JDK または OpenJDK バージョン 24 まで (他のバージョンも動作する可能性があります)。次の場所からダウンロードできます: https://bell-sw.com/pages/downloads/#/java-11-lts;
- Epic Browser 120;
- Python 3.6 (3.8/3.9 も動作する可能性がありますが、[mod](https://mega.nz/folder/KxExlAiC#L9rAQ5kTCtlHgZUwaxMpgw) バージョンのみ);
- .Net Framework バージョン 4.8 まで;
- .NET 6.0;
- Geekbench 4.2;
- Performance Test;
- Adob​​e Reader DC (2024 年まで);
- Foxit PDF Reader (2023);
- Windows 7 ゲーム;
- Windows 7 ペイント;
- Windows 7 ワードパッド;
- Windows Vista のネイティブ アプリケーション;
- Windows 7 および Windows 10 用の Spotify;
- Line;
- Zoom;
- Node 10.24;
- Telegram Desktop;
- Winrar 7.0 (最新);
- Postman;
- Insomnia;
- Dbeaver;
- TeamViewer 14;
- Directx 9EX、10、および 11 ゲーム:
- Need for Speed Most Wanted 2012;
- Need for Speed The Run;
- Street Fighter V;
- Injustice: Gods Among Us;
- Assassin's Creed Black Flag;
- Crysis 1、2、および 3 (directx 10-11 モード);
- GTA Trilogy;
- GTA V;
- Minecraft 1.21.x;
- バイオハザード 5 dx10 モード;
- Lost Planet;
- kate 23.08.1;
  
## 既知の制限
- Firefox のバージョン 131 以降では Youtube は動作しません。そのため、バージョン 115 または 128 ESR の使用をお勧めします。
- 一部のアプリケーション インストーラーは動作しない場合があります。たとえば、一部の電子ベースのアプリケーション (MS Teams)、Office 2013、GIMP 3.0 RC2 などがクラッシュし、アプリケーションがインストールされません。一部のインストーラーとアプリケーションでは、OS による AVX サポートが必要ですが、現時点ではサポートされていません。その場合は、他のオペレーティング システムからコピーされたプリインストール バージョンを使用します。
- パッケージは、標準の Microsoft バージョンではなく、「SFXCAB Substitute」というツールを使用しているため、nlite を使用して Windows ISO に統合できません。
- 4.6 以降の標準 .Net Framework インストーラーは、現時点ではサポートされていません。こちらで行ったように、再パックされたバージョンが必要です: https://github.com/abbodi1406/dotNetFx4xW7 また、次の場所から入手できます: https://www.wincert.net/forum/topic/13805-microsoft-net-framework-472-full-x86x64-incl-language-packs-by-ricktendo/#comment-123251 他のバージョンも入手できます。フォーラムのトピックで検索してください。
- palemoon の新しいバージョンでは、サイドバイサイド構成エラーの問題が発生する可能性があります。;
- Opera 39 - 50 を起動するには、次のパラメータが必要になる場合があります: --disable-gpu (黒い画面を防ぐため) および --single-process (最初のページが永遠に読み込まれないようにするため)。
## 問題を提出する前に…

**問題を報告する前に、その問題が[One-Core-API-Canary](https://github.com/shorthorn-project/One-Core-API-Binaries-Canary)に存在するかどうかを確認し、既存の[Issues](https://github.com/Skulltrail192/One-Core-API-Binaries/issues)を注意深くチェックして、あなたの問題がすでに報告されていないかどうかを確認することを強くお勧めします。**

**問題が Canary に存在し、現在のIssuesにまだリストされていない場合は、次の情報を含めて、問題の詳細なレポートを提供してください。**

**1. システム構成:**
* **タイプ** (実機/VM)
* **Windows OS エディション** (例: Windows XP Professional Service Pack 3)
* **SP 後の更新:**
* Service Pack 後の更新はインストールされましたか? (はい/いいえ)
* はいの場合、One-Core-API のインストール **前** または **後** にインストールされたかどうかを指定します。
* **インストールされているソフトウェア:** 問題に関連する可能性のある関連ソフトウェアをすべてリストします (例: Adob​​e Photoshop CC 2018、Firefox 132 など)。
* **技術仕様**:
* **物理ハードウェアの場合:** プロセッサ、RAM (タイプ、容量)、ハード ドライブ タイプ、容量 (例: IDE ディスク、120 GB) の詳細を提供します。
* **仮想マシンの場合:** 仮想マシンの構成の詳細を指定します (例: Oracle VirtualBox 6.1.0、2 GB RAM、120 GB ディスク、AHCI モード)。
* **エラーを再現する手順**


> **重要:** 可能であれば、**関連するログも** 問題に添付してください。これにより、解決策をより早く特定するのに大いに役立ちます。

> 可能であれば、問題のビデオ録画を添付することもお勧めします。
>
> **問題が再現できない場合は、Issueはクローズされますのでご注意ください。**
> 
## リポジトリの構造
- Document: プロジェクト ドキュメント、既知のバグ、sfxcab の使用法 (インストーラーの作成用) など。
- Packages\x86 および Packages\x64: パッケージ別に分類されたバイナリ リリース。ここからパッケージを直接ダウンロードしてインストール/更新できます (つまり、Packages\x86\Base installer\update に移動して update.exe を実行します)。
- Todo: 実行するタスク
- Test: テスト用のバイナリとドキュメント
- Release: 新しいバイナリ リリースを生成するスクリプト
- Output: Release フォルダーのスクリプトを使用して生成できるバイナリ出力。

## 追加情報とリンク
**One-Core-API ベースのシステムの拡張機能:**

<b><a href="https://github.com/shorthorn-project/One-Core-API-Extras" style="font-size: 18px">https://github.com/shorthorn-project/One-Core-API-Extras</a></b>

**One-Core-APIベースのシステムのための新しいデプロイメント・システムのためのツール：**

<b><a href="https://github.com/Skulltrail192/One-Core-API-Tools" style="font-size: 18px">https://github.com/Skulltrail192/One-Core-API-Tools</a></b>

### 公式Discordサーバー

**One-Core-API の公式 Discord サーバーに参加したい場合は、こちらから参加できます:**

<b><a href="https://discord.gg/eRcGuFtn6p" style="font-size: 25px">https://discord.gg/eRcGuFtn6p</a></n>

## ショーケース / 概念実証
XP/Server 2003 で実行されているアプリケーションのスクリーンショット:

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

**Microsoft チェス 3d**
![Chess3d](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/bd1ad0c6-edde-4ff2-a6e0-074c7379fab6)

**Telegram 4.14**
![Telegram-Desktop](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/73e13167-49b8-4282-81cb-969435046dde)

**Libre Office 24 (最新)**
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

**Avast と Chromium 68**
![Avast](https://user-images.githubusercontent.com/5159776/178078208-c13b3448-ee6a-4c56-9d94-d0c62d51949e.PNG)

**Windows 7 付箋**
![StickyNotes](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/669ba3e4-b831-4a96-ad40-d87e3e9531e2)

**Windows 7 ペイント**
![Paint](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/81728a44-c9e7-41e8-b68b-8ea7b119ebba)

**Windows 7 ワードパッド**
![Wordpad](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/9dac02c7-7139-47fe-8732-ccd9ef91090b)
