#include <Arduino.h>
#include <TinyGPS++.h>

// Select your modem
#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>

/**
 * Kids GPS Tracker - Version 0.0.4: GPRS Firebase
 * Focus: Periodic GPRS data upload to Firebase Realtime Database.
 */

// Pins
#define MODEM_RX 16
#define MODEM_TX 17
#define GPS_RX 18
#define GPS_TX 19

// GPRS Credentials
const char apn[]      = "internet"; // Change to your SIM APN
const char gprsUser[] = "";
const char gprsPass[] = "";

// Firebase Details
const char server[]   = "YOUR_PROJECT_ID.firebaseio.com";
const char auth[]     = "YOUR_DATABASE_SECRET"; // Or use Firebase Token
const int  port       = 443;

TinyGPSPlus gps;
HardwareSerial SerialAT(2);
HardwareSerial SerialGPS(1);
TinyGsm modem(SerialAT);
TinyGsmClientSecure client(modem);
HttpClient http(client, server, port);

void setup() {
  Serial.begin(115200);
  SerialAT.begin(9600, SERIAL_8N1, MODEM_RX, MODEM_TX);
  SerialGPS.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

  Serial.println("Initializing modem...");
  modem.restart();
  
  Serial.print("Connecting to APN: ");
  Serial.println(apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    Serial.println("GPRS connection failed");
    return;
  }
  Serial.println("GPRS Connected");
}

void loop() {
  while (SerialGPS.available() > 0) {
    gps.encode(SerialGPS.read());
  }

  static unsigned long lastUpload = 0;
  if (millis() - lastUpload > 30000) { // Every 30 seconds
    if (gps.location.isValid()) {
      uploadToFirebase(gps.location.lat(), gps.location.lng());
      lastUpload = millis();
    }
  }
}

void uploadToFirebase(float lat, float lon) {
  String path = "/location.json?auth=" + String(auth);
  String payload = "{\"lat\":" + String(lat, 6) + ",\"lng\":" + String(lon, 6) + ",\"timestamp\": \"2026-10-21\"}";

  Serial.println("Uploading to Firebase...");
  http.patch(path, "application/json", payload);

  int statusCode = http.responseStatusCode();
  String response = http.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
}
