# 🤠 سامانه جامع بازی آنلاین UTGame (سریع‌ترین دست در خاورمیانه)

سامانه **UTGame** یک سیستم بازی آنلاین دو نفره نوبتی بر پایه دوئل‌های وسترن است که به زبان **++C** و با رعایت کامل اصول **طراحی شیءگرا (OOP)**، معماری چندفایلی و استاندارد **C++20** پیاده‌سازی شده است[cite: 2, 3]. این سامانه مدیریت کامل کاربران (بازیکنان و ناظران)، دو حالت مسابقه تفننی و رقابتی، مکانیزم نبرد، سیستم رتبه‌بندی و سطح‌بندی، مسدودسازی و قابلیت نظارت و جریمه متخلفان را فراهم می‌کند[cite: 2, 3].

---

## 🛠 ویژگی‌ها و مفاهیم کلی سیستم

* **مدیریت کاربران و سطح دسترسی:**
  * **بازیکنان (Players):** امکان ثبت‌نام، ورود/خروج، شرکت در مسابقات، مشاهده پروفایل، ارسال دعوت‌نامه، اعلام آمادگی، مسدودسازی سایر بازیکنان و گزارش متخلفین[cite: 2, 3].
  * **ناظران (Admins):** کاربران ویژه‌ای که فقط جهت نظارت، بررسی گزارش‌های تخلف، اعمال جریمه روی متخلفین یا رد گزارش‌ها وارد سامانه می‌شوند (قابلیت شرکت در بازی ندارند)[cite: 2, 3].
* **انواع مسابقات:**
  * **مسابقه تفننی (Casual Match):** مسابقه‌ای که شروع آن با **۱ جان** و **۱ تیر** است و پس از اتمام، **امتیاز تجربه ($XP$)** بازیکنان به‌روزرسانی می‌شود[cite: 2].
  * **مسابقه رقابتی (Ranked Match):** مسابقه‌ای که شروع آن با **۳ جان** و **۳ تیر** است، شرط برگزاری آن هم‌سطح بودن دو بازیکن است و پس از اتمام، **امتیاز رتبه‌بندی ($RP$)** تغییر می‌کند[cite: 3].
* **سطوح بازیکنان (Levels):**
  * **برنزی (Bronze):** $RP \le 1400$[cite: 3]
  * **نقره‌ای (Silver):** $1400 < RP \le 1750$[cite: 3]
  * **طلایی (Golden):** $1750 < RP \le 2250$[cite: 3]
  * **پلاتینیوم (Platinum):** $RP > 2250$[cite: 3]
  * (بازیکنان جدید با $RP = 1200$ و $XP = 500$ شروع می‌کنند)[cite: 2, 3].
* **مکانیزم نبردها:**
  در هر نوبت، هر بازیکن یکی از سه حرکت **شلیک (`shoot`)**، **دفاع (`defend`)** یا **دریافت تیر (`reload`)** را انتخاب می‌کند[cite: 2]. شلیک به حریفی که در حال `reload` است باعث اصابت تیر و صدمه/پیروزی می‌شود[cite: 2].
* **سیستم جریمه ناظران (Penalties):**
  ناظران می‌توانند برای بازیکنان متخلف، جریمه‌های زیر را روی بازی‌های رقابتی بعدی اعمال نمایند[cite: 3]:
  * **کاهش تیر اولیه (`bullet_penalty`):** کاهش ۱ تا ۳ تیر اولیه[cite: 3].
  * **کاهش جان اولیه (`health_penalty`):** کاهش ۱ تا ۲ جان اولیه[cite: 3].

---

## 💻 نحوه کامپایل و اجرای برنامه

این پروژه دارای **Makefile** است و طبق دستورالعمل با استاندارد **C++20** کامپایل می‌شود[cite: 2, 3].

### کامپایل:
make

### اجرا:
./UTGame <path_to_players_csv> <path_to_admins_csv>

---

## 🗂 فرمت فایل‌های ورودی (CSV)

### ۱. فایل اطلاعات بازیکنان (`players.csv`)[cite: 3]
username, password, XP, RP
pishikoochool, meooow, 1500, 2350
amir, italy, 900, 1600
iliya, ghasab, 1300, 1200

### ۲. فایل اطلاعات ناظران (`admins.csv`)[cite: 2]
username, password
yasdan, ghomayshi
arshiya, cityfan

---

## 📋 راهنمای جامع دستورات سیستم

تمامی دستورات با یکی از چهار متد `POST` ،`GET` ،`PUT` یا `DELETE` شروع شده و پارامترها پس از کاراکتر `?` قرار می‌گیرند[cite: 2].

### 👤 ۱. حساب کاربری و مدیریت ارتباطات
* **ثبت‌نام بازیکن جدید:**
  POST register ? username "<username>" password "<password>"
* **ورود به سامانه:**
  POST login ? username "<username>" password "<password>"
* **خروج از سامانه:**
  POST logout ?
* **مشاهده پروفایل کاربر:**
  GET profile ? username "<username>"
* **مسدود / رفع مسدودیت بازیکن:**
  POST block ? username "<username>" status "<blocked|unblocked>"

### ⚔️ ۲. مسابقات، دعوت‌نامه‌ها و جریان بازی
* **اعلام آمادگی برای مسابقه تفننی:**
  POST casual_match_ready ? status "<true|false>"
* **جستجوی رقبای تفننی:**
  GET casual_match_opponents ? sort_order "<asc|desc>"
* **جستجوی رقبای هم‌سطح در مسابقه رقابتی:**
  GET ranked_match_opponents ? sort_order "<asc|desc>"
* **ارسال دعوت‌نامه:**
  POST invitation ? username "<username>" match_type "<casual|ranked>"
* **مشاهده دعوت‌نامه‌های دریافتی:**
  GET received_invitations ?
* **شروع مسابقه با پذیرش دعوت‌نامه:**
  POST start_match ? invitation_id "<invitation_id>"
* **رد دعوت‌نامه:**
  POST reject_invitation ? invitation_id "<invitation_id>"
* **ثبت حرکت در نوبت بازی:**
  POST action ? action "<shoot|defend|reload>"
* **مشاهده وضعیت فعلی مسابقه:**
  GET match_status ?

### 🛡️ ۳. گزارش‌دهی و پنل نظارت
* **ثبت گزارش تخلف توسط بازیکن:**
  POST report ? username "<reported_user>" reason "<reason>"
* **مشاهده لیست گزارش‌ها توسط ناظر:**
  GET reports ?
* **اعمال جریمه روی متخلف توسط ناظر:**
  POST penalty ? report_id "<report_id>" type "<health_penalty|bullet_penalty>" amount "<amount>" number_of_matches "<count>"
* **رد گزارش تخلف توسط ناظر:**
  POST dismiss_report ? report_id "<report_id>"

---

## 📐 فرمول‌های محاسباتی امتیازات

### ۱. به‌روزرسانی امتیاز $XP$ (پس از مسابقه تفننی):
* $\Delta_{XP} = \max\left(5, 50 - \left(0.1 \times (XP_{\text{winner}} - XP_{\text{loser}})\right)\right)$[cite: 2]
* $XP_{\text{new, winner}} = XP_{\text{winner}} + \Delta_{XP}$[cite: 2]
* $XP_{\text{new, loser}} = XP_{\text{loser}} - \Delta_{XP}$[cite: 2]

### ۲. به‌روزرسانی امتیاز $RP$ (پس از مسابقه رقابتی):
* **برنده:** $RP_{\text{new}} = RP_{\text{old}} + \Delta RP + \text{HealthBonus}$[cite: 3]
* **بازنده:** $RP_{\text{new}} = RP_{\text{old}} - \Delta RP$[cite: 3]
* **مقدار $\Delta RP$ بر اساس سطح:** Bronze (75), Silver (100), Golden (125), Platinum (150)[cite: 3]
* **پاداش جان باقی‌مانده:** $\text{HealthBonus} = \text{RemainingHealth} \times 25$[cite: 3]

---

## 🧪 نمونه خروجی‌های استاندارد

**خروجی دستور `GET profile ?`:**
username: "iliya"
Level: Silver
RP: 1500
XP: 500
Total wins: 5
Total losses: 5

**خروجی دستور `GET match_status ?` (در مسابقه رقابتی):**
Turn 3
You: shoot
Your opponent: pending
History:
Opponent's moves:    Your moves:
defend               defend
shoot                defend
Your remaining bullets: 1
Your remaining health: 2
