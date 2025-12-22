#ifndef CONFIG_H
#define CONFIG_H
#include <stdint.h>

// ===== WiFi Configuration =====
inline const char* WIFI_SSID = "Raphy_almond";
inline const char* WIFI_PASSWORD = "dufamily101";

// ===== Telegram Configuration =====
inline const char* BOT_TOKEN = "8031194619:AAEa7c4_9OYLFG_OGApUgsnhGoCc_PFXGvo";
inline const char* CHAT_ID = "8555858990";

// ===== Pin Configuration =====
inline const uint8_t SOIL_MOISTURE_PIN = 10;
inline const uint8_t DS18B20_PIN = 21;
inline const uint8_t I2C_SDA = 38;
inline const uint8_t I2C_SCL = 36;
inline const uint8_t OLED_CLK = 26;
inline const uint8_t OLED_MOSI = 47;
inline const uint8_t OLED_CS = 34;
inline const uint8_t OLED_DC = 48;
inline const uint8_t OLED_RESET = 33;

// ===== Sensor Thresholds =====
inline const float AIR_TEMP_MIN = 60.0;   // °F
inline const float AIR_TEMP_MAX = 80.0;
inline const float SOIL_TEMP_MIN = 60.0;
inline const float SOIL_TEMP_MAX = 75.0;
inline const int MOISTURE_MIN = 20;       // %
inline const int MOISTURE_MAX = 60;
inline const float PRESSURE_MIN = 98.0;   // kPa
inline const float PRESSURE_MAX = 105.0;

// ===== Soil Moisture Calibration =====
inline const int DRY_VALUE = 2925;
inline const int WET_VALUE = 1079;

// ===== Alert Settings =====
inline const unsigned long ALERT_COOLDOWN_MS = 900000;  // 15 min between same alerts

// ===== Update Intervals =====
inline const unsigned long DISPLAY_UPDATE_MS = 2000;     // 2 seconds
inline const unsigned long CLOUD_LOG_MS = 60000;         // 1 minute
inline const unsigned long TELEGRAM_CHECK_MS = 1000;     // 1 second

#endif