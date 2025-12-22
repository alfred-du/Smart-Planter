#ifndef IOT_H
#define IOT_H

#include "sensors.h"

void initIoT();
void checkAndSendAlerts(SensorData data);
void handleTelegramMessages(SensorData data);
void sendStatusUpdate(SensorData data);
bool isWiFiConnected();

#endif