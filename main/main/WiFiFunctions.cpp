#include "WiFiFunctions.h"
#include <WiFi.h>
#include "Utilities.h"

// Connect to Wi-Fi as a client
bool WifiClientConnect(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < MAX_WIFI_ATTEMPTS) {
        delay(1000);
        Serial.println("Connecting to Wi-Fi...");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Wi-Fi connected");
        showGreenLED(); // Success feedback
        return true;
    } else {
        Serial.println("Wi-Fi connection failed");
        showRedLED(); // Failure feedback
        return false;
    }
}

// Disconnect from Wi-Fi
bool WifiClientDisconnect() {
    WiFi.disconnect();
    if (WiFi.status() == WL_DISCONNECTED) {
        Serial.println("Wi-Fi disconnected");
        showYellowLED(); // Indicate disconnection
        return true;
    } else {
        Serial.println("Failed to disconnect Wi-Fi");
        return false;
    }
}

// Set up Wi-Fi Access Point
bool WifiAPConnect(const char* ssid, const char* password) {
    bool result = WiFi.softAP(ssid, password);
    if (result) {
        Serial.println("Wi-Fi Access Point started");
        showBlueLED(); // Indicate AP mode
    } else {
        Serial.println("Failed to start Access Point");
        showRedLED();
    }
    return result;
}

// Stop Wi-Fi Access Point
bool WifiAPDisconnect() {
    bool result = WiFi.softAPdisconnect(true);
    if (result) {
        Serial.println("Wi-Fi Access Point stopped");
        showYellowLED();
    } else {
        Serial.println("Failed to stop Access Point");
    }
    return result;
}
