#include <Arduino.h>
#include "SIM7670G_AT_Commands.h"  // Include the custom CPP file with AT command functions

void setup() {
    Serial.begin(115200);  // Initialize Serial for communication with PC
    Serial1.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin);  // Initialize Serial1 for SIM7670G communication

    // Step 1: Reset and turn on the SIM module with full functionality
    Serial.println("Resetting and turning on SIM module...");
    resetModule();  // Reset the SIM7670G module
    delay(2000);
    
    enableFullSIMFunctionality();  // Turn on full SIM functionality
    delay(2000);

    // Step 2: Turn on GNSS (GPS)
    Serial.println("Enabling GNSS (GPS) functionality...");
    enableGNSS();  // Enable GNSS power
    setGNSSMode();  // Set GNSS mode
    setGNSSNMEAOutput();  // Set NMEA output for GNSS
    delay(10000);

    // Step 3: Get all necessary information
    Serial.println("Fetching SIM and GNSS information...");

    getIMEI();  // Get IMEI number
    delay(5000);

    getICCID();  // Get ICCID (SIM number)
    delay(5000);

    getIMSI();  // Get IMSI (SIM subscriber identity)
    delay(5000);

    getSPN();  // Get service provider name (SPN)
    delay(5000);

    checkSignalStrength();  // Get signal strength
    delay(5000);

    checkNetworkRegistrationStatus();  // Get network registration status
    delay(5000);

    getTime();  // Get the current time
    delay(5000);

    testGNSS();  // Test GNSS functionality and retrieve GNSS data
    delay(5000);

    // Print all the variables with stored data at the end
    Serial.println("\n--- Final Stored Data ---");
    Serial.print("IMEI: ");
    Serial.println(IMEI);

    Serial.print("ICCID: ");
    Serial.println(ICCID);

    Serial.print("IMSI: ");
    Serial.println(IMSI);

    Serial.print("SPN: ");
    Serial.println(SPN);

    Serial.print("Signal Strength: ");
    Serial.println(signalStrength);

    Serial.print("Network Registration Status: ");
    Serial.println(networkRegistrationStatus);

    Serial.print("Current Time: ");
    Serial.println(currentTime);

    Serial.print("GNSS Data: ");
    Serial.println(GNSSData);
}

void loop() {
    // Nothing to do in the loop for this example, the setup() does all the work
}
