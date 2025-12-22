/*  Planter Monitor with IoT
 *  Modular structure for easy maintenance
 *  
 *  Hardware:
 *  - ESP32-S3
 *  - SSD1309 128x64 OLED (SPI)
 *  - BMP280 (I2C)
 *  - DS18B20 (One-Wire)
 *  - Capacitive Soil Moisture Sensor (Analog)
 *  
 *  Features:
 *  - Real-time display
 *  - Telegram alerts
 *  - On-demand status via Telegram
 */

#include "config.h"
#include "sensors.h"
#include "display.h"
#include "iot.h"

unsigned long lastDisplayUpdate = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("🌱 Planter Monitor Starting...");
  
  // Initialize all subsystems
  initSensors();
  initDisplay();
  initIoT();
  
  Serial.println("✅ All systems initialized!");
}

void loop() {
  unsigned long currentTime = millis();
  
  // Read sensors
  SensorData data = readSensors();
  
  // Update display at regular intervals
  if (currentTime - lastDisplayUpdate >= DISPLAY_UPDATE_MS) {
    updateDisplay(data);
    lastDisplayUpdate = currentTime;
  }
  
  // Check for alerts
  checkAndSendAlerts(data);
  
  // Handle Telegram commands
  handleTelegramMessages(data);
  
  // Small delay to prevent watchdog issues
  delay(10);
}