**One-Core-API バイナリへようこそ!**
***
**言語：**
[English](README.md) | [简体中文](README_CN.md) | [Русский](README_RU.md) | [Українська](README_UK.md) | [日本語](README_JP.md) | [Português-Brasil](README_BR.md)
***
***この日本語訳は機械翻訳をベースに誤訳などをできる限り修正したものです 間違いがある可能性があります***

このリポジトリには、One-Core-API プロジェクトのバイナリ リリースが含まれています。これらは、Windows Server 2003 SP2、Windows XP SP3、および Windows XP x64 SP2 と互換性があります。

**公式 Discord サーバー**: https://discord.gg/eRcGuFtn6p

**警告**
これは、それぞれのシステムから変更されたファイルを使用し、まだテストまたは実験段階にある他のファイルを含み、開発者が一人しかいないソフトウェアです。言い換えれば、あらゆる種類のコンピューターや仮想マシンで起こりうるすべてのシナリオを予測することは不可能です。XP/2003からVistaの間に、新しいAPI、新しい技術、既存のAPIへの修正が最も飛躍的に進んだため、NT6と同じレベルの互換性をNT5で持つことは非常に困難です。落ち着いて慎重になり、このソフトウェアが悪い、または「クソ」と言う前に、問題をissuesに報告してください。できるだけ早く分析して、問題を修正するように努めます。助けてください。ソフトウェアに不満を言ったり、中傷したりしても、誰の役にも立ちません。

**注意**:
-Windows XP 非公式 SP4 およびおそらく Integral Edition はサポートされていません。

**言語サポート**
Windows のほとんどの部分で、ポルトガル語 (ブラジル) (私の言語)、ポルトガル語 (ポルトガル)、トルコ語、中国語 (繁体字および簡体字)、フランス語、イタリア語、ハンガリー語、ウクライナ語 (一部)、スペイン語、ポーランド語、ロシア語、韓国語がサポートされています。

**このリポジトリ内のフォルダー:**
- Documents: プロジェクトのドキュメント、既知のバグ、sfxcab の使用法 (インストーラー作成用) など。
- Packages\x86 および Packages\x64: パッケージごとに分類されたバイナリ リリース。 ここからパッケージを直接ダウンロードしてインストール/更新できます (つまり、Packages\x86\Base installer\update に移動して update.exe を実行します)。
- Todo: やるべきタスク
- Test: テスト用のいくつかのバイナリとドキュメント。
- release: 新しいバイナリ リリースを生成するスクリプト。
- Output: バイナリ出力。リリース フォルダーのスクリプトを使用して生成できます。

**One-Core-API バイナリ プロジェクトは次のパッケージで構成されています:**
 警告: OCA パッケージの再起動が必要な場合は、必ず実行してください。すべてのパッケージをインストールし、最後のパッケージだけを再起動すると、Windows が破損します。 
- **Base**: One-Core-API のメイン パッケージであり、App Compat と Driver Update を除く他のすべてのパッケージで必要であり、このプロジェクトで使用されるすべてのラッパー (kernelbase や ntext など) が含まれています。
- **Additional Dlls**: Windows の以降のバージョンで導入されたいくつかの新しい DLL。
- **D3d**: D3D ランタイム (主に WineD3D に基づく DX10 および DX11);
- **App Compat**: Windows の新しいバージョンからバックポートされたアプリケーション互換性設定。
- **Driver Update**: ACPI 2.0 をサポートする最新の acpi ドライバー、Storachi (AHCI ドライバー コントローラー用)、NVME (NVME M.2 ドライバー コントローラー用)、USBXHCI (USB 3.0) などの新しいドライバーを導入します。
- **Branding**: Windows Vista で導入された新しいブランド システム。D3D および Modern Setup パッケージに必要です。

**パッケージのインストール順序:**
- **一般的な順序**:  Base-> Additional Dlls -> Branding -> D3d -> Driver Update -> App Compat;

- **スタンドアロン パッケージ (必要な場合のみ、他のパッケージを一緒に使用する必要はありません)**
   - **App Compat のみ:** このパッケージは、他のパッケージなしでインストールできます。
   - **Driver Updateのみ**: このパッケージは、他のパッケージなしでインストールできます。
   - **AppCompat を最初に**: このパッケージは、他のパッケージなしでインストールできます。

**主な特徴**：
- x86では128GB、x64では2TBのメモリをデフォルトでサポート；
- 最新のWindows OS用に設計された新しいプログラムの実行をサポートする；
- 新しいドライバーコントローラーを備えた新しいハードウェアをサポートします。
  
**One-Core-API を使用すると、次のものを実行できます。**
- JetBrains WebStorm 2018
- Intellij 2018 (他のバージョンも動作する可能性があります)
- JetBrains WebStorm 2023.x.x (現時点では Windows XP x64 のみ)
- JetBrains IntelliJ 2023.x.x (現時点では Windows XP x64 のみ)
- Adob​​e Photoshop CC 2018
- Filezilla (最新)
- Discord 0.309.0
- Visual Studio Code 1.83.1 まで
- Chrome バージョン 123 まで
- Opera バージョン 106 まで
- Firefox バージョン 122 まで (一部バグあり)
- Microsoft Edge バージョン 122 まで
- Brave バージョン 1.62.153 まで (Chromium 121)
- Yandex Browser 最新バージョン
- Thorium ブラウザ 109 まで
- Supermium バージョン 117 まで
- Seamonkey バージョン 2.53.10 まで
- Thunderbird バージョン 121 まで
- Maxthon バージョン 7.1.6 まで
- Vivaldi 最新バージョンまで
- JDK 1.8 (現時点では Windows XP x64 のみ)
- Java Alternative JDK または OpenJDK バージョン 21 まで (他のバージョンでも動作する可能性があります)。ダウンロードは次のサイトから行えます: https://bell-sw.com/pages/downloads/#/java-11-lts
-  Epic Browser 120
- Python 3.6
- .Net Framework バージョン 4.8 まで
- Geekbench 4.2
- Performance Test
- Adob​​e Reader DC (2018)
- Foxit PDF Reader (2023)
- Windows 7 ゲーム
- Windows 7 ペイント
- Windows 7 ワードパッド
- Windows Vista ネイティブ アプリケーション
- Spotify Windows 7 および Windows 10 バージョン
- Yandex ブラウザー (最新)
- Zoom
- その他のアプリケーション
- Node 10.24
- Telegram デスクトップ
- Winrar 7.0 Beta 4 (最新)
- Directx 9EX、10、11 ゲーム:
- Need for Speed Most Wanted 2012
- Need for Speed The Run
- Street Figther V
- Injustice: Gods among us
- Assassign Creed Black Flag
- Crysis 1、2、3 (directx 10-11 モード)
- Kate 23.08.1 (現時点では Windows XP x64 のみ)
  
**既知の制限事項:**
- Firefox 56 以降を実行すると、ページの読み込みの問題が発生する可能性があります。これを解決するには、バージョン 55 から 72 では、MOZ_FORCE_DISABLE_E10S という環境変数を作成する必要があります。これはユーザーの環境変数のキーとなり、値は 1 になります。バージョン 73 以降では、値はバージョン番号、つまり 73.0.0 などになります (バージョン番号を確認してください)。
- ダイアログやウィンドウの一部が英語のみで表示されます。国際化は現在進行中です。インストールされた mui パックに関する問題:
- Chrome、Maxthon、Discord、Team viewer などの新しいアプリケーションのインストーラーが機能しない場合があります。クラッシュしてアプリケーションがインストールされません。他のオペレーティング システムからコピーされたプリインストール バージョンを使用する必要があります。
- パッケージは、標準の Microsoft バージョンではなく、「SFXCAB Substitute」というツールを使用しているため、nlite で Windows Iso に統合できません。
- 4.6 以降の標準 .Net Framework インストーラーは、現時点ではサポートされていません。https://github.com/abbodi1406/dotNetFx4xW7 のように再パックされたバージョンが必要です。また、こちらで入手できます: https://www.wincert.net/forum/topic/13805-microsoft-net-framework-472-full-x86x64-incl-language-packs-by-ricktendo/#comment-123251 他のバージョンも入手できます。フォーラムのトピックで検索してください。
- palemon の新しいバージョンでは、サイド バイ サイド構成エラーの問題が発生する可能性があります。
- Opera 39 - 50 を起動するには、次のパラメーターが必要になる場合があります: --disable-gpu (黒い画面が表示されないようにするため) および --single-process (最初のページが永遠に読み込まれないようにするため);

**問題のレポート**
問題を再現するには、**常に**このテンプレートに従うことをお勧めします:
- 何が起こったかの説明。
例: Windows をロードしようとすると常に BSOD が発生する。Windows がブート画面で停止する。Windows が常に黒い画面になるなど。
- VM/PC 構成
例: Vmware 10、virtualBox 6.1.0 または Core 2 Duo 8400、2Gb DDR2、IDE/SATA ハード ディスク。
- Windows のエディションと構成
例: Windows XP Service Pack 3 に POSReady 2009 更新プログラムをインストールし、次のプログラムをインストールしています: Adob​​e、Office 

XP/Server 2003 で実行されているアプリケーションのスクリーンショット:
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

**Avast and Chromium 68**
![Avast](https://user-images.githubusercontent.com/5159776/178078208-c13b3448-ee6a-4c56-9d94-d0c62d51949e.PNG)

**Windows 7 Sticky Notes**
![StickyNotes](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/669ba3e4-b831-4a96-ad40-d87e3e9531e2)

**Windows 7 Paint**
![Paint](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/81728a44-c9e7-41e8-b68b-8ea7b119ebba)

**Windows 7 Wordpad**
![Wordpad](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/9dac02c7-7139-47fe-8732-ccd9ef91090b)



