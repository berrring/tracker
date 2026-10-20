#include <Arduino.h>
#include <TinyGPS++.h>

/**
 * Kids GPS Tracker - Version 0.0.2: GPS Serial
 * Focus: Reading NMEA sentences from NEO-6M GPS module.
 * 
 * Circuit:
 * - ESP32 RX (GPIO 18) -> NEO-6M TX
 * - ESP32 TX (GPIO 19) -> NEO-6M RX (Optional)
 * - NEO-6M VCC -> ESP32 3.3V or 5V
 * - GND -> Common Ground
 */

#define GPS_RX 18
#define GPS_TX 19

TinyGPSPlus gps;
HardwareSerial SerialGPS(1);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n--- Kids GPS Tracker: V0.0.2 GPS Serial ---");

  // NEO-6M usually defaults to 9600 baud
  SerialGPS.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
  
  Serial.println("Waiting for GPS data...");
}

void loop() {
  while (SerialGPS.available() > 0) {
    if (gps.encode(SerialGPS.read())) {
      if (gps.location.isValid()) {
        Serial.print("Latitude: ");
        Serial.println(gps.location.lat(), 6);
        Serial.print("Longitude: ");
        Serial.println(gps.location.lng(), 6);
        Serial.print("Altitude: ");
        Serial.println(gps.altitude.meters());
        Serial.print("Satellites: ");
        Serial.println(gps.satellites.value());
        Serial.println("---------------------------");
      } else {
        Serial.println("Searching for satellites... (Ensure you are outdoors)");
      }
    }
  }

  // If no data is received for 5 seconds, report error
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println("No GPS data detected: check wiring.");
    delay(2000);
  }
}
