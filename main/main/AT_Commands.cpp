#include "AT_Commands.h"
#include "ATCommandHandler.h"
#include "DeviceManager.h"

// ===================== SIM Status Commands =====================

bool ResetModule() {
    return SendATCommand("AT+CRESET", 5000);
}

bool PowerDownModule() {
    return SendATCommand("AT+CPOF", 5000);
}

bool ResetSimModule() {
    return SendATCommandNoError("AT+CFUN=6", 25000);
}

bool TurnOffSim() {
    return SendATCommandNoError("AT+CFUN=7", 25000);
}

bool SetSIMSleepMode() {
    return SendATCommand("AT+CSIMSLEEP=0", 500);  // Set UE to allow SIM card sleep for power consumption
}

bool ReadICCID(String &iccid) {
    return SendATCommandWithResponse("AT+CICCID", 5000, iccid);  // Read ICCID
}

bool GetServiceProviderName(String &spn) {
    return SendATCommandWithResponse("AT+CSPN?", 1000, spn);  // Get service provider name from SIM
}

// ===================== Device Identification Commands =====================

bool RequestProductSerialNumber(String &serialNumber) {
    return SendATCommandWithResponse("AT+CGSN", 5000, serialNumber);  // Request product serial number
}

bool RequestManufacturer(String &manufacturer) {
    return SendATCommandWithResponse("AT+CGMI", 5000, manufacturer);  // Request manufacturer identification
}

bool RequestModel(String &model) {
    return SendATCommandWithResponse("AT+CGMM", 5000, model);  // Request model identification
}

bool DisplayProductIdentification(String &productInfo) {
    return SendATCommandWithResponse("ATI", 5000, productInfo);  // Display product identification information
}

bool TurnOffEcho() {
    return SendATCommand("ATE0", 1000);  // Turn off command echo
}

bool GetCurrentConfiguration(String &ATconfig) {
    return SendATCommandWithResponse("AT&V", 1000, ATconfig);  // Display current configuration
}

// ===================== Network Status Commands =====================

bool SetNetworkRegistration() {
    return SendATCommandNoError("AT+CREG=2", 5000);
}

bool CheckNetworkRegistration(String &RawNetRegData) {
    return SendATCommandWithResponse("AT+CREG?", 5000, RawNetRegData);
}

bool GetIMSI(String &imsi) {
    return SendATCommandWithResponse("AT+CIMI", 5000, imsi);
}

bool QuerySignalQuality(String &signalQuality) {
    return SendATCommandWithResponse("AT+CSQ", 1000, signalQuality);  // Query signal quality
}

bool ReadTime(String &currentTime) {
    return SendATCommandWithResponse("AT+CCLK?", 1000, currentTime);  // Read current time
}

// ===================== Internet Commands =====================

bool AttachPDP() {
    return SendATCommandNoError("AT+CGATT=1", 45000);
}

bool DettachPDP() {
    return SendATCommandNoError("AT+CGATT=0", 45000);
}

bool ActivatePDP() {
    return SendATCommandNoError("AT+CGACT=1,1", 45000);
}

bool DeActiatePDP() {
    return SendATCommandNoError("AT+CGACT=0,1", 45000);
}

bool GetPDPCongif(String &PDPInfo) {
    return SendATCommandWithResponse("AT+CGDCONT?", 10000, PDPInfo);
}

bool GetPDPAuth(String &PDPAuth) {
    return SendATCommandWithResponse(" AT+CGAUTH?", 10000, PDPAuth);
}


// ===================== HTTP Commands =====================

bool StartHTTPService() {
    return SendATCommand("AT+HTTPINIT", 5000);  // Start HTTP service
}

bool StopHTTPService() {
    return SendATCommand("AT+HTTPTERM", 5000);  // Stop HTTP service
}

bool SetHTTPParameters(const String &param) {
    String command = "AT+HTTPPARA=" + param;  // Set HTTP parameters value
    return SendATCommand(command.c_str(), 5000);
}

bool PerformHTTPAction(int action) {
    String command = "AT+HTTPACTION=" + String(action);  // Perform HTTP action
    return SendATCommand(command.c_str(), 5000);
}

bool ReadHTTPResponse(String &response) {
    return SendATCommandWithResponse("AT+HTTPREAD", 5000, response);  // Read HTTP response information
}

// ===================== GNSS Commands =====================

bool FullFunctionalityMode() {
    return SendATCommand("AT+CFUN=1", 25000);
}

bool StartGNSSUART() {
    return SendATCommand("AT+CGNSSTST=1", 10000);
}

bool PowerOnGNSS() {
    return SendATCommandNoError("AT+CGNSSPWR=1", 10000);
}

bool PowerOffGNSS() {
    return SendATCommandNoError("AT+CGNSSPWR=0", 10000);
}

bool GetGNSSInfo(String &gnssInfo) {
    return SendATCommandWithResponse("AT+CGNSSINFO", 10000, gnssInfo);
}

bool GetGPSInfo(String &gpsInfo) {
    return SendATCommandWithResponse("AT+CGPSINFO", 10000, gpsInfo);
}

bool SetGNSSMode() {
    return SendATCommand("AT+CGNSSMODE=15", 10000);
}

bool SetGNSSNEMA() {
    return SendATCommand("AT+CGNSSNMEA=1,1,1,1,1,1,1,1,1,1", 10000);
}

bool SetGNSSPort() {
    return SendATCommand("AT+CGNSSPORTSWITCH=1,1", 10000);
}

bool SleepGNSS() {
    return SendATCommandNoError("AT+CGNSSSLEEP", 10000);
}

bool WakeUpGNSS() {
    return SendATCommandNoError("AT+CGNSSWAKEUP", 10000);
}

bool GetGPSProductInfo(String &gnssprod) {
    return SendATCommandWithResponse("AT+CGNSSPROD", 1000, gnssprod);  // Display current configuration
}

// ===================== Hardware Commands =====================

bool CheckCPUTemperature(String &tempInfo) {
    return SendATCommandWithResponse("AT+CPMUTEMP", 5000, tempInfo);  // Read the current CPU temperature
}

bool CheckPowerSupply(String &voltageInfo) {
    return SendATCommandWithResponse("AT+CBC", 5000, voltageInfo);  // Read the current power supply voltage
}

bool SetCriticalVoltagePowerDown() {
    return SendATCommand("AT+CPMVT=1,3100,4400", 5000);  // Set critical voltage power down thresholds
}

bool ControlTemperatureAlarm() {
    return SendATCommand("AT+CMTE=1", 5000);  // Set the critical temperature URC alarm
}

bool SetCriticalVoltageWarning() {
    return SendATCommand("AT+CVALARM=1,3200,4300", 5000);  // Set voltage alarm limits
}
