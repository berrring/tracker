#include <Arduino.h>

/**
 * Kids GPS Tracker - Version 0.0.5: Power Deep Sleep
 * Focus: Battery optimization using ESP32 Deep Sleep and module power management.
 */

#define MODEM_RX 16
#define MODEM_TX 17
#define SOS_BUTTON GPIO_NUM_4

HardwareSerial SerialAT(2);

void powerDownModem() {
  Serial.println("Powering down SIM800L...");
  SerialAT.println("AT+CPOWD=1"); // Normal Power Down
  delay(2000);
}

void setup() {
  Serial.begin(115200);
  SerialAT.begin(9600, SERIAL_8N1, MODEM_RX, MODEM_TX);
  
  delay(1000);
  Serial.println("\n--- Kids GPS Tracker: V0.0.5 Power Management ---");

  // Logic to determine why we woke up
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

  if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT0) {
    Serial.println("Woke up from SOS Button! Sending Emergency Alert...");
    // [Insert SOS SMS Logic from V3 here]
  } else {
    Serial.println("Initial Power On or Timer Wakeup.");
  }

  // Perform tasks...
  delay(5000); 

  // Prepare for Sleep
  powerDownModem();
  
  Serial.println("Entering Deep Sleep now...");
  
  // Configure SOS Button as Wakeup Source (Active Low)
  esp_sleep_enable_ext0_wakeup(SOS_BUTTON, 0); 
  
  // Optional: Wake up every 1 hour to check in
  // esp_sleep_enable_timer_wakeup(3600ULL * 1000000ULL);

  esp_deep_sleep_start();
}

void loop() {
  // This will never be reached in Deep Sleep
}
