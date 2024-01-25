**Добро пожаловать в бинарники One-Core-API!**
***
**Language:**    
[English](README.md) | [简体中文](README_CN.md) | [Русский](README_RU.md) | [Українська](README_UK.md) | [日本語](README_JP.md)
***

Этот репозиторий содержит бинарные версии проекта One-Core-API. Они совместимы с Windows Server 2003 SP2, Windows XP SP3 и Windows XP x64 SP2.

**Официальный Discord сервер**: https://discord.gg/eRcGuFtn6p

**ПРИМЕЧАНИЕ**:
- Windows XP Unofficial SP4 и, возможно, Integral Edition не поддерживаются!

**Языковая поддержка**
Языковая поддержка почти всех частей Windows: Португальский-Бразилия (мой язык), Португальский (Portugual), Турецкий, Китайский (традиционный и упрощенный), Французский, Итальянский, Венгерский, Украинский (частично), Испанский, Польский, Русский и Корейский;

**Директории в этом репозитории**
- Documents: Документация по проекту, известные ошибки, использование sfxcab (для создания инсталляторов) и т.д.
- Packages\x86 и Packages\x64: Бинарные файлы, которые разделены по пакетам. Вы можете скачать и установить/обновить пакеты прямо отсюда (т.е. перейдя в Packages\x86\Base installer\update и запустив update.exe).
- Todo: To do задания
- Test: Некоторые бинарные файлы и документы для тестирования;
- Release: Скрипты для генерации нового выпуска бинарников;
- Output: Выходные двоичные файлы, можно сгенерировать с помощью скриптов в папке Release;

**Проект One-Core-API Binaries состоит из следующих пакетов:**
Внимание: Если пакет OCA требует перезагрузки, всегда выполняйте ее. Если вы установите все пакеты и перезапустите только последний, Windows **может быть** повреждена.
- **Base**: Основной пакет One-Core-API, который требуется всем остальным пакетам, кроме App Compat и Driver Update, и содержит все обёртки, используемые в этом проекте (например, kernelbase и ntext);
- **Additional Dlls**: Некоторые новые dll файлы, появившихся в более поздних версиях Windows.
- **D3d**: D3D (в основном DX10 и DX11, основано на WineD3D);
- **App Compat**: Параметры совместимости приложений (Application Compatibility), перенесенные из более поздних версий Windows.
- **Driver Update**: Обновленный драйвер acpi с поддержкой ACPI 2.0, новые драйверы Storachi (для контроллеров AHCI), NVME (для контроллеров NVME M.2) и USBXHCI (USB 3.0);
- **Branding**: Новая система брендинга (branding system), введенная в Windows Vista, необходимая для пакетов D3D и Modern Setup;

**Порядок установки пакетов:**
- **Обычный порядок**: Base -> Additional Dlls -> API-SET -> Branding -> D3d -> Modern Setup -> Driver Update -> App Compat;

- **Отдельные пакеты (не требуют установки других пакетов, только по желанию)**
  - **App Compat only:** Этот пакет может быть установлен без каких-либо других пакетов;
  - **Driver Update Only**: Этот пакет может быть установлен без каких-либо других пакетов;
  - **AppCompat first**: Этот пакет может быть установлен без каких-либо других пакетов;

**Основные особенности**:
- Увеличение поддержки памяти до 128 ГБ для x86 и 2 ТБ для x64 по умолчанию;
- Обеспечивает поддержку запуска новых программ, разработанных для современных ОС Windows;
- Обеспечение поддержки нового оборудования с помощью новых контроллеров с драйверами;

**One-Core-API позволяет запускать:**
- JetBrains WebStorm 2018;
- Intellij 2018 (Другие версии тоже могут работать);
- JetBrains WebStorm 2023.x.x (Пока только для Windows XP x64)
- JetBrains IntelliJ 2023.x.x (Пока только для Windows XP x64)
- Adobe Photoshop CC 2018;
- Filezilla (последняя версия);
- Visual Studio Code до версии 1.83.1;
- Chrome вплоть до версии 122!;
- Opera до версии 105;
- Firefox до версии 116 (с ошибками, требуюется обходные пути);
- Microsoft Edge вплоть до версии 115;
- Brave вплоть до версии 117;
- Yandex Browser последняя версия;
- Thorium browser вплоть до 109;
- Supermium вплоть до версии 117;
- Seamonkey вплоть до версии 2.53.10;
- Thunderbird вплоть до версии 115;
- Maxthon вплоть до 7.1.6;
- Vivaldi вплоть до последней версии;
- JDK 1.8 (Пока только для Windows XP x64);
- Java Alternative JDK или OpenJDK до версии 21 (возможно, другие версии работают). Вы можете скачать с сайта https://bell-sw.com/pages/downloads/#/java-11-lts;
- Maxthon 6;
- Epic Browser 94;
- Python 3.6;
- .Net Framework вплоть до 4.8;
- Geekbench 4.2;
- Performance Test;
- Adobe Reader DC (2017);
- Windows 7 игры;
- Родные приложения Windows Vista;
- Spotify после окончания поддержки Windows XP/Vista, версия 1.0.60;
- Yandex browser (последняя версия);
- Zoom;
- Некоторые другие приложения;
- Node 10.24;
- Winrar 6.24 (последняя);
- Directx 9EX, 10 и 11 игры: 
  - Need for Speed Most Wanted 2012;
  - Need for Speed The Run;
  - Street Figther V;
  - Injustice: Gods among us;
  - Assassign Creed Black Flgs;
  - Crysis 1,2, and 3 (directx 10-11 режим);
- Kate 23.08.1 (Пока только для Windows XP x64)


**Известные ограничения:**
- Диалоги открытия/сохранения в нескольких приложениях работают не полностью, поскольку реализация новой функции есть в Windows Vista, называемой IFileDialog. Работа продолжается;
- При использовании Firefox 56+ вы можете столкнуться с проблемами загрузки страниц. Чтобы решить эту проблему, начиная с версии 55 и заканчивая 72, необходимо создать переменную окружение под названием MOZ_FORCE_DISABLE_E10S со значением 1. Для версий, начиная с 73, значением будет номер версии, то есть, например, 73.0.0 (проверьте номер версии).
- Некоторые места в диалогах или окнах поддерживают только английский язык. Работа над интернационализацией продолжается;
- Установщики новых приложений могут не работать, например Chrome, Maxthon, Discord, Team viewer и т. д. падают, и приложение не устанавливается. Необходимо 
использовать предустановленную версию, скопированную из другой операционной системы;
- Исправлено в версии 3.0.4 ~~Некоторые программы установки, например VS Code и Skype, выдают ошибку "Class is not supported" при создании фазы ярлыка;~~
- Исправлено в OCA 3.0.3/3.0.4 ~~В Chrome наблюдается нестабильность и случайные сбои. Расширение пока не поддерживается. В зависимости от версии, при загрузке происходит сбой приложения или ошибка сети;~~
- Firefox 55-116 работает с некоторыми ограничениями;
- Firefox начиная с версии 54 (32 бит) не работает на Windows XP x64. Chrome 61+ также не работает;
- Пакеты не могут быть интегрированы с помощью nlite в Windows Iso, так как используют инструмент под названием "SFXCAB Substitute", а не стандартную версию Microsoft;
- Стандартные инсталляторы .Net Framework, начиная с 4.6, пока не поддерживаются. Вам нужна перепакованная версия, как здесь: https://github.com/abbodi1406/dotNetFx4xW7. И доступная, как здесь: https://www.wincert.net/forum/topic/13805-microsoft-net-framework-472-full-x86x64-incl-language-packs-by-ricktendo/#comment-123251. Другие версии тоже доступны, ищите в темах на форуме;
- Palemoon версии 27-31 ничего не скачивается. Загрузка не начинается;
- Для запуска Opera 39 - 46 могут потребоваться следующие параметры: --disable-gpu (для предотвращения черного экрана) и --single-process (для предотвращения вечной загрузки первой страницы);
- Приложения QT 5, использующие Dwrite, аварийно завершаются при запуске. Есть некоторые глюки, например, черный экран, если удалить dwrite (приложение запускается и может быть использован)

**Отчет о проблемах**
Для помощи в воспроизведении проблемы рекомендуется **всегда** следовать этому шаблону:
- Описание того, что произошло.
  Например: Всегда получайте BSOD при попытке загрузить Windows. Windows застряла на экране загрузки. Windows всегда получает черный экран и т. д;
- Конфигурация виртуальной машины/ПК
  Например: Vmware 10, virtualBox 6.1.0 или Core 2 Duo 8400, 2Gb DDR2, IDE/SATA Hard Disk;
- Версия Windows и конфигурация
  Например: Windows XP Service Pack 3 с установленными обновлениями POSReady 2009, с таким списком программ: Adobe, Office и т.д;

Несколько скриншотов приложений, работающих на XP/Server 2003:

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

**Spotify (Для Windows 7)**
![Spotify-Windows7](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/09de7c20-8670-45dc-9471-a6db9349abd0)

**Visual Studio Code 1.81**
![VisualCode](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/b21748b9-25bb-412d-95b3-2219d2efdf42)

**Microsoft Chess 3d**
![Chess3d](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/bd1ad0c6-edde-4ff2-a6e0-074c7379fab6)

**Telegram Desktop 4.14**
![Telegram-Desktop](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/73e13167-49b8-4282-81cb-969435046dde)

**Libre Office 24 (последняя версия)**
![LibreOffice](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/11fd191d-270c-428d-8d41-0498e8fafb3b)

**Discord 0.309**
![Discord-Login](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/8a4c12b5-19fc-454d-b02a-a1db807d3900)

![Discord](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/eb673541-4e66-4c76-867e-346edbaaa0af)

**Zoom meeting**
![Zoom](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/d002cf1b-c5f4-4c0c-b629-00e031a56765)

**Java 11**
![Capturar](https://user-images.githubusercontent.com/5159776/178078132-da504607-a1ca-4f8d-ae25-6a7eb367bdaa.PNG)

**Avast и Chromium 68**
![Avast](https://user-images.githubusercontent.com/5159776/178078208-c13b3448-ee6a-4c56-9d94-d0c62d51949e.PNG)

**Windows 7 Sticky Notes**
![StickyNotes](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/669ba3e4-b831-4a96-ad40-d87e3e9531e2)

**Windows 7 Paint**
![Windows7 Paint](https://user-images.githubusercontent.com/5159776/192194273-de70c23e-8731-4fb6-96c1-9bee98947269.PNG)

**Windows 7 Wordpad**
![Wordpad](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/9dac02c7-7139-47fe-8732-ccd9ef91090b)


