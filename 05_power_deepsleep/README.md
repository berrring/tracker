# Version 0.0.5: Power Management & Deep Sleep Optimization

## 📌 Purpose
Version 0.0.5 implements advanced power management to maximize battery longevity for a wearable child tracker. It features software-controlled modem power-down (`AT+CPOWD=1`), GPS standby mode, and ESP32 **Deep Sleep Mode** with external GPIO wakeup (`esp_sleep_enable_ext0_wakeup`) triggered by the SOS button.

## 🛠 Hardware Setup & Optimization
- **Hardware:** ESP32, SIM800L, NEO-6M, Omron SOS Button on **GPIO 4**, and Pololu S7V8F3 step-up/step-down voltage regulator.
- **Power Efficiency:** In Deep Sleep, the ESP32 core shuts down most peripherals, reducing current consumption from ~40mA down to **< 10 µA**, while keeping GPIO 4 active as an interrupt wakeup source.

## 🔌 Step-by-Step Wiring Instructions
1. Ensure the SOS button is connected between **GPIO 4** and **GND** (active low).
2. Wire the power input through a BQ24074 power-path management IC or TP4056 module paired with the Pololu S7V8F3 regulator to maintain stable 3.3V even as the LiPo battery discharges to 3.0V.

## 🚀 How to Flash and Test
1. Flash `src/main.cpp` to the ESP32.
2. Open the Serial Monitor. Observe the boot sequence, simulated location transmit, modem power-down command (`AT+CPOWD=1`), and entry into Deep Sleep.
3. Press the SOS button. The ESP32 will immediately wake up via EXT0, initialize the GSM modem, and transmit the emergency alert.

---
![Architecture Overview](../docs/images/architecture_overview.svg)
