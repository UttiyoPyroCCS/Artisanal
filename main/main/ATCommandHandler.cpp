#include "ATCommandHandler.h"
#include "DeviceManager.h"

// Function 1: Send AT command, clear buffer, check for OK/ERROR or timeout
bool SendATCommand(const char *atCommand, unsigned long maxDelay) {
  // Clear the serial buffer
  while (Serial1.available()) {
    Serial1.read();
  }

  // Send the AT command
  Serial1.print(atCommand);
  Serial1.print("\r\n");

  // Start the timer
  unsigned long startTime = millis();
  String response = "";

  // Check for a response within the given delay
  while (millis() - startTime < maxDelay) {
    if (Serial1.available()) {
      char c = Serial1.read();
      response += c;

      // Check for OK
      if (response.indexOf("OK") != -1) {
        Serial.println("Program ran successfully!");
        return true;
      }

      // Check for ERROR
      if (response.indexOf("ERROR") != -1) {
        Serial.println("Program did not run successfully!");
        return false;
      }
    }
  }

  // Timeout case
  Serial.println("Timeout!");
  return false;
}

// Function 2: Send AT command, check for OK or timeout
bool SendATCommandNoError(const char *atCommand, unsigned long maxDelay) {
  // Send the AT command
  Serial1.print(atCommand);
  Serial1.print("\r\n");

  // Start the timer
  unsigned long startTime = millis();
  String response = "";

  // Check for a response within the given delay
  while (millis() - startTime < maxDelay) {
    if (Serial1.available()) {
      char c = Serial1.read();
      response += c;

      // Check for OK
      if (response.indexOf("OK") != -1) {
        Serial.println("Program ran successfully!");
        return true;
      }
    }
  }

  // If no response, assume error
  Serial.println("Error: No response!");
  return false;
}

// Function 3: Send AT command, capture the response string
bool SendATCommandWithResponse(const char *atCommand, unsigned long maxDelay, String &responseVar) {
  // Clear the serial buffer before sending the command
  while (Serial1.available()) {
    Serial1.read();
  }

  // Send the AT command
  Serial1.print(atCommand);
  Serial1.print("\r\n");

  // Add a small delay to avoid capturing the echoed command itself
  delay(100); // You can tweak this if needed

  // Start the timer
  unsigned long startTime = millis();
  String response = "";
  bool commandEchoSkipped = false; // Flag to indicate we've skipped the echoed command

  // Check for a response within the given delay
  while (millis() - startTime < maxDelay) {
    if (Serial1.available()) {
      char c = Serial1.read();
      response += c;

      // If we haven't skipped the command echo yet, check if it's done
      if (!commandEchoSkipped) {
        if (response.indexOf("\r\n") != -1) { // AT command echo should end with \r\n
          response = ""; // Clear the response variable to ignore the command echo
          commandEchoSkipped = true; // Mark that we've skipped the echo
        }
      }
    }
  }

  // If response was received after skipping the echo, process it
  if (response.length() > 0) {
    // Find the start of the actual response after the ':'
    int startIdx = response.indexOf(":");
    if (startIdx != -1) {
      startIdx += 1; // Move to the first character after ':'
    } else {
      startIdx = 0; // If no ':' found, start from the beginning
    }

    int endIdx = response.lastIndexOf("OK");
    if (endIdx == -1) {
      endIdx = response.length(); // If no 'OK' found, use the full length
    }

    // Extract the actual response content between ':' and 'OK'
    responseVar = response.substring(startIdx, endIdx);
    responseVar.trim(); // Remove any extra spaces or newlines
    Serial.println("Response received and stored.");
    return true;
  } else {
    Serial.println("Timeout or no valid response!");
    return false;
  }
}
