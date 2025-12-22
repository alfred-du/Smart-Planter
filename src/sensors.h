#ifndef SENSORS_H
#define SENSORS_H

struct SensorData {
  float airTemp;      // Air temperature in °F
  float pressure;     // Atmospheric pressure in kPa
  float soilTemp;     // Soil temperature in °F
  int moisture;       // Soil moisture percentage
};

void initSensors();
SensorData readSensors();

#endif