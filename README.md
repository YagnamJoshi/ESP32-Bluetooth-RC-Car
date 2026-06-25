# 🏎️ ESP32 Smart Bluetooth RC Car

A high-performance, Bluetooth-controlled RC car built with an ESP32. This project features a custom Android controller app built with MIT App Inventor, live distance telemetry, and smart front-collision avoidance.

This code is written specifically for **ESP32 Arduino Core v3.x**, utilizing the updated LEDC PWM API for smooth motor control.

## ✨ Features

* **Smart Obstacle Avoidance:** Automatically halts forward movement if an obstacle is detected within 30cm, while preserving the ability to reverse and turn out of tight spots.
* **Live App Telemetry:** The ESP32 utilizes a non-blocking timer to broadcast live distance readings (in cm) to the Android app twice a second.
* **Dynamic Speed Control:** 8-level speed adjustment via a slider in the app.
* **Stabilized Turning:** Turns and U-turns are automatically locked to the lowest speed setting to prevent spin-outs and maintain control.
* **Core v3.x Compatible:** Uses modern `ledcAttach()` and `ledcWrite()` functions, bypassing deprecated PWM handling.

---

## 🛠️ Hardware Requirements

* **Microcontroller:** ESP32 DevKit V1
* **Motor Driver:** TB6612FNG
* **Sensor:** HC-SR04 Ultrasonic Distance Sensor
* **Chassis:** Standard 2WD Robot Car Chassis (2 DC gear motors + 1 front caster wheel)
* **Power Supply:** 2x 18650 Batteries (7.4V)
* **Voltage Regulation:** LM2596 Buck Converter (Stepping 7.4V down to 5.0V)
* **Misc:** 400-pin breadboard, jumper wires, 1kΩ & 2kΩ resistors (for voltage divider), ON/OFF switch.

---

## ⚡ Wiring Guide

*Note: All grounds (Battery negative, Buck Converter output negative, ESP32 GND, TB6612FNG GND) must be connected to a Common Ground rail.*

### Motor Driver (TB6612FNG)
| TB6612FNG Pin | ESP32 Pin / Connection | Function |
| :--- | :--- | :--- |
| **VCC** | 5V (Buck Converter) | Logic Power |
| **VM** | 7.4V (Direct Battery) | Motor Power |
| **STBY** | GPIO33 | Standby Switch |
| **PWMA** | GPIO25 | Left Motor Speed |
| **AIN1** | GPIO26 | Left Motor Direction |
| **AIN2** | GPIO27 | Left Motor Direction |
| **PWMB** | GPIO14 | Right Motor Speed |
| **BIN1** | GPIO12 | Right Motor Direction |
| **BIN2** | GPIO13 | Right Motor Direction |

### Ultrasonic Sensor (HC-SR04)
**⚠️ WARNING:** The HC-SR04 ECHO pin outputs 5V. You must use a voltage divider to step it down to ~3.3V before connecting it to the ESP32.

| HC-SR04 Pin | ESP32 Pin / Connection | Notes |
| :--- | :--- | :--- |
| **VCC** | 5V (Buck Converter) | Power |
| **GND** | Common Ground | Ground |
| **TRIG** | GPIO4 | Trigger Pulse |
| **ECHO** | GPIO2 *(via divider)* | **Must pass through 1kΩ/2kΩ divider** |

---

## 📱 The Android App (MIT App Inventor)

The controller app communicates via **Bluetooth Classic**. 
* **Commands:** Sends single-character strings for movement (`F`, `B`, `L`, `R`, `S`, `U`) and numbers (`1`-`8`) for speed control.
* **Telemetry:** Reads byte strings terminated by a newline character (Delimiter Byte: `10`) to update the live distance label.

---

## 🚀 Installation & Setup

1. **Clone the Repository:**
   ```bash
   git clone [https://github.com/YourUsername/ESP32-Bluetooth-RC-Car.git](https://github.com/YourUsername/ESP32-Bluetooth-RC-Car.git)
2. Setup Arduino IDE:

Install the ESP32 board manager (must be version 3.0.0 or higher).

Ensure Bluetooth is enabled in your ESP32 core configuration (usually on by default).

3. Upload the Code:

4. Open esp32_rc_car.ino in the Arduino IDE.

5. Select your ESP32 board and COM port.

6. Hit Upload.

7. Connect the App:

8. Power on the car.

9. Pair your Android phone to ESP32_RC_CAR in your phone's Bluetooth settings.

10. Open the custom MIT App, tap "Connect", and select the car.

## 🔮 Future Improvements
Add a rear-facing ultrasonic sensor to implement reverse collision avoidance.

Add LED headlights and taillights.

Migrate to Bluetooth Low Energy (BLE) for iOS compatibility.

Built with logic, coffee, and a few blown fuses.
