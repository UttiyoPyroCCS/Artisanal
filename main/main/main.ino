#include <Arduino.h>
#include "ATCommandHandler.h"
#include "AT_Commands.h"
#include "DeviceManager.h"

void ReadConfig() {
    String iccid;
    ReadICCID(iccid); // Correct usage
    String spn;
    GetServiceProviderName(spn); // Correct usage
    String serialNumber;
    RequestProductSerialNumber(serialNumber); // Correct usage                                  // Wait for serial number

    String manufacturer;
    RequestManufacturer(manufacturer);             // AT+CGMI
    delay(10000);                                  // Wait for manufacturer

    String model;
    RequestModel(model);                           // AT+CGMM
    delay(10000);                                  // Wait for model

    String productInfo;
    DisplayProductIdentification(productInfo);     // ATI
    delay(10000);                                  // Wait for product info

    String ATconfig;
    GetCurrentConfiguration(ATconfig);             // AT&V
    delay(10000);                                  // Wait for current configuration

    String RawNetRegData;
    CheckNetworkRegistration(RawNetRegData);      // AT+CREG?
    delay(10000);                                  // Wait for network registration check

    String imsi;
    GetIMSI(imsi);                                 // AT+CIMI
    delay(10000);                                  // Wait for IMSI

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

void loop() {
  // Your main loop code here
}

