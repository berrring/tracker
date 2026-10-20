# Version 0.0.4: GPRS Firebase Realtime Logging

## 📌 Purpose
This version transitions the project from SMS alerts to continuous cloud telemetry. The ESP32 utilizes the SIM800L GPRS data stack (`TinyGsmClientSecure` and `ArduinoHttpClient`) to establish an internet connection over cellular data and upload real-time Latitude/Longitude JSON payloads to a Firebase Realtime Database.

## 🛠 Hardware Setup & Requirements
- **Hardware:** Identical to V0.0.3 (ESP32 + SIM800L + NEO-6M).
- **SIM Card Requirements:** Active Nano-SIM card with provisioned mobile data (APN configured correctly in code).
- **Libraries:** `TinyGSM`, `ArduinoHttpClient`, `TinyGPS++`.

## 🔌 Step-by-Step Wiring & Configuration
1. Verify dual UART connections and 1k/2k level shifter on SIM800L RX.
2. In `src/main.cpp`, update the cellular APN credentials (`apn`, `gprsUser`, `gprsPass`) matching your local carrier.
3. Update `server` and `auth` variables with your Firebase Realtime Database URL and database secret/token.

## 🚀 How to Flash and Test
1. Build and flash the firmware.
2. Monitor the Serial output to verify GprS attachment (`+CGATT: 1`) and IP address acquisition.
3. Observe periodic HTTP PATCH requests and check your Firebase console for incoming location updates.

---
![Power Distribution](../docs/images/power_distribution.svg)
