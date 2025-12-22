#include "sensors.h"
#include "config.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <OneWire.h>
#include <DallasTemperature.h>

Adafruit_BMP280 bmp;
OneWire oneWire(DS18B20_PIN);
DallasTemperature ds18b20(&oneWire);
bool bmpConnected = false;

void initSensors() {
  // Initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);
  
  // Initialize BMP280
  bmpConnected = bmp.begin(0x76);
  if (!bmpConnected) {
    bmpConnected = bmp.begin(0x77);
  }
  
  if (bmpConnected) {
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                    Adafruit_BMP280::SAMPLING_X2,
                    Adafruit_BMP280::SAMPLING_X16,
                    Adafruit_BMP280::FILTER_X16,
                    Adafruit_BMP280::STANDBY_MS_500);
  }
  
  // Initialize DS18B20
  ds18b20.begin();
  
  // Initialize soil moisture sensor
  pinMode(SOIL_MOISTURE_PIN, INPUT);
}

SensorData readSensors() {
  SensorData data;
  
  // Read BMP280
  if (bmpConnected) {
    float tempC = bmp.readTemperature();
    data.airTemp = tempC * 9.0 / 5.0 + 32.0;  // Convert to Fahrenheit
    data.pressure = bmp.readPressure() / 1000.0F;  // Convert to kPa
  } else {
    data.airTemp = -1;
    data.pressure = -1;
  }
  
  // Read DS18B20
  ds18b20.requestTemperatures();
  float soilTempC = ds18b20.getTempCByIndex(0);
  if (soilTempC == DEVICE_DISCONNECTED_C || soilTempC == -127.0) {
    data.soilTemp = -1;
  } else {
    data.soilTemp = soilTempC * 9.0 / 5.0 + 32.0;  // Convert to Fahrenheit
  }
  
  // Read soil moisture
  int raw = analogRead(SOIL_MOISTURE_PIN);
  int moisture = map(raw, DRY_VALUE, WET_VALUE, 0, 100);
  moisture = constrain(moisture, 0, 100);

  data.moisture = moisture;
  return data;
}