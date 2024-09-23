#ifndef ATCOMMANDS_H
#define ATCOMMANDS_H

#include "AT_Commands.h"
#include "ATCommandHandler.h"
#include "DeviceManager.h"

// ===================== SIM Status Commands =====================
bool ResetModule();
bool PowerDownModule();
bool ResetSimModule();
bool TurnOffSim();
bool SetSIMSleepMode();
bool ReadICCID(String &iccid);
bool GetServiceProviderName(String &spn);

// ===================== Device Identification Commands =====================
bool RequestProductSerialNumber(String &serialNumber);
bool RequestManufacturer(String &manufacturer);
bool RequestModel(String &model);
bool DisplayProductIdentification(String &productInfo);
bool TurnOffEcho();
bool GetCurrentConfiguration(String &ATconfig);

// ===================== Network Status Commands =====================
bool SetNetworkRegistration();
bool CheckNetworkRegistration(String &RawNetRegData);
bool GetIMSI(String &imsi);
bool QuerySignalQuality(String &signalQuality);
bool ReadTime(String &currentTime);

// ===================== Internet Commands =====================
bool AttachPDP();
bool DettachPDP();
bool DeActiatePDP();
bool ActivatePDP();
bool GetPDPCongif(String &PDPInfo);
bool GetPDPAuth(String &PDPAuth);

// ===================== HTTP Commands =====================
bool StartHTTPService();
bool StopHTTPService();
bool SetHTTPParameters(const String &param);
bool PerformHTTPAction(int action);
bool ReadHTTPResponse(String &response);

// ===================== GNSS Commands =====================
bool FullFunctionalityMode();
bool StartGNSSUART();
bool PowerOnGNSS();
bool PowerOffGNSS();
bool GetGNSSInfo(String &gnssInfo);
bool GetGPSInfo(String &gpsInfo);
bool SetGNSSMode();
bool SetGNSSNEMA();
bool SetGNSSPort();
bool SleepGNSS();
bool WakeUpGNSS();
bool GetGPSProductInfo(String &gnssprod);

// ===================== Hardware Commands =====================
bool CheckCPUTemperature(String &tempInfo);
bool CheckPowerSupply(String &voltageInfo);
bool SetCriticalVoltagePowerDown();
bool ControlTemperatureAlarm();
bool SetCriticalVoltageWarning();

#endif // ATCOMMANDS_H
