#include <Arduino.h>
#include "ATCommandHandler.h"
#include "AT_Commands.h"
#include "DeviceManager.h"
#include "Functions.h"

void ReadConfig() {
    String iccid;
    ReadICCID(iccid);
    String spn;
    GetServiceProviderName(spn);
    String serialNumber;
    RequestProductSerialNumber(serialNumber);
    String manufacturer;
    RequestManufacturer(manufacturer);
    delay(10000);
    String model;
    RequestModel(model);
    delay(10000);
    String productInfo;
    DisplayProductIdentification(productInfo);
    delay(10000);
    String ATconfig;
    GetCurrentConfiguration(ATconfig);
    delay(10000);
    String RawNetRegData;
    CheckNetworkRegistration(RawNetRegData);
    delay(10000);
    String imsi;
    GetIMSI(imsi);
    delay(10000); 
    String signalQuality;
    QuerySignalQuality(signalQuality);             // AT+CSQ
    delay(10000);                                  // Wait for signal quality

    String currentTime;
    ReadTime(currentTime);                          // AT+CCLK?
    delay(10000);                                  // Wait for current time

    String PDPInfo;
    GetPDPCongif(PDPInfo);                         // AT+CGDCONT?
    delay(10000);                                  // Wait for PDP config

    String PDPAuth;
    GetPDPAuth(PDPAuth);                           // AT+CGAUTH?
    delay(10000);                                  // Wait for PDP auth

    String gpsInfo;
    GetGPSInfo(gpsInfo);                           // AT+CGPSINFO
    delay(10000);                                  // Wait for GPS info

    String gnssInfo;
    GetGNSSInfo(gnssInfo);                         // AT+CGNSSINFO
    delay(10000);                                  // Wait for GNSS info

    String GNSSProd;
    GetGPSProductInfo(GNSSProd);                   // AT+CGNSSPROD
    delay(10000);                                  // Wait for GPS product info

    String voltageInfo;
    CheckPowerSupply(voltageInfo);                 // AT+CBC
    delay(10000);                                  // Wait for power supply check

    String tempInfo;
    CheckCPUTemperature(tempInfo);                 // AT+CPMUTEMP
    delay(10000);                                  // Wait for CPU temperature
}



void SleepModule() {
    SetSIMSleepMode();                    // AT+CSIMSLEEP=0
    delay(10000);                         // Wait for SIM sleep mode

    DettachPDP();                          // AT+CGATT=0
    delay(10000);                         // Wait for PDP detac

    DeActiatePDP();                      // AT+CGACT=0,1
    delay(10000);                         // Wait for PDP deactivation

    SleepGNSS();                          // AT+CGNSSSLEEP
    delay(10000); 
    
    turnOffRGB();                        // Wait for GNSS sleep
}

void PowerStatus(String &tempInfo, String &voltageInfo) {
    CheckCPUTemperature(tempInfo); // Use the parameter directly
    CheckPowerSupply(voltageInfo); // Use the parameter directly

    // Read battery level, no need to declare a new variable here
    float batteryLevel = readBatteryLevel(); // Call without parameters
}


void setup() {
  initESP();
  initAT();
  ReadConfig();
  String tempInfo;
  String voltageInfo;
  PowerStatus(tempInfo, voltageInfo);
  SleepModule();
  
}

#include <esp_camera.h>

// Function prototypes
bool StartAutoSend();
bool StopAutoSend();
bool CaptureAndSendImage();
bool SetupGNSS();
bool SetupInternet();
bool SendImageOverHTTP(const uint8_t *imageData, size_t imageSize);

// Global variables
int iteration = 8; // Number of images to capture per hour
unsigned long interval = 3600000 / iteration; // Interval in milliseconds
bool autoSendEnabled = false;
TaskHandle_t autoSendTaskHandle = NULL;

bool CaptureAndSendImage() {
    // Capture an image
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera capture failed");
        return false;
    }

    // Convert to JPEG if needed
    size_t jpg_buf_len = 0;
    uint8_t *jpg_buf = NULL;
    if (fb->format != PIXFORMAT_JPEG) {
        bool jpeg_converted = frame2jpg(fb, 80, &jpg_buf, &jpg_buf_len);
        esp_camera_fb_return(fb);
        if (!jpeg_converted) {
            Serial.println("JPEG conversion failed");
            return false;
        }
    } else {
        jpg_buf_len = fb->len;
        jpg_buf = fb->buf;
    }

    // Send image over HTTP
    bool result = SendImageOverHTTP(jpg_buf, jpg_buf_len);

    // Clean up
    if (fb->format != PIXFORMAT_JPEG) {
        free(jpg_buf);
    }
    esp_camera_fb_return(fb);
    return result;
}

bool SendImageOverHTTP(const uint8_t *imageData, size_t imageSize) {
    if (!StartHTTPService()) {
        Serial.println("Failed to start HTTP service");
        return false;
    }

    String response;
    if (!SetHTTPParameters("image/jpeg") || 
        !PerformHTTPAction(1) || // Assuming '1' corresponds to HTTP POST
        !ReadHTTPResponse(response)) {
        StopHTTPService();
        Serial.println("HTTP action failed");
        return false;
    }

    Serial.println("HTTP Response: " + response);
    StopHTTPService();
    return true;
}

bool SetupGNSS() {
    if (!FullFunctionalityMode() || 
        !PowerOnGNSS() || 
        !StartGNSSUART() || 
        !SetGNSSMode() || 
        !SetGNSSNEMA() || 
        !SetGNSSPort()) {
        Serial.println("GNSS setup failed");
        return false;
    }

    String gpsInfo;
    if (!GetGPSInfo(gpsInfo)) {
        Serial.println("Failed to get GPS info");
        return false;
    }
    Serial.println("GPS Info: " + gpsInfo);
    return true;
}

bool SetupInternet() {
    if (!AttachPDP() || 
        !ActivatePDP()) {
        Serial.println("Internet setup failed");
        return false;
    }

    String pdpInfo;
    if (!GetPDPCongif(pdpInfo)) {
        Serial.println("Failed to get PDP config");
        return false;
    }
    Serial.println("PDP Config: " + pdpInfo);
    return true;
}



