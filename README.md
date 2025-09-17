# ATM System – Console Application in C++

## 📌 Overview
This project is a **console-based ATM simulation** written in C++.  
It enables clients to log in with an **account number and PIN**, then perform different banking operations such as withdrawals, deposits, and balance checks.  
Data is stored in text files for persistence, simulating a basic banking system without a database.

---

## ✨ Features
- 🔐 **Secure Login**
  - Account number + PIN validation.
  - Only registered clients can access ATM services.

- 💵 **Withdrawals**
  - **Quick Withdraw:** Fast selection of common amounts (20, 50, 100, etc.).
  - **Normal Withdraw:** Enter any amount (multiples of 5).

- 💰 **Deposit**
  - Deposit positive amounts.
  - Updates balance immediately.

- 📊 **Check Balance**
  - Displays the client’s current account balance.

- 💾 **File Handling**
  - Client data stored in `ClientDataFile.txt`.
  - Supports saving updated balances after transactions.

---

## Example Screens

========================================
        Login Screen
========================================
Enter Account Number?: 1234
Enter PinCode?: 4321

========================================
        ATM Main Menu Screen
========================================
 [1] Quick Withdraw
 [2] Normal Withdraw
 [3] Deposit
 [4] Check Balance
 [5] Logout

 ---

 ## 📖 Learning Objectives
 - Apply file handling for persistent data storage.
 - Implement user authentication with account number and PIN.
 - Build modular functions for different ATM operations.
 - Simulate a real-world ATM system using console-based UI.
 - Write clean, maintainable code using the principle of (Divide and Conquer) by breaking the program into smaller, manageable functions.
