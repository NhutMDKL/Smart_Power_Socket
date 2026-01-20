#include "Relay.h"

// Set the state flags for relays to false
static bool relayState[2] = {false, false};

// Update LEDs based on relay states
static void updateLed(uint8_t relay, bool state) {
  uint8_t ledPin = (relay == 1) ? LED1_PIN : LED2_PIN;
  digitalWrite(ledPin, state ? HIGH : LOW);
}

void relayInit() {
  // Configure relays output pins
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);

  // Configure LEDs uotput pins
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  relaySet(1, false); // Turn off relay 1
  relaySet(2, false); // Turn off relay 2
}

// Turn relays and LEDs on/off 
void relaySet(uint8_t relay, bool state) {
  // Control corresponding relays
  uint8_t relayPin = (relay == 1) ? RELAY1_PIN : RELAY2_PIN;
  digitalWrite(relayPin, state ? HIGH : LOW);
  relayState[relay - 1] = state;

  // Update LEDs based on relay states
  updateLed(relay, state);
}

// Read relay state from the "relayState" array
bool relayGet(uint8_t relay) {
  return relayState[relay - 1];
}

// Toggle relay states
void relayToggle(uint8_t relay) {
  relaySet(relay, !relayGet(relay));
}
