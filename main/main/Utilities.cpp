#include "Utilities.h"
#include <Adafruit_NeoPixel.h>
#include "soc/rtc_cntl_reg.h"

Adafruit_NeoPixel rgbLED(1, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);

void disableBrownout() {
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
    Serial.println("Brownout detector disabled");
}

String urlencode(const String& str) {
    String encoded = "";
    for (size_t i = 0; i < str.length(); i++) {
        char c = str.charAt(i);
        if (isalnum(c)) {
            encoded += c;
        } else if (c == ' ') {
            encoded += '+';
        } else {
            char buf[4];
            snprintf(buf, sizeof(buf), "%%%02X", c);
            encoded += buf;
        }
        yield(); // Allow background tasks
    }
    return encoded;
}

// LED Feedback Functions
void initializeRGBLED() {
    rgbLED.begin();
    rgbLED.setBrightness(50); // Adjust brightness as needed
    rgbLED.show();
}

void showRedLED() {
    rgbLED.setPixelColor(0, rgbLED.Color(255, 0, 0));
    rgbLED.show();
}

void showGreenLED() {
    rgbLED.setPixelColor(0, rgbLED.Color(0, 255, 0));
    rgbLED.show();
}

void showBlueLED() {
    rgbLED.setPixelColor(0, rgbLED.Color(0, 0, 255));
    rgbLED.show();
}

void showYellowLED() {
    rgbLED.setPixelColor(0, rgbLED.Color(255, 255, 0));
    rgbLED.show();
}

void showOffLED() {
    rgbLED.setPixelColor(0, rgbLED.Color(0, 0, 0));
    rgbLED.show();
}

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

void PowerStatus(String &tempInfo, String &voltageInfo) {
    CheckCPUTemperature(tempInfo); // Use the parameter directly
    CheckPowerSupply(voltageInfo); // Use the parameter directly

    // Read battery level, no need to declare a new variable here
    float batteryLevel = readBatteryLevel(); // Call without parameters
}

bool StartAutoSend() {
    if (autoSendEnabled) {
        Serial.println("Auto-send is already running");
        return false;
    }

    if (!SetupGNSS() || !SetupInternet()) {
        Serial.println("Failed to set up GNSS or internet");
        return false;
    }

    autoSendEnabled = true;
    xTaskCreatePinnedToCore(
        AutoSendTask, 
        "AutoSendTask", 
        4096, 
        NULL, 
        1, 
        &autoSendTaskHandle, 
        1
    );
    Serial.println("Auto-send started");
    return true;
}

bool StopAutoSend() {
    if (!autoSendEnabled) {
        Serial.println("Auto-send is not running");
        return false;
    }

    autoSendEnabled = false;
    if (autoSendTaskHandle) {
        while (eTaskGetState(autoSendTaskHandle) != eDeleted) {
            delay(10);
        }
        autoSendTaskHandle = NULL;
    }

    DettachPDP();
    PowerOffGNSS();
    Serial.println("Auto-send stopped");
    return true;
}


void AutoSendTask(void *parameter) {
    while (autoSendEnabled) {
        if (CaptureAndSendImage()) {
            Serial.println("Image captured and sent successfully");
        } else {
            Serial.println("Failed to capture or send image");
        }
        vTaskDelay(interval / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

