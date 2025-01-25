<p align="center">
  <a href="https://github.com/One-Core-API/One-Core-API/releases">
    <img src="Assets/banner.png" width="100%" height="auto" alt="Баннер">
  </a>
</p>

***
**Язык:**
[English](README.md) | [简体中文](README_CN.md) | [Русский](README_RU.md) | [Українська](README_UK.md) | [日本語](README_JP.md) | [Português-Brasil](README_BR.md)
***

**Этот репозиторий содержит бинарные выпуски проекта One-Core-API. Они совместимы с Windows Server 2003 SP2, Windows XP SP3 и Windows XP x64 SP2.**

- [Основные возможности](#основные-возможности)
  - [Перед использованием этого программного обеспечения](#перед-использованием-этого-программного-обеспечения)
- [Совместимость приложений](#совместимость-приложений)
- [Известные ограничения](#известные-ограничения)
- [Перед тем, как сообщить о проблеме...](#перед-тем-как-сообщить-о-проблеме)
- [Структура репозитория](#структура-репозитория)
- [Дополнительная информация и ссылки](#дополнительная-информация-и-ссылки)
  - [Официальный сервер Discord](#официальный-сервер-discord)
- [Демонстрация / Подтверждение концепции](#демонстрация--подтверждение-концепции)

## Основные возможности
- **Увеличение поддержки памяти до 128 ГБ для x86 и 2 ТБ для x64 по умолчанию;**
- **Позволяет поддержку запуска новых программ, разработанных для современных ОС Windows;**
- **Позволяет поддержку нового оборудования с новыми драйверами контроллеров;**
- **Многоязычная поддержка для всех поддерживаемых языков Windows XP и Windows Server 2003;**

### Перед использованием этого программного обеспечения
> Это программное обеспечение использует модифицированные файлы из соответствующих систем, а также включает файлы, которые все еще находятся на стадии тестирования или экспериментирования, и **разрабатывается одним человеком**. Следовательно, предсказать все возможные сценарии для различных конфигураций компьютеров или виртуальных машин невозможно.
>
> <h4>Переход между Windows XP/2003 и Vista ознаменовал значительный скачок в разработке новых API, технологий и модификаций существующих API. Это затрудняет достижение такого же уровня совместимости между системами NT 5.x и NT 6.x.</h4>
>
> Я прошу вас сохранять спокойствие и осторожность.
> Прежде чем сделать вывод, что программное обеспечение имеет недостатки или "плохое качество", **пожалуйста, сообщите о любых проблемах, с которыми вы столкнулись, через Github Issues или [сервер Discord](https://discord.gg/eRcGuFtn6p)**.
>
> Обратите внимание, что я не могу гарантировать, что все проблемы будут решены. Однако я их проанализирую и приложу все усилия, чтобы их исправить.
>
> Ваша помощь ценна, а жалобы или негативные отзывы не способствуют улучшению продукта.

## Совместимость приложений
- JetBrains IDE до последних выпусков (2024);
- Android Studio до последней версии;
- NetBeans до последней версии;
- Eclipse IDE до последней версии;
- Продукты Adobe (Photoshop, Illustrator, Dreamweaver и т. д.) до версии 2019;
- Filezilla (последняя версия);
- LibreOffice 24.0.x (последняя версия);
- Discord 0.309.0;
- Legocord (форк Discord) последний
- Visual Studio 2012 и Visual Studio 2013;
- Visual Studio Code (и форки типа Codium) до последней версии;
- Chromium браузеры (Chrome, Opera, Edge и другие) до последней версии;
- Gecko based (Firefox, Zen Browser) до последних версий (однако youtube работает только до версии Firefox 130);
- Seamonkey до версии 2.53.10;
- Thunderbird до последней версии;
- Maxthon до версии 7.1.6;
- Vivaldi до последней версии;
- JDK 1.8 (только Windows XP x64 на данный момент);
- Java JDK и альтернативные JDK или OpenJDK до версии 24 (возможно, работают и другие версии). Вы можете скачать с: https://bell-sw.com/pages/downloads/#/java-11-lts;
- Epic Browser 120;
- Python 3.6 (3.8/3.9 также может работать, только [mod](https://mega.nz/folder/KxExlAiC#L9rAQ5kTCtlHgZUwaxMpgw) версия);
- .Net Framework до 4.8;
- .NET 6.0
- Geekbench 4.2;
- Performance Test;
- Adobe Reader DC (до 2024);
- Foxit PDF Reader (2023);
- Игры Windows 7;
- Paint Windows 7;
- Wordpad Windows 7;
- Нативные приложения из Windows Vista;
- Spotify для Windows 7 и для Windows 10;
- Line;
- Zoom;
- Node 10.24;
- Telegram Desktop;
- Winrar 7.0 (последняя);
- Postman;
- Insominia;
- Dbeaver;
- TeamViewer 14;
- Игры Directx 9EX, 10 и 11:
  - Need for Speed Most Wanted 2012;
  - Need for Speed The Run;
  - Street Figther V;
  - Injustice: Gods among us;
  - Assassign Creed Black Flag;
  - Crysis 1,2 и 3 (режим directx 10-11);
  - GTA Trilogy;
  - GTA V;
- Kate 23.08.1

## Известные ограничения
- Youtube в Firefox, начиная с версии 131, не работает. Поэтому рекомендуется использовать версию 115 или 128 ESR.
- Некоторые установщики приложений могут не работать, например, electron based приложения (MS teams), Office 2013 или GIMP 3.0 RC2 и т.д., аварийно завершаются и приложение не устанавливается. Некоторые установщики и программы требуют поддержки AVX операционной системой, что не поддерживается вовсе. Поэтому используйте предварительно установленную версию, скопированную из другой операционной системы;
- Пакеты нельзя интегрировать через nlite в ISO Windows, так как используется инструмент под названием "SFXCAB Substitute", а не стандартная версия от Microsoft;
- Стандартные установщики .Net Framework, начиная с 4.6, не поддерживаются на данный момент. Вам нужна перепакованная версия, как сделано здесь: https://github.com/abbodi1406/dotNetFx4xW7. И доступно здесь: https://www.wincert.net/forum/topic/13805-microsoft-net-framework-472-full-x86x64-incl-language-packs-by-ricktendo/#comment-123251. Другие версии также доступны, ищите в темах форума;
- Новые версии palemoon могут столкнуться с проблемой ошибки параллельной конфигурации;
- Opera 39-50 может потребовать следующих параметров для запуска: --disable-gpu (чтобы предотвратить черный экран) и --single-process (чтобы предотвратить вечную загрузку первой страницы);

## Перед тем, как сообщить о проблеме...

**Прежде чем сообщать о проблеме, я настоятельно рекомендую проверить, существует ли проблема в [One-Core-API-Canary](https://github.com/shorthorn-project/One-Core-API-Binaries-Canary) и внимательно просмотреть существующие [проблемы](https://github.com/Skulltrail192/One-Core-API-Binaries/issues), чтобы проверить, сообщалось ли о вашей проблеме.**

**Если проблема *существует* в Canary и еще не указана в текущих проблемах, пожалуйста, предоставьте подробный отчет о вашей проблеме, включая следующую информацию:**

**1. Конфигурация системы:**
*   **Тип** (ПК/ВМ)
*   **Редакция ОС Windows** (например, Windows XP Professional Service Pack 3)
*   **Обновления после SP:**
    *   Были ли установлены обновления после Service Pack обновлений? (Да/Нет)
        *   Если да, укажите, были ли они установлены **до** или **после** установки One-Core-API.
*   **Установленное программное обеспечение:** Перечислите любое соответствующее программное обеспечение, которое может быть связано с проблемой (например, Adobe Photoshop CC 2018, Firefox 132 и т. д.).
*   **Технические характеристики**:
    *   **Если физическое оборудование:** Предоставьте подробности о процессоре, ОЗУ (тип, объем), типе жесткого диска и его емкости (например, диск IDE, 120 ГБ).
    *   **Если виртуальная машина:** Укажите детали конфигурации виртуальной машины (например, Oracle VirtualBox 6.1.0, 2 ГБ ОЗУ, диск 120 ГБ, режим AHCI).
*   **Шаги для воспроизведения ошибки**

> **ВАЖНО:** Если возможно, пожалуйста, также **добавьте любые соответствующие журналы** к обращению. Это значительно поможет быстрее идентифицировать решение.

> Также целесообразно добавить видеозапись проблемы, если это возможно.
>
> **Обратите внимание, что обращение будет закрыто, если проблему нельзя воспроизвести.**

## Структура репозитория
- Documents: Документация проекта, известные ошибки, использование sfxcab (для создания установщиков) и т.д.
- Packages\x86 и Packages\x64: Бинарные выпуски, сгруппированные по пакетам. Вы можете загрузить и установить/обновить пакеты непосредственно отсюда (то есть, перейдя к Packages\x86\Base installer\update и запустив update.exe).
- Todo: Задачи для выполнения
- Test: Некоторые бинарные файлы и документы для тестирования;
- Release: Скрипты для создания нового бинарного выпуска;
- Output: Выходные бинарные файлы, которые можно сгенерировать с помощью скриптов в папке Release;

## Дополнительная информация и ссылки
**Расширенные возможности для систем на основе One-Core-API:**

<b><a href="https://github.com/shorthorn-project/One-Core-API-Extras" style="font-size: 18px">https://github.com/shorthorn-project/One-Core-API-Extras</a></b>

**Инструменты для новой системы развертывания для систем на основе One-Core-API:**

<b><a href="https://github.com/Skulltrail192/One-Core-API-Tools" style="font-size: 18px">https://github.com/Skulltrail192/One-Core-API-Tools</a></b>

### Официальный сервер Discord

**Если вы хотите присоединиться к нашему официальному серверу Discord One-Core-API, вы можете присоединиться здесь:**

<b><a href="https://discord.gg/eRcGuFtn6p" style="font-size: 25px">https://discord.gg/eRcGuFtn6p</a></n>

## Демонстрация / Подтверждение концепции
Некоторые снимки экрана приложений, работающих на XP/Server 2003:

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

**Spotify (Для Windows 7)**
![Spotify-Windows7](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/09de7c20-8670-45dc-9471-a6db9349abd0)

**Visual Studio Code 1.81**
![VisualCode](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/b21748b9-25bb-412d-95b3-2219d2efdf42)

**Microsoft Chess 3d**
![Chess3d](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/bd1ad0c6-edde-4ff2-a6e0-074c7379fab6)

**Telegram 4.14**
![Telegram-Desktop](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/73e13167-49b8-4282-81cb-969435046dde)

**Libre Office 24 (последняя)**
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

**Avast та Chromium 68**
![Avast](https://user-images.githubusercontent.com/5159776/178078208-c13b3448-ee6a-4c56-9d94-d0c62d51949e.PNG)

**Windows 7 Sticky Notes**
![StickyNotes](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/669ba3e4-b831-4a96-ad40-d87e3e9531e2)

**Paint Windows 7**
![Paint](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/81728a44-c9e7-41e8-b68b-8ea7b119ebba)

**Wordpad Windows 7**
![Wordpad](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/9dac02c7-7139-47fe-8732-ccd9ef91090b)
![Wordpad](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/9dac02c7-7139-47fe-8732-ccd9ef91090b)