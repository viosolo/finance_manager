# Finance Manager 💸

![C++](https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Qt](https://img.shields.io/badge/Framework-Qt-41CD52?style=for-the-badge&logo=qt&logoColor=white)
![Qt Creator](https://img.shields.io/badge/IDE-Qt%20Creator-41CD52?style=for-the-badge&logo=qt&logoColor=white)

A desktop application for efficient personal finance tracking and budget management. Developed using C++ and the Qt framework with **Object-Oriented Programming (OOP)** to provide a high-performance, native user experience.

## 🚀 Key Capabilities

| Feature | Description |
| :--- | :--- |
| **Income & Expense Tracking** | Log transactions with specialized categories. |
| **Tax Integration** |Flag specific income entries as "Taxed" for net-income clarity. |
| **Undo System** | One-click reversal of recent actions (Delete/Edit) using a history stack. |
| **Recurring Expenses:** | Set up automated expense logs with custom time intervals. |
| **Data Persistence**| Secure storage in local files with automatic loading on startup. |

---

## 📊 Performance & Sorting
The application provides advanced data manipulation capabilities:
* **Advanced Sorting:** View transactions by descending date or amount.
* **Time-Window Filtering:** View financial records for a specific day or a custom date range with total amount.
* **Analytics:** Instant identification of period-based reports. 
* **Automated Summaries:** Overall balance, total expenses, and total income are calculated in real-time.

---
## 📸 Interface Overview

#### 1. Main Home Page
![Main Home Page](interface/home_page.png)
*Overview of current balance and real-time financial summaries.*

#### 2. Adding the financial record
![Add Transaction](interface/adding.png)
*Interface for adding recurring expense with interval.*

### 3. Categorized View
![Category Filter](interface/view.png)
*Detailed list of expenses grouped by defined categories.*
![Category Filter](interface/view_empty.png)
*Nothing to view in the category.*
![Category Filter](interface/view_expenses.png)
*Detailed list of expenses grouped by one chosen category.*



### 5. Entering the app
![Sorting](interface/enter.png)


### 6. History Search

![Search](interface/date_range1.png)
<br>

![Search](interface/date_range2.png)

*Filtering transaction history by periods.*

<br>

![Search](interface/specific_date.png)

*Filtering transaction history by specific date.*
### 7. Financial Highs & Lows
![Statistics](interface/search.png)
*Quick identificationof the largest and the smallest financial records.*


---

## 🛠 Technical Stack

* **Language:** `C++17/20` ![C++](https://img.shields.io/badge/-%2300599C?style=flat&logo=c%2B%2B&logoColor=white)
* **Framework:** `Qt Widgets` ![Qt](https://img.shields.io/badge/-%2341CD52?style=flat&logo=qt&logoColor=white)
* **IDE:** `Qt Creator` ![Qt Creator](https://img.shields.io/badge/-%2341CD52?style=flat&logo=qt&logoColor=white)
* **Storage:** Local File I/O

## ⚙️ How to Build

1. Clone the repository.
2. Open the `.pro` file in **Qt Creator**.
3. Select your kit (e.g., Desktop Qt 6.x MSVC/MinGW).
4. Click **Build** and **Run**.

## License
This project is licensed under the [MIT License](LICENSE).
