#include <Arduino.h>
#include "config.h"
#include "tdk_ctrl.h"

// Function declarations
void readSerial();
void relayControl(int, int);
void runCycles(int, int);

// Global Variables
const char relay[] = {'0', '1', '2', '3', '4', '7', '8', '5', '6', 'A'};
int cycleSteps = 1;       // Default to 1 cycle
int cycleDuration = 1;    // Default to 1 minute per cycle

void setup() {
  Serial.begin(9600); // Initialize serial communication with a baud rate of 9600
  Wire.begin();       // Initialize I2C communication

  for (int pin = RELAY_MIN; pin <= RELAY_MAX; pin++) {
    pinMode(pin, OUTPUT); // Set relay pin controlling solenoid valve as an output
    digitalWrite(pin, RELAY_OFF); // Close SV1 initially (Relay is Active LOW)
  }
}

void loop() {
  // Main code here
  readSerial();
}

void runCycles(int steps, int duration) {
  // Update global cycle parameters
  cycleSteps = steps;
  cycleDuration = duration;

  // Run cycles
  for (int cycleCount = 0; cycleCount < cycleSteps; ++cycleCount) {
    // Wait for 10 seconds
    delay(10000);

    // Run "CMD=3,1" for 3 minutes
    relayControl(3, 1);
    delay(180000); // 3 minutes in milliseconds

    // Wait for 10 seconds
    delay(10000);

    // Run "CMD=4,1" for 4 minutes
    relayControl(4, 1);
    delay(240000); // 4 minutes in milliseconds

    // Wait for 5 seconds
    delay(5000);
  }
}

void readSerial() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');

    // Check that the data is valid
    if (cmd.substring(0, 4) == STRING_CMD && cmd.length() == STRING_LENGTH && cmd[3] == '=' && cmd[5] == ',') {
      char relay_pin = cmd[4];
      char action = cmd[6];
      int arr_length = sizeof(relay) / sizeof(relay[0]);
      uint16_t selected_relay = 0;
      bool isValidRelay = false;

      // Check which relay pin has been selected
      for (int i = 0; i < arr_length; i++) {
        if (relay_pin == relay[i]) {
          // Check which action has been selected
          selected_relay = i + 1;
          isValidRelay = true;
          if (action == TURN_OFF) {
            // Send command to close selected relay
            relayControl(selected_relay, RELAY_OFF);
            Serial.println("OK");
            break;
          } else if (action == TURN_ON) {
            // Send command to open the selected relay
            relayControl(selected_relay, RELAY_ON);
            Serial.println("OK");
            break;
          } else {
            Serial.println("ERROR");
            break;
          }
        }
        if (!isValidRelay && i == arr_length - 1) {
          Serial.println("ERROR");
        }
      }
    } else if (cmd.substring(0, 7) == READ_TDK) {
      // AT+TDK=1,0 for single measurement
      if (cmd.substring(7, cmd.length()) == "1,0\r") {
        // Placeholder for your code - Perform action for single measurement
        Serial.println("OK");
      }
      // AT+TDK=1,1 for continuous measurement
      else if (cmd.substring(7, cmd.length()) == "1,1\r") {
        // Placeholder for your code - Perform action for continuous measurement
        Serial.println("OK");
      } else if (cmd.substring(7, cmd.length()) == "0\r") {
        // Placeholder for your code - Perform action to stop measurement
        Serial.println("OK");
      } else {
        Serial.println("ERROR");
      }

    } else if (cmd.substring(0, 4) == "CMD=") {
      // Extract cycle parameters
      int cmdIndex = cmd.indexOf('=') + 1;
      int comma1 = cmd.indexOf(',', cmdIndex);
      int comma2 = cmd.indexOf(',', comma1 + 1);
      int cyclesRequested = cmd.substring(comma2 + 1).toInt();

      // Run cycles
      runCycles(cyclesRequested, cycleDuration);

      Serial.println("OK");
    } else {
      Serial.println("ERROR");
    }
  }
}

void relayControl(int relaypin, int action) {
  if (relaypin == ALL_OFF) {
    for (int pin = RELAY_MIN; pin <= RELAY_MAX; pin++) {
      digitalWrite(pin, RELAY_OFF);
    }
  } else if (relaypin == ALL_ON) {
    for (int pin = RELAY_MIN; pin <= RELAY_MAX; pin++) {
      digitalWrite(pin, RELAY_ON);
    }
  } else {
    digitalWrite(relaypin, action);
  }
}
