# Smart Planter Monitor - Technical Documentation

> ESP32-S3 based plant monitoring system with environmental sensors and Telegram alerts

---

## 📋 Hardware Overview

| Component | Interface | GPIO | Purpose |
|-----------|-----------|------|---------|
| BMP280 | I²C (0x76) | 38(SDA), 36(SCL) | Air temp/pressure |
| DS18B20 | One-Wire | 21 | Soil temperature probe |
| Capacitive Moisture | Analog | 10 | Soil moisture level |
| OLED SSD1309 | SPI | 26,47,34,48,33 | 128×64 display |

**Power:** 3.3V, ~150mA typical  
**MCU:** ESP32-S3-Mini DevBoard (12-bit ADC)

---

## 🔌 Pinout Reference

### Sensor Connections
```
BMP280:          DS18B20:         Soil Moisture:
  VCC → 3V3        Red → 3V3        VCC → 3V3
  GND → GND        Black → GND      GND → GND
  SDA → GPIO 38    Yellow → 21      AOUT → GPIO 10
  SCL → GPIO 36    (4.7kΩ pullup)
  (4.7kΩ pullups)

OLED Display (SPI):
  VCC → 3V3
  GND → GND
  SCK → GPIO 26
  MOSI → GPIO 47
  CS → GPIO 34
  DC → GPIO 48
  RES → GPIO 33
```

### Pull-up Resistors Required
- **I²C Bus:** 4.7kΩ on SDA (GPIO 38) and SCL (GPIO 36) to 3V3
- **One-Wire:** 4.7kΩ on DS18B20 Data (GPIO 21) to 3V3
- *Can use 2.2kΩ - 10kΩ range*

---

## 🎯 Sensor Calibration

### Soil Moisture (Capacitive v1.2)

**Current Calibration:**
```cpp
DRY_VALUE = 2925  // ADC reading in air (12-bit: 0-4095)
WET_VALUE = 1079  // ADC reading in water
```

**How to Recalibrate:**

1. **Dry Test:** Hold sensor in air
   ```cpp
   Serial.println(analogRead(10)); // Record this value
   ```

2. **Wet Test:** Submerge sensor in water
   ```cpp
   Serial.println(analogRead(10)); // Record this value
   ```

3. Update `config.h` with your values

**Moisture Scale:**
- 0-20%: Critically dry 🔴
- 20-40%: Needs water 🟡
- 40-60%: Optimal 🟢
- 60-80%: Moist ⚪
- 80-100%: Oversaturated 🔵

### BMP280 (Air Sensor)
- **Factory calibrated** - no user adjustment needed
- Accuracy: ±1°C, ±1 hPa
- I²C address: 0x76 (or 0x77 if SDO high)

### DS18B20 (Soil Temperature)
- **Factory calibrated** - ±0.5°C accuracy
- 12-bit resolution (0.0625°C steps)
- Wire colors: Red=VCC, Yellow=Data, Black=GND
- If reads -127°C → check pull-up resistor

---

## ⚙️ Configuration Settings

### Alert Thresholds
```cpp
Temperature (°F):
  Air:  60-80°F
  Soil: 60-75°F

Moisture: 20-60%
Pressure: 98-105 kPa
```

### Timing Intervals
```cpp
Display update:  2 seconds
Cloud logging:   60 seconds
Telegram check:  1 second
Alert cooldown:  15 minutes
```

### WiFi & Telegram
**File:** `config.h` (create from template)
```cpp
WIFI_SSID = "your_network"
WIFI_PASSWORD = "your_password"
BOT_TOKEN = "your_bot_token"
CHAT_ID = "your_chat_id"
```
*⚠️ Never commit credentials to public repos!*

---

## 🔧 Troubleshooting

### Sensor Not Detected

**BMP280:**
- Check I²C address (try 0x77 instead of 0x76)
- Verify pull-up resistors on SDA/SCL
- Run I²C scanner sketch

**DS18B20:**
- Reading -127°C? → Missing pull-up resistor
- Verify GPIO 21 has 4.7kΩ to 3V3
- Check waterproof probe seal

**Soil Moisture:**
- Reading always 0 or 4095? → Check GPIO 10 connection
- Unstable readings? → Add 10µF capacitor from GPIO 10 to GND

### Display Issues
- Blank screen? → Check CS, DC, RES pin connections
- Garbled output? → Verify SPI pins (SCK, MOSI)
- Wrong display? → Change constructor to `U8G2_SSD1309_...` if needed

### WiFi/Telegram
- Can't connect? → Check credentials in `config.h`
- Alerts not sending? → Verify BOT_TOKEN and CHAT_ID
- Commands not working? → Ensure bot has privacy mode disabled

---

## 📊 Plant Care Guidelines

### Optimal Ranges (Most Indoor Plants)
- **Temperature:** 65-75°F (18-24°C)
- **Humidity:** 40-60%
- **Soil Moisture:** 40-60%
- **Soil Temp:** 65-70°F (18-21°C)

### Species-Specific Adjustments

**Succulents/Cacti:**
- Moisture: 10-30% (very dry)
- Less frequent watering

**Tropical Plants:**
- Humidity: 60-80% (higher)
- Moisture: 50-70% (more moist)

**Herbs:**
- Moisture: 40-60% (moderate)
- Good drainage essential

*Adjust thresholds in `config.h` for your plant type*

---

## 🛠️ Hardware Tips

### Power Supply
- Use quality 5V USB adapter (≥500mA)
- Avoid cheap phone chargers (cause brownouts)
- Consider battery backup for critical plants

### Sensor Placement
- **BMP280:** Mount on planter floor
- **DS18B20:** Insert into the side cavity of bucket
- **Moisture:** Insert into the side cavity of bucket

### Wiring Best Practices
- Keep I²C wires under 30cm
- Use twisted pair for SDA/SCL
- Star ground topology (all GND to one point)
- Add 100nF capacitors near each sensor VCC/GND

### Weatherproofing
- Keep electronics dry
- Only DS18B20 probe should touch soil/water
- Seal all connections with heat shrink
- Use waterproof enclosure for outdoor use

---

## 📈 Future Enhancements

**Possible Additions:**
- [ ] Light sensor (BH1750 or VEML7700)
- [ ] pH sensor (analog or I²C)
- [ ] Water level sensor
- [ ] Automatic watering pump control
- [ ] Solar panel + battery
- [ ] Multiple plant monitoring
- [ ] Web dashboard (ESP32 web server)
- [ ] Historical data logging (SD card)

---

## 📚 Library Dependencies

```cpp
Adafruit_BMP280
U8g2lib
OneWire
DallasTemperature
WiFi (ESP32 built-in)
UniversalTelegramBot
ArduinoJson
```

Install via Arduino Library Manager

---
## 🔐 Security Notes

**Important:**
- Never commit `config.h` with real credentials
- Add `config.h` to `.gitignore`
- Use environment variables for public repos
- Rotate tokens if accidentally exposed
- Keep firmware updated for security patches

---

## 📞 Support & Resources

**Documentation:** [GitHub Wiki](#)  
**Issues:** [GitHub Issues](#)  
**Discussions:** [GitHub Discussions](#)

**Useful Links:**
- [ESP32-S3 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)
- [BMP280 Datasheet](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmp280-ds001.pdf)
- [U8g2 Reference](https://github.com/olikraus/u8g2/wiki)
- [Telegram Bot API](https://core.telegram.org/bots/api)

---

*Last updated: December 2025*  
*Hardware revision: v1.0*  
*Firmware version: 1.0.0*
