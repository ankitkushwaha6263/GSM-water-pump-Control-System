# 📡 GSM Based water pump Control System

## 📌 Problem Statement

In many rural and industrial areas, controlling electrical devices remotely is difficult due to lack of internet connectivity. This leads to:

* Manual operation of motors and appliances
* Delay in response time
* Dependency on physical presence

---

## 💡 Solution

This project provides a **GSM-based remote control system** that allows users to operate electrical devices using **missed calls and SMS**, without requiring internet connectivity.

---

## 🚀 Key Features

* 📞 Missed call based ON/OFF control
* 📩 SMS-based control system
* 🔒 Multi-user authentication (only authorized numbers allowed)
* 🌐 Works without internet
* ⚡ Low-cost and reliable solution

---

## ⚙️ Technologies Used

* Arduino Uno/Nano
* SIM800L GSM Module
* Relay Module
* Embedded C / Arduino IDE

---

## 🔧 Working Principle

1. User makes a missed call or sends SMS
2. GSM module receives signal
3. Arduino reads incoming number
4. System verifies authorized users
5. Relay is switched ON/OFF accordingly

---

## 📷 Project Demonstration

circuit :-
available in circuit folder

---

## 📽️ Demo Video

👉 [https://youtu.be/SJ76ulapixA?si=Q1nHJhXzTnO7TG48]

---

## 💻 Code

Available in `/code/gsm_relay.ino`

---

## ⚠️ Challenges Faced

* 📶 GSM network delay and response time handling
* ⚡ Voltage instability in SIM800L module
* 🔁 False triggering due to noisy signals
* 🔋 Power supply issues during GSM transmission

---

## 🛠️ Solutions Implemented

* Used stable external power supply for SIM800L
* Implemented **number filtering logic** for security
* Added **software debouncing** to avoid false triggering
* Optimized code for reliable response

---

## ✅ Results

* Successfully controlled relay using missed calls and SMS
* Secure system with authorized user access
* Reliable operation without internet dependency

---

## 🔮 Future Improvements

* 📱 Mobile app integration
* 📊 Status feedback via SMS
* ☁️ IoT cloud integration

---

## ⭐ Conclusion

This project demonstrates a **practical embedded system solution** for remote device control using GSM technology, focusing on reliability, security, and real-world usability.

---
