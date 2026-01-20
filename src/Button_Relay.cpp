#include "Button_Relay.h"
#include "Relay.h"

static uint32_t lastDebounce[2] = {0, 0}; // Record button press time
static bool lastState[2] = {HIGH, HIGH};  // Set initial HIGH level for the buttons

void relayButtonInit() {
  // Configure relay buttons using external pull-up resistors
  pinMode(RELAY_BTN1_PIN, INPUT);  
  pinMode(RELAY_BTN2_PIN, INPUT);
}

void relayButtonLoop() {
  // Read relay button states
  bool state1 = digitalRead(RELAY_BTN1_PIN);
  bool state2 = digitalRead(RELAY_BTN2_PIN);

  // Detect falling edge (button press 1) and ensure 250 ms debounce time
  if (state1 == LOW && lastState[0] == HIGH && millis() - lastDebounce[0] > 250) {
    relayToggle(1); // Toggle relay 1 satte
    lastDebounce[0] = millis();
  }

  // Detect falling edge (button press 2) and ensure 250 ms debounce time
  if (state2 == LOW && lastState[1] == HIGH && millis() - lastDebounce[1] > 250) {
    relayToggle(2); // Toggle relay 2 states
    lastDebounce[1] = millis();
  }

  // Update relay button states
  lastState[0] = state1;
  lastState[1] = state2;
}
