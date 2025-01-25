<p align="center">
  <a href="https://github.com/One-Core-API/One-Core-API/releases">
    <img src="Assets/banner.png" width="100%" height="auto" alt="Banner">
  </a>
</p>

***
**Мова:**
[English](README.md) | [简体中文](README_CN.md) | [Русский](README_RU.md) | [Українська](README_UK.md) | [日本語](README_JP.md) | [Português-Brasil](README_BR.md)
***

**Цей репозиторій містить бінарні випуски проєкту One-Core-API. Вони сумісні з Windows Server 2003 SP2, Windows XP SP3 та Windows XP x64 SP2.**

- [Основні можливості](#основні-можливості)
  - [Перед використанням цього програмного забезпечення](#перед-використанням-цього-програмного-забезпечення)
- [Сумісність додатків](#сумісність-додатків)
- [Відомі обмеження](#відомі-обмеження)
- [Перед тим, як повідомити про проблему...](#перед-тим-як-повідомити-про-проблему)
- [Структура репозиторію](#структура-репозиторію)
- [Додаткова інформація та посилання](#додаткова-інформація-та-посилання)
  - [Офіційний сервер Discord](#офіційний-сервер-discord)
- [Демонстрація / Підтвердження концепції](#демонстрація--підтвердження-концепції)

## Основні можливості
- **Збільшення підтримки пам'яті до 128 ГБ для x86 та 2 ТБ для x64 за замовчуванням;**
- **Дозволяє підтримку запуску нових програм, розроблених для сучасних ОС Windows;**
- **Дозволяє підтримку нового обладнання з новими драйверами контролерів;**
- **Багатомовна підтримка для всіх підтримуваних мов Windows XP та Windows Server 2003;**

### Перед використанням цього програмного забезпечення
> Це програмне забезпечення використовує модифіковані файли з відповідних систем, а також включає файли, які все ще знаходяться на стадії тестування або експериментування, і **розробляється однією особою**. Отже, передбачити всі можливі сценарії для різних конфігурацій комп'ютерів або віртуальних машин неможливо.
>
> <h4>Перехід між Windows XP/2003 та Vista ознаменував значний стрибок у розробці нових API, технологій та модифікацій існуючих API. Це ускладнює досягнення такого ж рівня сумісності між системами NT 5.x та NT 6.x.</h4>
>
> Я прошу вас зберігати спокій та обережність.
> Перш ніж зробити висновок, що програмне забезпечення має недоліки або "погану якість", **будь ласка, повідомте про будь-які проблеми, з якими ви зіткнулися, через Github Issues або [сервер Discord](https://discord.gg/eRcGuFtn6p)**.
>
> Зверніть увагу, що я не можу гарантувати, що всі проблеми будуть вирішені. Однак я їх проаналізую і докладу всіх зусиль, щоб їх виправити.
>
> Ваша допомога є цінною, а скарги або негативні відгуки не сприяють покращенню продукту.

## Сумісність додатків
- JetBrains IDE до останніх випусків (2024);
- Android Studio до останньої версії;
- NetBeans до останньої версії;
- Eclipse IDE до останньої версії;
- Продукти Adobe (Photoshop, Illustrator, Dreamweaver тощо) до версії 2019;
- Filezilla (остання версія);
- LibreOffice 24.0.x (остання версія);
- Discord 0.309.0;
- Legocord (форк Discord) останній
- Visual Studio 2012 та Visual Studio 2013;
- Visual Studio Code (та форки типу Codium) до останньої версії;
- Chromium браузери (Chrome, Opera, Edge та інші) до останньої версії;
- Gecko based (Firefox, Zen Browser) до останніх версій (однак youtube працює тільки до версії Firefox 130);
- Seamonkey до версії 2.53.10;
- Thunderbird до останньої версії;
- Maxthon до версії 7.1.6;
- Vivaldi до останньої версії;
- JDK 1.8 (тільки Windows XP x64 на даний момент);
- Java JDK та альтернативні JDK або OpenJDK до версії 24 (можливо, працюють і інші версії). Ви можете завантажити з: https://bell-sw.com/pages/downloads/#/java-11-lts;
- Epic Browser 120;
- Python 3.6 (3.8/3.9 також може працювати, лише [mod](https://mega.nz/folder/KxExlAiC#L9rAQ5kTCtlHgZUwaxMpgw) версія);
- .Net Framework до 4.8;
- .NET 6.0
- Geekbench 4.2;
- Performance Test;
- Adobe Reader DC (до 2024);
- Foxit PDF Reader (2023);
- Ігри з Windows 7;
- Paint з Windows 7;
- Wordpad з Windows 7;
- Нативні програми з Windows Vista;
- Spotify для Windows 7 та для Windows 10;
- Line;
- Zoom;
- Node 10.24;
- Telegram Desktop;
- Winrar 7.0 (остання);
- Postman;
- Insominia;
- Dbeaver;
- TeamViewer 14;
- Ігри Directx 9EX, 10 та 11:
  - Need for Speed Most Wanted 2012;
  - Need for Speed The Run;
  - Street Figther V;
  - Injustice: Gods among us;
  - Assassign Creed Black Flag;
  - Crysis 1,2 та 3 (режим directx 10-11);
  - GTA Trilogy;
  - GTA V;
- Kate 23.08.1

## Відомі обмеження
- Youtube у Firefox, починаючи з версії 131, не працює. Тому рекомендовано використовувати версію 115 або 128 ESR.
- Деякі інсталятори застосунків можуть не працювати, як-от electron based застосунки (MS teams), Office 2013 або GIMP 3.0 RC2 тощо, аварійно завершуються і додаток не встановлюється. Деякі інсталятори та програми вимагають підтримки AVX операційною системою, що не підтримується взагалі. Тому, використовуйте попередньо встановлену версію, скопійовану з іншої операційної системи;
- Пакети не можна інтегрувати через nlite в ISO Windows, оскільки використовується інструмент під назвою "SFXCAB Substitute", а не стандартна версія від Microsoft;
- Стандартні інсталятори .Net Framework, починаючи з 4.6, не підтримуються на даний момент. Вам потрібна перепакована версія, як зроблено тут: https://github.com/abbodi1406/dotNetFx4xW7. І доступно тут: https://www.wincert.net/forum/topic/13805-microsoft-net-framework-472-full-x86x64-incl-language-packs-by-ricktendo/#comment-123251. Інші версії також доступні, шукайте у темах форуму;
- Нові версії palemoon можуть зіткнутися з проблемою помилки паралельної конфігурації;
- Opera 39-50 може потребувати наступних параметрів для запуску: --disable-gpu (щоб запобігти чорному екрану) та --single-process (щоб запобігти вічному завантаженню першої сторінки);

## Перед тим, як повідомити про проблему...

**Перш ніж повідомляти про проблему, я наполегливо рекомендую перевірити, чи існує проблема в [One-Core-API-Canary](https://github.com/shorthorn-project/One-Core-API-Binaries-Canary) і уважно переглянути існуючі [проблеми](https://github.com/Skulltrail192/One-Core-API-Binaries/issues), щоб перевірити, чи ваша проблема вже повідомлялася.**

**Якщо проблема *існує* в Canary і ще не вказана в поточних проблемах, будь ласка, надайте докладний звіт про вашу проблему, включаючи наступну інформацію:**

**1. Конфігурація системи:**
*   **Тип** (ПК/ВМ)
*   **Редакція ОС Windows** (наприклад, Windows XP Professional Service Pack 3)
*   **Оновлення після SP:**
    *   Чи були встановлені оновлення після Service Pack оновлень? (Так/Ні)
        *   Якщо так, вкажіть, чи вони були встановлені **до** чи **після** встановлення One-Core-API.
*   **Встановлене програмне забезпечення:** Перерахуйте будь-яке відповідне програмне забезпечення, яке може бути пов’язане з проблемою (наприклад, Adobe Photoshop CC 2018, Firefox 132 тощо).
*   **Технічні характеристики**:
    *   **Якщо фізичне обладнання:** Надайте подробиці про процесор, ОЗП (тип, обсяг), тип жорсткого диска та його ємність (наприклад, диск IDE, 120 ГБ).
    *   **Якщо віртуальна машина:** Укажіть деталі конфігурації віртуальної машини (наприклад, Oracle VirtualBox 6.1.0, 2 ГБ ОЗП, диск 120 ГБ, режим AHCI).
*   **Кроки для відтворення помилки**

> **ВАЖЛИВО:** Якщо можливо, будь ласка, також **додайте будь-які відповідні журнали** до звернення. Це значно допоможе швидше ідентифікувати рішення.

> Також доцільно додати відеозапис проблеми, якщо це можливо.
>
> **Зверніть увагу, що звернення буде закрито, якщо проблему не можна відтворити.**

## Структура репозиторію
- Documents: Документація проєкту, відомі помилки, використання sfxcab (для створення інсталяторів) тощо.
- Packages\x86 та Packages\x64: Бінарні випуски, згруповані за пакетами. Ви можете завантажити та встановити/оновити пакети безпосередньо звідси (тобто, перейшовши до Packages\x86\Base installer\update та запустивши update.exe).
- Todo: Завдання для виконання
- Test: Деякі бінарні файли та документи для тестування;
- Release: Скрипти для створення нового бінарного випуску;
- Output: Вихідні бінарні файли, які можна згенерувати за допомогою скриптів у папці Release;

## Додаткова інформація та посилання
**Розширені можливості для систем на основі One-Core-API:**

<b><a href="https://github.com/shorthorn-project/One-Core-API-Extras" style="font-size: 18px">https://github.com/shorthorn-project/One-Core-API-Extras</a></b>

**Інструменти для нової системи розгортання для систем на основі One-Core-API:**

<b><a href="https://github.com/Skulltrail192/One-Core-API-Tools" style="font-size: 18px">https://github.com/Skulltrail192/One-Core-API-Tools</a></b>

### Офіційний сервер Discord

**Якщо ви хочете приєднатися до нашого офіційного сервера Discord One-Core-API, ви можете приєднатися тут:**

<b><a href="https://discord.gg/eRcGuFtn6p" style="font-size: 25px">https://discord.gg/eRcGuFtn6p</a></n>

## Демонстрація / Підтвердження концепції
Деякі знімки екрана додатків, що працюють на XP/Server 2003:

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

**Libre Office 24 (остання)**
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

**Paint з Windows 7**
![Paint](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/81728a44-c9e7-41e8-b68b-8ea7b119ebba)

**Wordpad з Windows 7**
![Wordpad](https://github.com/Skulltrail192/One-Core-API-Binaries/assets/5159776/9dac02c7-7139-47fe-8732-ccd9ef91090b)
