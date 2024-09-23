#ifndef ATCOMMANDHANDLER_H
#define ATCOMMANDHANDLER_H

#include <Arduino.h>
#include "DeviceManager.h"

// Function declarations
bool SendATCommand(const char *atCommand, unsigned long maxDelay);
bool SendATCommandNoError(const char *atCommand, unsigned long maxDelay);
bool SendATCommandWithResponse(const char *atCommand, unsigned long maxDelay, String &responseVar);

#endif // ATCOMMANDHANDLER_H