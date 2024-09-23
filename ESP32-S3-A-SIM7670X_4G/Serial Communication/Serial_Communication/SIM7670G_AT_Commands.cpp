// SIM7670G_AT_Commands.cpp
#include "SIM7670G_AT_Commands.h"
#include <Arduino.h>



// Variables to store specific responses
String IMEI;
String ICCID;
String IMSI;
String SPN;
String signalStrength;
String networkRegistrationStatus;
String GNSSData;
String GPSData;
String currentTime;

// Helper function to send AT commands (without expecting a response)
void SentSerial(const char *p_char) {
    for (int i = 0; i < strlen(p_char); i++) {
        Serial1.write(p_char[i]);
        delay(10);
    }
    Serial1.write('\r');  // Carriage return
    delay(10);
    Serial1.write('\n');  // New line
    delay(10);
}

void SentSerialWithResponse(const char *p_char, String &responseVar, unsigned long timeout) {
    SentSerial(p_char);  // Send the command
    unsigned long start = millis();
    responseVar = "";  // Clear previous response

    while (millis() - start < timeout) {
        if (Serial1.available()) {
            responseVar = Serial1.readString();  // Read the response
            if (responseVar.indexOf("OK") != -1) {
                Serial.println("Got OK!");  // If "OK" is found in the response
                return;
            }
        }
    }
    responseVar = "Timeout";  // Indicate timeout if no response
}


// Function to reset SIM functionality (no response expected)
void resetSIM() {
    SentSerial("AT+CFUN=6");
}

// Function to reset the whole module (no response expected)
void resetModule() {
    SentSerial("AT+CRESET");
}

// Function to turn off SIM functionality (no response expected)
void turnOffSIM() {
    SentSerial("AT+CFUN=7");
}

// Function to turn off the entire module (no response expected)
void turnOffModule() {
    SentSerial("AT+CPOF");
}

// Function to enable full SIM functionality (no response expected)
void enableFullSIMFunctionality() {
    SentSerial("AT+CFUN=1");
}

// Function to check signal strength (response stored in 'signalStrength')
void checkSignalStrength() {
    SentSerialWithResponse("AT+CSQ", signalStrength);
    Serial.print("Signal Strength: ");
    Serial.println(signalStrength);  // Print the response
}

// Function to ask for time (response stored in 'currentTime')
void getTime() {
    SentSerialWithResponse("AT+CCLK?", currentTime);
    Serial.print("Current Time: ");
    Serial.println(currentTime);  // Print the response
}

// Function to set time (no response expected)
void setTime(const char *time) {
    String command = "AT+CCLK=\"";
    command += time;
    command += "\"";
    SentSerial(command.c_str());
}

// Function to get IMEI number of SIM (response stored in 'IMEI')
void getIMEI() {
    SentSerialWithResponse("AT+SIMEI?", IMEI);
    Serial.print("IMEI Number: ");
    Serial.println(IMEI);  // Print the response
}

// Function to check SIM network registration status (response stored in 'networkRegistrationStatus')
void checkNetworkRegistrationStatus() {
    SentSerialWithResponse("AT+CREG?", networkRegistrationStatus);
    Serial.print("Network Registration Status: ");
    Serial.println(networkRegistrationStatus);  // Print the response
}

// Function to set SIM network registration status (no response expected)
void setNetworkRegistrationStatus(int n) {
    String command = "AT+CREG=";
    command += String(n);
    SentSerial(command.c_str());
}

// Function to check SIM ICCID (SIM number) (response stored in 'ICCID')
void getICCID() {
    SentSerialWithResponse("AT+CICCID", ICCID);
    Serial.print("SIM ICCID: ");
    Serial.println(ICCID);  // Print the response
}

// Function to check SIM IMSI (International Mobile Subscriber Identity) (response stored in 'IMSI')
void getIMSI() {
    SentSerialWithResponse("AT+CIMI", IMSI);
    Serial.print("SIM IMSI: ");
    Serial.println(IMSI);  // Print the response
}

// Function to check SIM service provider name (SPN) (response stored in 'SPN')
void getSPN() {
    SentSerialWithResponse("AT+CSPN?", SPN);
    Serial.print("SIM Service Provider: ");
    Serial.println(SPN);  // Print the response
}

// Function to turn on GNSS (no response expected)
void enableGNSS() {
    SentSerial("AT+CGNSSPWR=1");
}

// Function to test GNSS functionality (response stored in 'GNSSData')
void testGNSS() {
    SentSerialWithResponse("AT+CGNSSTST=1", GNSSData);
    Serial.print("GNSS Data: ");
    Serial.println(GNSSData);  // Print the response
}

// Function to set GNSS mode (no response expected)
void setGNSSMode() {
    SentSerial("AT+CGNSSMODE=15");
}

// Function to set NMEA data output for GNSS (no response expected)
void setGNSSNMEAOutput() {
    SentSerial("AT+CGNSSNMEA=1,1,1,1,1,1,1,1,1");
}

// Function to set GNSS baud rate (no response expected)
void setGNSSBaudRate(int baud) {
    String command = "AT+CGNSSIPR=";
    command += String(baud);
    SentSerial(command.c_str());
}

// Function to put GNSS into sleep mode (no response expected)
void sleepGNSS() {
    SentSerial("AT+CGNSSSLEEP");
}

// Function to wake up GNSS (no response expected)
void wakeGNSS() {
    SentSerial("AT+CGNSSWAKEUP");
}
