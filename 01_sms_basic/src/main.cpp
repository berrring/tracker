#include <Arduino.h>

/**
 * Kids GPS Tracker - Version 0.0.1: SMS Basic
 * Focus: Basic GSM communication using SIM800L module.
 * 
 * Circuit:
 * - ESP32 RX2 (GPIO 16) -> SIM800L TX
 * - ESP32 TX2 (GPIO 17) -> SIM800L RX (Use 1k/2k Resistor Divider for 3.3V logic)
 * - SIM800L VCC -> External 3.7V-4.4V Li-Po or DC Source (2A peak)
 * - GND -> Common Ground
 */

#define MODEM_RX 16
#define MODEM_TX 17

HardwareSerial SerialAT(2);

void setup() {
  // Console for debugging
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n--- Kids GPS Tracker: V0.0.1 SMS Basic ---");

  // Communication with SIM800L
  SerialAT.begin(9600, SERIAL_8N1, MODEM_RX, MODEM_TX);
  
  Serial.println("Initializing modem...");
  delay(3000);

  // Basic AT check
  SerialAT.println("AT");
  delay(100);
  while (SerialAT.available()) {
    Serial.write(SerialAT.read());
  }

  // Set SMS mode to text
  SerialAT.println("AT+CMGF=1");
  delay(100);
  
  Serial.println("Modem Ready. Type 's' to send a test SMS.");
}

void sendSMS(String phoneNumber, String message) {
  Serial.println("Sending SMS...");
  SerialAT.print("AT+CMGS=\"");
  SerialAT.print(phoneNumber);
  SerialAT.println("\"");
  delay(100);
  SerialAT.print(message);
  delay(100);
  SerialAT.write(26); // Ctrl+Z to send
  delay(5000);
  
  while (SerialAT.available()) {
    Serial.write(SerialAT.read());
  }
  Serial.println("\nSMS Process Finished.");
}

void loop() {
  // Forward data from Modem to Console
  if (SerialAT.available()) {
    Serial.write(SerialAT.read());
  }

  // Handle Serial Commands
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 's') {
      // Replace with a real number for testing
      sendSMS("+1234567890", "Test from Kids GPS Tracker V0.0.1");
    } else {
      SerialAT.write(c);
    }
  }
}
