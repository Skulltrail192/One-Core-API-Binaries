<p align="center">
  <a href="https://github.com/One-Core-API/One-Core-API/releases">
    <img src="Assets/banner.png" width="100%" height="auto" alt="Banner">
  </a>
</p>

***
**Linguagem:**    
[English](README.md) | [简体中文](README_CN.md) | [Русский](README_RU.md) | [Українська](README_UK.md) | [日本語](README_JP.md) | [Português do Brasil](README_BR.md)
***

Este repositório contém os lançamentos dos binários do projeto One-Core-API. Eles são compatíveis com o Windows Server 2003 SP2, Windows XP SP3 e Windows XP
x64 SP2.

**Servidor oficial no Discord**: https://discord.gg/eRcGuFtn6p

**NOTA**:
-Windows XP Unofficial SP4 e talvez Integral Edition podem não ser suportados!

**Suporte de línguas:**
Suporte de línguas para a maioria das partes do Windows em: Português do Brasil (minha língua), Português (Portugal), Turco, Chinês (Tradicional and Simplificado), Francês, Italiano, Húngaro, Ucraniano (parcial), Espanhol, Polonês, Russo e Coreano;

**Pastas neste repositório:**
- Documents: Documentação do projeto, bugs conhecidos, uso do sfxcab (para fazer instaladores), etc.
- Packages\x86 e Packages\x64: Arquivos binários categorizados por pacotes. Você pode baixar e instalar/atualizar os pacotes diretamente por aqui (exemplo: indo até Packages\x86\Base installer\update e rodando update.exe).
- Todo: Tarefas a serem feitas
- Test: Alguns binários e documentos para testes;
- Release: Scripts para gerar uma nova versão dos binários;
- Output: Saída dos binários, você pode gerá-los usando scripts na pasta Release;

**O projeto One-Core-API consiste nos seguintes pacotes:**
Atenção: Sempre que o pacote exigir a reinicialização do sistema, faça isso. Se você instalar todos os pacotes e reiniciar apenas no último, o Windows pode ser corrompido.  
- **Base**: Pacote principal do One-Core-API e é exigido por todos os outros pacotes, com exceção do App Compat e do Driver Update, e contém todos os wrappers usados neste projeto (como o kernelbase e o ntext);
- **Additional Dlls**: Várias novas DLLs usadas em versões mais recentes do Windows.
- **D3d**: Runtime do D3D (Sobretudo DX10 e DX11, baseados no WineD3D);
- **App Compat**: Opções de compatibilidade do aplicativo, portadas de versões posteriores do Windows.
- **Driver Update**: Contém um driver ACPI mais recente. Suporta ACPI 2.0, novos drivers como Storahci (para controladoras AHCI), NVME (para controladoras NVME M.2  e USBXHCI (USB 3.0); 
- **Branding**: Novo sistema de marcas introduzido no Windows Vista, exigido para a instalação dos pacotes D3D e Modern Setup;

**Ordem de instalação dos pacotes:**
- **Ordem comum**: Base-> Additional Dlls -> API-SET -> Branding -> D3d -> Modern Setup -> Driver Update -> App Compat;

- **Pacotes separados (não requer a instalação de outros pacotes, somente se você desejar)**
  - **Somente App Compat:** Este pacote pode ser instalado sem a necessidade de outros pacotes; 
  - **Somente Driver Update**: Este pacote pode ser instalado sem a necessidade de outros pacotes; 
  - **AppCompat primeiro**: Este pacote pode ser instalado sem a necessidade de outros pacotes. 

**Recursos principais**:
- Aumento da memória máxima para 128 GB (x86) e 2 TB (x64) por padrão;
- Permite suporte para novos programas feitos para versões mais recentes do Windows;
- Permite suporte para hardware mais recente com novas controladoras de drivers;

**One-Core-API permite você executar:**
- JetBrains WebStorm 2018;
- Intellij 2018 (Outras versões também podem funcionar);
- JetBrains WebStorm 2023.x.x (Somente Windows XP x64 por enquanto)
- JetBrains IntelliJ 2023.x.x (Somente Windows XP x64 por enquanto)
- Adobe Photoshop CC 2018;
- Filezilla (latest);
- K-Lite Codec Pack última versão;
- Visual Studio Code até a versão 1.83.1;
- Chrome até a versão 123!;
- Opera até a versão 105;
- Firefox até a versão 116 (com bugs e precisa de soluções alternativas);
- Microsoft Edge até a versão 122;
- Brave até a versão 117;
- Yandex Browser última versão;
- Thorium browser até a versão 109;
- Supermium até a versão 117;
- Seamonkey até a versão 2.53.10;
- Thunderbird até a versão 115;
- Maxthon até a versão 7.1.6;
- Vivaldi última versão;
- JDK 1.8 (Somente Windows XP x64 por enquanto);
- Java Alternative JDK ou OpenJDK até a versão 21 (outras versões talvez funcionem). Você pode baixar aqui: https://bell-sw.com/pages/downloads/#/java-11-lts;
- Epic Browser 94;
- Python 3.6;
- .Net Framework até a versão 4.8;
- Geekbench 4.2;
- Performance Test;
- Adobe Reader DC (2017);
- Jogos do Windows 7;
- Aplicativos nativos do Windows Vista;
- Spotify após o suporte do Windows XP/Vista, versão 1.0.60;
- Yandex browser (última versão);
- Zoom;
- Várias outras aplicações;
- Node 10.24;
- Winrar 6.24 (última versão);
- Jogos do DirectX 9EX, 10 e 11: 
  - Need for Speed Most Wanted 2012;
  - Need for Speed The Run;
  - Street Figther V;
  - Injustice: Gods among us;
  - Assassign Creed Black Flag;
  - Crysis 1,2, and 3 (modo directx 10-11);
- Kate 23.08.1 (Somente Windows XP x64 por enquanto)
  
**Limitações conhecidas:**
- Diálogos de Abrir e Salvar de várias aplicações não funcionam perfeitamente porque são implementações do Vista, chamadas IFileDialog. Trabalho em progresso;
- Você pode ter problemas de carregamento de páginas no Firefox 56+. Para resolver isso, nas versões 55 até a 72, você precisa criar uma variável de ambiente chamada MOZ_FORCE_DISABLE_E10S. Ela será a chave da variável de ambiente de usuário, e o seu valor será 1. A partir da veresão 73, o valor será igual ao da versão, neste caso, 73.0.0, por exemplo (verifique o número da versão).
- Alguns lugares nas caixas de diálogo ou janelas podem ficar em inglês. Internacionalização ainda está em progresso.
- Instaladores de aplicativos modernos como o Chrome, Maxthon, Discord, Team viewer, etc. podem não funcionar e travar. É necessário usar uma versão pré-instalada e copiada de outro sistema.
- Firefox 55-116 funcionando com algumas limitações;
- Firefox a partir da versão 54 (32 bit) não funciona no Windows XP x64. Chrome 61+ também não funciona;
- Estes pacotes não podem ser integradas em uma ISO do Windows, porque eles usam uma ferramenta chamada "SFXCAB Substitute", que é diferente da versão da Microsoft;
- Instaladores do .Net Framework a partir da versão 4.6 não funcionam no momento. Você precisa de uma versão reembalada utilizando este guia: https://github.com/abbodi1406/dotNetFx4xW7. Disponível também neste link: https://www.wincert.net/forum/topic/13805-microsoft-net-framework-472-full-x86x64-incl-language-packs-by-ricktendo/#comment-123251. Outras versões também estão disponíveis, procure nos tópicos do fórum;
- Palemoon nas versões 27-31 não faz downloads;
- Opera 39 - 46 podem precisar dos seguintes parâmetros: --disable-gpu (para evitar tela preta) and --single-process(para evitar longos tempos de inicialização na primeira vez);
- Aplicativos Qt5 que utilizam Dwrite travam quando iniciados. Eles tem alguns problemas como tela preta se remover o Dwrite (no entanto, os aplicativos iniciam e podem ser utilizados)

**Relatar problemas**
Para ajudar a reproduzir problemas, é **sempre** recomendado seguir este padrão:
- Descrever o que aconteceu.
  Ex: Sempre dá tela azul no boot do Windows. Windows trava na tela de boot. Windows sempre trava com tela azul, etc;
- Configuração do PC ou máquina virtual
  Ex: VMware 10, VirtualBox 6.1.0 ou Core 2 Duo 8400, 2GB DDR2, HD IDE/SATA;
- Edição do Windows e configuração
  Ex: Windows XP Service Pack 3 com as atualizações POSReady 2009 instaladas, com esta lista de programas: Adobe, office, etc;

Alguns prints de aplicativos rodando no Windows XP/Server 2003:

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

**Spotify (para Windows 7)**
![Spotify-Windows7](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/09de7c20-8670-45dc-9471-a6db9349abd0)

**Visual Studio Code 1.81**
![VisualCode](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/b21748b9-25bb-412d-95b3-2219d2efdf42)

**Microsoft Chess 3D**
![Chess3d](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/bd1ad0c6-edde-4ff2-a6e0-074c7379fab6)

**Telegram 4.14**
![Telegram-Desktop](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/73e13167-49b8-4282-81cb-969435046dde)

**Libre Office 24 (última versão)**
![LibreOffice](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/11fd191d-270c-428d-8d41-0498e8fafb3b)

**Discord 0.309**
![Discord-Login](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/8a4c12b5-19fc-454d-b02a-a1db807d3900)
![Discord](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/eb673541-4e66-4c76-867e-346edbaaa0af)

**Zoom meeting**
![Zoom](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/d002cf1b-c5f4-4c0c-b629-00e031a56765)

**Java 11**
![Capturar](https://user-images.githubusercontent.com/5159776/178078132-da504607-a1ca-4f8d-ae25-6a7eb367bdaa.PNG)

**Avast e Chromium 68**
![Avast](https://user-images.githubusercontent.com/5159776/178078208-c13b3448-ee6a-4c56-9d94-d0c62d51949e.PNG)

**Notas Adesivas do Windows 7**
![StickyNotes](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/669ba3e4-b831-4a96-ad40-d87e3e9531e2)

**Paint do Windows 7**
![Paint](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/11f354d4-5392-4edd-95c7-13c6faa37362)

**Wordpad do Windows 7**
![Wordpad](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/9dac02c7-7139-47fe-8732-ccd9ef91090b)