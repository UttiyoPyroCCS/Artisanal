#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <SD.h>
#include "esp_camera.h"
#include <Adafruit_NeoPixel.h>
#include <MAX17048.h>



// Camera Pins
#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 34
#define SIOD_GPIO_NUM 15
#define SIOC_GPIO_NUM 16
#define Y9_GPIO_NUM 14
#define Y8_GPIO_NUM 13
#define Y7_GPIO_NUM 12
#define Y6_GPIO_NUM 11
#define Y5_GPIO_NUM 10
#define Y4_GPIO_NUM 9
#define Y3_GPIO_NUM 8
#define Y2_GPIO_NUM 7
#define VSYNC_GPIO_NUM 36
#define HREF_GPIO_NUM 35
#define PCLK_GPIO_NUM 37

// RGB LED Pin
#define RGB_LED_PIN 38

// TF Card Pins
#define SDMMC_CLK 5
#define SDMMC_CMD 4
#define SDMMC_DATA0 6
#define SD_CD_PIN 46

// Battery SCL, SDA Pins
#define MAX17048_I2C_ADDRESS 0x36

// Wi-Fi Parameters
#define WIFI_AP_SSID "ESP32_AP"
#define WIFI_AP_PASS "12345678"
#define WIFI_STA_SSID "Maxwadrin"
#define WIFI_STA_PASS "101202522"

// Function Declarations
void initESP();
void initAT();
void initWiFiAP();
void initWiFiSTA();
void initBatteryManager();
float readBatteryLevel();
void writeSDCard(const char* fileName, const char* data);
String readSDCard(const char* fileName);
void captureImage(fs::FS &fs, const char* path);
void setRGBColor(uint8_t r, uint8_t g, uint8_t b);
void turnOffRGB();

#endif // DEVICE_MANAGER_H
