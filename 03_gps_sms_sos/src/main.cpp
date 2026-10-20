#include <Arduino.h>
#include <TinyGPS++.h>

/**
 * Kids GPS Tracker - Version 0.0.3: GPS SMS SOS
 * Focus: SOS Button trigger -> Fetch GPS -> Send Google Maps link via SMS.
 */

// Pins
#define MODEM_RX 16
#define MODEM_TX 17
#define GPS_RX 18
#define GPS_TX 19
#define SOS_BUTTON 4

// Objects
TinyGPSPlus gps;
HardwareSerial SerialAT(2);
HardwareSerial SerialGPS(1);

const String DEST_PHONE = "+1234567890"; // REPLACE WITH EMERGENCY CONTACT

void sendSMS(String message) {
  Serial.println("Sending SOS SMS...");
  SerialAT.print("AT+CMGS=\"");
  SerialAT.print(DEST_PHONE);
  SerialAT.println("\"");
  delay(100);
  SerialAT.print(message);
  delay(100);
  SerialAT.write(26); // Ctrl+Z
  delay(5000);
  Serial.println("SMS Sent.");
}

void setup() {
  Serial.begin(115200);
  SerialAT.begin(9600, SERIAL_8N1, MODEM_RX, MODEM_TX);
  SerialGPS.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
  
  pinMode(SOS_BUTTON, INPUT_PULLUP);
  
  Serial.println("\n--- Kids GPS Tracker: V0.0.3 GPS SMS SOS ---");
  Serial.println("System Ready. Press SOS Button to alert.");
}

void loop() {
  // Feed GPS data
  while (SerialGPS.available() > 0) {
    gps.encode(SerialGPS.read());
  }

  // Check SOS Button (Active Low)
  if (digitalRead(SOS_BUTTON) == LOW) {
    Serial.println("SOS BUTTON PRESSED!");
    delay(200); // Debounce
    
    String msg = "EMERGENCY! My location: ";
    if (gps.location.isValid()) {
      msg += "https://www.google.com/maps?q=";
      msg += String(gps.location.lat(), 6);
      msg += ",";
      msg += String(gps.location.lng(), 6);
    } else {
      msg += "Location unknown (No GPS Fix).";
    }
    
    sendSMS(msg);
    
    // Wait for button release
    while(digitalRead(SOS_BUTTON) == LOW) delay(10);
  }
}
