#include "iot.h"
#include "config.h"
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

// Alert tracking
unsigned long lastAlertTime[6] = {0}; // Track last alert time for each threshold
enum AlertType {
  AIR_TEMP_LOW = 0,
  AIR_TEMP_HIGH = 1,
  SOIL_TEMP_LOW = 2,
  SOIL_TEMP_HIGH = 3,
  MOISTURE_LOW = 4,
  MOISTURE_HIGH = 5
};

void initIoT() {
  Serial.println("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    
    // Don't verify SSL certificate (easier for Telegram)
    secured_client.setInsecure();
    
    // Send startup message
    bot.sendMessage(CHAT_ID, "🌱 Planter Monitor Online!", "");
  } else {
    Serial.println("\nWiFi connection failed!");
  }
}

bool isWiFiConnected() {
  return WiFi.status() == WL_CONNECTED;
}

void sendAlert(String message, AlertType type) {
  unsigned long currentTime = millis();
  
  // Check cooldown period
  if (currentTime - lastAlertTime[type] < ALERT_COOLDOWN_MS) {
    return; // Don't spam alerts
  }
  
  if (isWiFiConnected()) {
    bot.sendMessage(CHAT_ID, "⚠️ ALERT: " + message, "");
    lastAlertTime[type] = currentTime;
  }
}

void checkAndSendAlerts(SensorData data) {
  if (!isWiFiConnected()) return;
  
  // Check air temperature
  if (data.airTemp != -1) {
    if (data.airTemp < AIR_TEMP_MIN) {
      sendAlert("Air temperature LOW: " + String(data.airTemp, 1) + "°F", AIR_TEMP_LOW);
    } else if (data.airTemp > AIR_TEMP_MAX) {
      sendAlert("Air temperature HIGH: " + String(data.airTemp, 1) + "°F", AIR_TEMP_HIGH);
    }
  }
  
  // Check soil temperature
  if (data.soilTemp != -1) {
    if (data.soilTemp < SOIL_TEMP_MIN) {
      sendAlert("Soil temperature LOW: " + String(data.soilTemp, 1) + "°F", SOIL_TEMP_LOW);
    } else if (data.soilTemp > SOIL_TEMP_MAX) {
      sendAlert("Soil temperature HIGH: " + String(data.soilTemp, 1) + "°F", SOIL_TEMP_HIGH);
    }
  }
  
  // Check moisture
  if (data.moisture < MOISTURE_MIN) {
    sendAlert("Soil moisture LOW: " + String(data.moisture) + "% - Time to water!", MOISTURE_LOW);
  } else if (data.moisture > MOISTURE_MAX) {
    sendAlert("Soil moisture HIGH: " + String(data.moisture) + "% - May be overwatered!", MOISTURE_HIGH);
  }
}

void handleTelegramMessages(SensorData data) {
  if (!isWiFiConnected()) return;
  
  static unsigned long lastCheck = 0;
  unsigned long currentTime = millis();
  
  if (currentTime - lastCheck > TELEGRAM_CHECK_MS) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    
    while (numNewMessages) {
      for (int i = 0; i < numNewMessages; i++) {
        String chat_id = String(bot.messages[i].chat_id);
        String text = bot.messages[i].text;
        
        // Only respond to authorized chat ID
        if (chat_id != CHAT_ID) {
          continue;
        }
        
        if (text == "/start") {
          bot.sendMessage(chat_id, "🌱 Planter Monitor Bot\n\nCommands:\n/status - Current readings\n/help - Show commands", "");
        }
        else if (text == "/status") {
          bot.sendMessage(chat_id, "📊 Fetching current readings...", "");
          sendStatusUpdate(data);
        }
        else if (text == "/help") {
          String help = "🌱 Available Commands:\n\n";
          help += "/status - Get current sensor readings\n";
          help += "/help - Show this message\n\n";
          help += "Alert thresholds:\n";
          help += "Air Temp: " + String(AIR_TEMP_MIN, 0) + "-" + String(AIR_TEMP_MAX, 0) + "°F\n";
          help += "Soil Temp: " + String(SOIL_TEMP_MIN, 0) + "-" + String(SOIL_TEMP_MAX, 0) + "°F\n";
          help += "Moisture: " + String(MOISTURE_MIN) + "-" + String(MOISTURE_MAX) + "%";
          bot.sendMessage(chat_id, help, "");
        }
      }
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    
    lastCheck = currentTime;
  }
}

void sendStatusUpdate(SensorData data) {
  if (!isWiFiConnected()) return;
  
  String status = "📊 Current Readings:\n\n";
  
  status += "🌡️ Air Temp: ";
  if (data.airTemp != -1) {
    status += String(data.airTemp, 1) + "°F\n";
  } else {
    status += "N/A\n";
  }
  
  status += "⏲️ Pressure: ";
  if (data.pressure != -1) {
    status += String(data.pressure, 1) + " kPa\n";
  } else {
    status += "N/A\n";
  }
  
  status += "🌡️ Soil Temp: ";
  if (data.soilTemp != -1) {
    status += String(data.soilTemp, 1) + "°F\n";
  } else {
    status += "N/A\n";
  }
  
  status += "💧 Moisture: " + String(data.moisture) + "%";
  
  bot.sendMessage(CHAT_ID, status, "");
}