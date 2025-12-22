#include "display.h"
#include "config.h"
#include <Arduino.h>
#include <U8g2lib.h>

U8G2_SSD1309_128X64_NONAME0_F_4W_SW_SPI u8g2(U8G2_R0, OLED_CLK, OLED_MOSI, OLED_CS, OLED_DC, OLED_RESET);

void initDisplay() {
    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x13_tr);
    u8g2.drawStr(0, 10, "Smart Planter");
    u8g2.drawStr(0, 30, "Initializing...");
    u8g2.sendBuffer();
    delay(2000);
}

void updateDisplay(SensorData data) {
    drawstuff(data.airTemp, data.pressure, data.soilTemp, data.moisture);
}

static const unsigned char image_cloud_bits[] U8X8_PROGMEM = {0x00,0x00,0x00,0xe0,0x03,0x00,0x10,0x04,0x00,0x08,0x08,0x00,0x0c,0x10,0x00,0x02,0x70,0x00,0x01,0x80,0x00,0x01,0x00,0x01,0x02,0x00,0x01,0xfc,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static const unsigned char image_Layer_19_bits[] U8X8_PROGMEM = {0x02,0x00,0x07,0x03,0x87,0x07,0xcf,0x07,0xcf,0x03,0xc6,0x01,0x6c,0x00,0x78,0x00,0x30,0x00,0x20,0x00,0x20,0x00};
static const unsigned char image_Layer_28_bits[] U8X8_PROGMEM = {0x02,0x01};
static const unsigned char image_Layer_30_1_bits[] U8X8_PROGMEM = {0x06,0x09,0x09,0x06};
static const unsigned char image_Layer_30_bits[] U8X8_PROGMEM = {0x01};
static const unsigned char image_Layer_31_bits[] U8X8_PROGMEM = {};
static const unsigned char image_Layer_32_bits[] U8X8_PROGMEM = {0x20,0x10,0x06,0x09,0x09,0x06};
static const unsigned char image_Layer_33_bits[] U8X8_PROGMEM = {0x06,0x09,0x09,0x06,0x10,0x20};
static const unsigned char image_passport_left_bits[] U8X8_PROGMEM = {0x3c,0x02,0x19,0x25,0x25,0x19,0x01,0x01,0x05,0x09,0x11,0x25,0x09,0x11,0x25,0x09,0x11,0x25,0x09,0x11,0x25,0x09,0x11,0x25,0x09,0x11,0x25,0x09,0x10,0x24,0x08,0x10,0x20};
static const unsigned char image_weather_humidity_white_bits[] U8X8_PROGMEM = {0x20,0x00,0x20,0x00,0x30,0x00,0x50,0x00,0x48,0x00,0x88,0x00,0x04,0x01,0x04,0x01,0x82,0x02,0x02,0x03,0x01,0x05,0x01,0x04,0x02,0x02,0x02,0x02,0x0c,0x01,0xf0,0x00};
static const unsigned char image_weather_temperature_bits[] U8X8_PROGMEM = {0x38,0x00,0x44,0x00,0x54,0x00,0x54,0x00,0x54,0x00,0x54,0x00,0x54,0x00,0x54,0x00,0x54,0x00,0x92,0x00,0x39,0x01,0x75,0x01,0x7d,0x01,0x39,0x01,0x82,0x00,0x7c,0x00};
static const unsigned char image_weather_temperature_copy_bits[] U8X8_PROGMEM = {0x38,0x00,0x44,0x00,0x44,0x00,0x44,0x00,0x44,0x00,0x44,0x00,0x44,0x00,0x44,0x00,0x44,0x00,0x82,0x00,0x01,0x01,0x09,0x01,0x05,0x01,0x01,0x01,0x82,0x00,0x7c,0x00};

void drawstuff(float t, float p, float st, int m) {
    u8g2.clearBuffer();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    
    // Layer 2
    u8g2.drawLine(7, 14, 127, 14);

    // Layer 3
    u8g2.setFont(u8g2_font_5x8_tr);
    u8g2.drawStr(17, 11, "Air");

    // Layer 5
    u8g2.setFont(u8g2_font_helvB08_tr);
    char buffer[10];
    dtostrf(t, 4, 1, buffer);
    u8g2.drawUTF8(24, 35, (String(buffer) + "F").c_str());

    // Layer 6
    u8g2.setFont(u8g2_font_4x6_tr);
    u8g2.drawStr(13, 22, "Temperature");

    // Layer 7
    u8g2.drawStr(25, 50, "Pressure");

    // Layer 8
    u8g2.setFont(u8g2_font_helvB08_tr);
    dtostrf(p, 4, 1, buffer);
    u8g2.drawUTF8(28, 61, (String(buffer) + "k").c_str());

    // Layer 9
    u8g2.setFont(u8g2_font_4x6_tr);
    u8g2.drawStr(86, 50, "Moisture");


    // Layer 6 copy
    u8g2.setFont(u8g2_font_4x6_tr);
    u8g2.drawStr(74, 22, "Temperature");

    // weather_temperature
    u8g2.drawXBMP(13, 25, 9, 16, image_weather_temperature_bits);

    // Layer 3 copy
    u8g2.setFont(u8g2_font_5x8_tr);
    u8g2.drawStr(80, 11, "Soil");

    // Layer 8 copy
    u8g2.setFont(u8g2_font_helvB08_tr);
    u8g2.drawStr(89, 61, (String(m) + "%").c_str());

    // Layer 5 copy
    dtostrf(st, 4, 1, buffer);
    u8g2.drawUTF8(85, 35, (String(buffer) + "F").c_str());

    // Layer 16
    u8g2.drawLine(65, 64, 65, 1);

    // weather_humidity_white
    u8g2.drawXBMP(74, 45, 11, 16, image_weather_humidity_white_bits);

    // cloud
    u8g2.drawXBMP(36, 2, 17, 16, image_cloud_bits);

    // weather_temperature copy
    u8g2.drawXBMP(74, 25, 9, 16, image_weather_temperature_copy_bits);

    // Layer 19
    u8g2.drawXBMP(105, 2, 11, 11, image_Layer_19_bits);

    // cloud
    u8g2.drawXBMP(9, 48, 17, 16, image_cloud_bits);

    // passport_left
    u8g2.drawXBMP(0, 0, 6, 33, image_passport_left_bits);

    // Layer 20
    u8g2.drawLine(6, 0, 125, 0);

    // Layer 27
    u8g2.drawLine(127, 2, 127, 61);

    // Layer 27
    u8g2.drawLine(0, 28, 0, 61);

    // Layer 28
    u8g2.drawLine(110, 63, 125, 63);

    // Layer 28
    u8g2.drawXBMP(0, 62, 2, 2, image_Layer_28_bits);

    // Layer 29
    u8g2.drawLine(0, 0, 0, 0);

    // Layer 29
    u8g2.drawLine(2, 63, 109, 63);

    // Layer 30
    u8g2.drawXBMP(126, -1, 1, 1, image_Layer_30_bits);

    // Layer 30
    u8g2.drawXBMP(2, 58, 4, 4, image_Layer_30_1_bits);

    // Layer 31
    u8g2.drawXBMP(0, 0, 0, 0, image_Layer_31_bits);

    // Layer 31
    u8g2.drawLine(3, 34, 3, 54);

    // Layer 32
    u8g2.drawXBMP(0, 0, 0, 0, image_Layer_31_bits);

    // Layer 32
    u8g2.drawXBMP(122, 0, 6, 6, image_Layer_32_bits);

    // Layer 33
    u8g2.drawXBMP(122, 58, 6, 6, image_Layer_33_bits);

    u8g2.sendBuffer();
}