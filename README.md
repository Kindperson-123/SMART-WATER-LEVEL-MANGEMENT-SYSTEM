# SMART-WATER-LEVEL-MANGEMENT-SYSTEM
# Smart Water Level Management System (IoT-Based)

An intelligent, IoT-enabled water level monitoring and pump control system powered by an ESP32 microcontroller and integrated with the Blynk IoT platform. This project provides both automated and manual control over water pumps, tracks active runtime, calculates water usage and billing metrics in real-time, and sends instant push notifications.

For complete project assets, including visual architectural schematics, wiring diagrams, circuit photos, and control flowcharts, please refer to the `DOCUMENTATION/`, `HARDWARE/`, and `MEDIA/` folders in this repository.

---

## 🚀 Key Features

- **Dual Operation Modes**: Seamlessly switch between Automatic Mode (sensor-driven level regulation) and Manual Mode via the Blynk App.
- **Real-Time IoT Dashboard**: Monitor sensor states, pump status, running time, water consumption (in liters), and estimated billing (Blynk IoT).
- **Automated Safety & Error Handling**: Interlock logic handles edge cases (such as conflicting float switch states) to prevent pump dry-runs or overflows.
- **Instant Alerts**: Push notifications sent straight to your mobile device when critical states like "Tank FULL" or "Error State" are triggered.
- **Efficient Switching Circuit**: Low-power ESP32 digital outputs safely drive a higher-voltage 12V DC mini pump using a 2N2222 BJT transistor and relay module.

---

## 🛠 System Architecture & Block Diagram

The system architecture coordinates multiple input and output domains. High-level interactions flow from the lower and upper float switches into the ESP32 main controller, which executes either automatic or manual logic routines. Depending on the mode, the controller activates a relay driver circuit utilizing a 2N2222 transistor to switch a 12V DC water pump. Water consumption and runtime metrics are processed and streamed back to the Blynk dashboard for analytics and remote visualization. Visual diagrams of this architecture are located in the `MEDIA/` folder.

---

## 📌 Pinout & Hardware Connections

- **Lower Float Sensor**: Connected to ESP32 Pin G25 and ground to detect lower critical water thresholds.
- **Upper Float Sensor**: Connected to ESP32 Pin G26 and ground to detect upper full water thresholds.
- **Relay Driver Base**: Connected to ESP32 Pin G27 (via a base current-limiting resistor) to trigger the 2N2222 BJT transistor.
- **Status LED Indicator**: Connected to ESP32 Pin G5 (via a current-limiting resistor) for visual system feedback.
- **Relay Module Power**: Tied to the ESP32 VCC and GND power rails.
- **Water Pump Load**: Isolated high-current switching loop powered by a 12V DC adapter running through the relay's Common (COM) and Normally Open (NO) terminals.

A full photographic reference of the physical breadboard prototype and demonstration media can be found in the `HARDWARE/` and `MEDIA/` folders.

---

## 📊 Operational Flowchart Logic

- **Initialization**: Initializes serial communications at 115200 baud, connects to Wi-Fi and the Blynk server, configures GPIO input pins with internal pull-ups, and sets initial outputs to OFF.
- **Main Loop & Sync**: Continuously executes `Blynk.run()` and reads digital states from both float switches.
- **Manual Mode Branch**: Directly handles pump control via Blynk Virtual Pin `V5` while tracking active duration, water usage, and cost calculations.
- **Automatic Mode Branch**: Evaluates float switch combinations across four states:
  - **State 1 (Tank Empty - Low: LOW, High: LOW)**: Turns the pump ON and records the start time.
  - **State 2 (Tank Filling - High: HIGH, Low: LOW)**: Maintains current pump operation.
  - **State 3 (Tank Full - Low: HIGH, High: HIGH)**: Turns the pump OFF, calculates final metrics, triggers notifications, and pulses the indicator LED.
  - **State 4 (Error State - Low: LOW, High: HIGH)**: Safety interlock shuts the pump OFF, triggers a warning LED, and sends an error alert to Blynk.

The complete graphical flowchart has been archived under `DOCUMENTATION/`.

---

## 🚀 Getting Started

1. Clone or download this repository to your local machine.
2. Open the firmware script located in the `FIRMWARE/` folder using the Arduino IDE.
3. Install the required libraries via the Library Manager (Blynk and ESP32 board support packages).
4. Update your local network credentials (`WIFI_SSID`, `WIFI_PASS`) and your unique Blynk Template credentials/Auth Token in the source code.
5. Compile and upload the firmware sketch to your ESP32 board.
6. Manage float sensor hardware and rig the circuit.
7. Configure your mobile or web Blynk dashboard matching the virtual pin mapping table above to begin monitoring and managing your system!
