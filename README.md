# Door Locker Security System  

This project implements a **password-protected door locking system** using two **ATmega32 microcontrollers (8 MHz)**. It follows a **layered architecture** with clear separation between the **Human-Machine Interface (HMI_ECU)** and the **Control Unit (CONTROL_ECU)**.  

---

## 🔹 System Overview  

- **HMI_ECU** (MCU1)  
  - 2x16 LCD display  
  - 4x4 Keypad  
  - Handles user interaction (input & messages)  

- **CONTROL_ECU** (MCU2)  
  - EEPROM for password storage  
  - DC Motor for door locking/unlocking  
  - Buzzer for alarms  
  - Handles decision-making & system control  

---

## 🔹 Features  

1. **Password Setup**  
   - User enters a 5-digit password (masked with `*`).  
   - Confirmation required (must match).  
   - Stored securely in EEPROM.  

2. **Main Options**  
   - **`+` Open Door** → Unlock & re-lock sequence (motor CW → hold → CCW).  
   - **`-` Change Password** → Requires old password, then setup again.  

3. **Security**  
   - Password entered incorrectly **3 times → System lock**:  
     - Buzzer ON for 1 minute  
     - LCD shows error  
     - No inputs accepted during lock period  

---

## 🔹 Drivers Implemented  

- **GPIO** – Shared between ECUs  
- **LCD (2x16)** – 4/8-bit modes  
- **Keypad (4x4)** – Input handling  
- **DC Motor** – Controlled with Timer0 PWM  
- **EEPROM** – I2C communication  
- **I2C (TWI)** – Configurable with `TWI_ConfigType`  
- **UART** – Communication between ECUs (`UART_ConfigType`)  
- **Timer1** – Used for delays, motor timing (interrupt + callback)  
- **Buzzer** – Controlled via GPIO  

---

## 🔹 System Sequence  

1. **Setup Password** → Store in EEPROM.  
2. **Select Option** (Open Door / Change Password).  
3. **Verify Password** → Match with EEPROM.  
4. **Door Control** (if correct) OR **Alarm** (if failed 3 times).  

---

## 🔹 Hardware Connections  

- **HMI_ECU**: LCD + Keypad  
- **CONTROL_ECU**: EEPROM + DC Motor + Buzzer  
- **Communication**: UART between the two MCUs  

---

## 🔹 Tools & Environment  

- **MCU**: ATmega32 @ 8 MHz  
- **Language**: C  
- **IDE**: Atmel Studio / Microchip Studio  
- **Programmer**: Any AVR ISP  

---

## 📌 Author  
Developed as part of the **Embedded Systems Course Final Project**.  
