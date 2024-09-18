#include <Arduino.h>

static const int RXPin = 17, TXPin = 18;
static const uint32_t GPSBaud = 115200;  // Baud rate for SIM7670G

String rev;   // Variable to store response from SIM7670G
String inputCommand; // Variable to store the user input from Serial Monitor

// Forward declaration of the SentMessage function (without default argument here)
bool SentMessage(const char *p_char, unsigned long timeout);

// Function to send an AT command to SIM7670G module
void SentSerial(const char *p_char) {
  for (int i = 0; i < strlen(p_char); i++) {
    Serial1.write(p_char[i]);
    delay(10);
  }
  Serial1.write('\r');   // Send carriage return
  delay(10);
  Serial1.write('\n');   // Send newline
  delay(10);
}

// Function to handle and send the user-entered AT command
void handleSerialInput() {
  if (Serial.available()) {
    inputCommand = Serial.readStringUntil('\n');  // Read input until newline
    inputCommand.trim();  // Remove any trailing whitespace
    if (inputCommand.length() > 0) {
      SentSerial(inputCommand.c_str());  // Send command to SIM7670G
      Serial.print("Command Sent: ");
      Serial.println(inputCommand);  // Print sent command to Serial Monitor
    }
  }
}

void setup() {
  Serial.begin(115200);  // Initialize Serial for communication with PC
  Serial1.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin);  // Initialize Serial1 for SIM7670G communication

  while (!SentMessage("AT", 2000)) {  // Check communication with the module using basic "AT"
    delay(1000);
  }

  Serial.println("Ready to receive AT commands. Type your command below:");
}

void loop() {
  handleSerialInput();  // Handle user input from the Serial Monitor

  // Check if there's a response from the SIM7670G module
  if (Serial1.available()) {
    rev = Serial1.readString();  // Read the incoming response from SIM7670G
    Serial.print("Response: ");
    Serial.println(rev);  // Print the response to the Serial Monitor
  }
}

// Function to send an AT command and wait for "OK" or a timeout
bool SentMessage(const char *p_char, unsigned long timeout = 2000) {
  SentSerial(p_char);  // Send the command

  unsigned long start = millis();
  while (millis() - start < timeout) {
    if (Serial1.available()) {
      rev = Serial1.readString();  // Read the response
      if (rev.indexOf("OK") != -1) {
        Serial.println("Got OK!");  // If "OK" is found in the response
        return true;
      }
    }
  }
  return false;  // Timeout without receiving "OK"
}
