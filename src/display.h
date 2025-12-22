#ifndef DISPLAY_H
#define DISPLAY_H

#include "sensors.h"

void drawstuff(float t, float p, float st, int m);
void initDisplay();
void updateDisplay(SensorData data);

#endif