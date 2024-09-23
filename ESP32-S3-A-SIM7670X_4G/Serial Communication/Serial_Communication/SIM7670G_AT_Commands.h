#ifndef SIM7670G_AT_COMMANDS_H
#define SIM7670G_AT_COMMANDS_H

#include <Arduino.h>

// Pin configuration for SIM7670G communication
static const int RXPin = 17, TXPin = 18;
static const uint32_t GPSBaud = 115200;  // Baud rate for SIM7670G

// Variables to store specific responses
extern String IMEI;
extern String ICCID;
extern String IMSI;
extern String SPN;
extern String signalStrength;
extern String networkRegistrationStatus;
extern String GNSSData;
extern String GPSData;
extern String currentTime;

// Function declarations
void SentSerial(const char *p_char);
void SentSerialWithResponse(const char *p_char, String &responseVar, unsigned long timeout = 2000);
void resetSIM();
void resetModule();
void turnOffSIM();
void turnOffModule();
void enableFullSIMFunctionality();
void checkSignalStrength();
void getTime();
void setTime(const char *time);
void getIMEI();
void checkNetworkRegistrationStatus();
void setNetworkRegistrationStatus(int n);
void getICCID();
void getIMSI();
void getSPN();
void enableGNSS();
void testGNSS();
void setGNSSMode();
void setGNSSNMEAOutput();
void setGNSSBaudRate(int baud);
void sleepGNSS();
void wakeGNSS();

#endif  // SIM7670G_AT_COMMANDS_H
