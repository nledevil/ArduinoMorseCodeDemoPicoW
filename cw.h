#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

DynamicJsonDocument doc(1024);

/*_____________________________________________________MORSE_FUNCTIONS______________________________________________________*/

// Key Pressed Down, note the time and set buttonPressed to true
void startButtonPress() {
  pressStart = millis();
  buttonPressed = true;
}

// Key Up
void endButtonPress() {
  // Get time that the key was pressed down
  unsigned long pressTimer = millis() - pressStart;
  // If time is greater than the hold time (default 150ms), then dash, else dit
  if (pressTimer > holdtime) {
    // dash
    Serial.print("-");
    cBuffer += "-";
  } else {
    // dit
    Serial.print(".");
    cBuffer += ".";
  }
  // Make sure to set buttonPressed to false;
  buttonPressed = false;
  // Reset the pressStart time to -1
  pressStart = -1;
  // Note the time the button was last released
  releaseStart = millis();
  longReleaseStart = millis();
}

void handleKey() {
  // Debouncing
  if (millis() - lastTimeKeyStateChanged > debounceDuration) {
    // Read Key State
    byte keyState = digitalRead(CW_KEY);
    // If Keystate changed
    if (keyState != lastKeyState) {
      // Note the time last keystate changed since we are past the debounce threshold
      lastTimeKeyStateChanged = millis();
      // Set lastKeyState
      lastKeyState = keyState;
      // If Key is pressed (brought to ground aka Low)
      if (keyState == LOW) {
        // Start the button pressed if it isn't already
        if (buttonPressed == false) {
          startButtonPress();
        }
        // Set the LED HIGH
        digitalWrite(LED_BUILTIN, HIGH);
        // Set TONE HIGH
        digitalWrite(CW_TONE, HIGH);
      } else {
        // Start endButtonPress if it isn't already
        if (buttonPressed == true) {
          endButtonPress();
        }
        // Set LED LOW
        digitalWrite(LED_BUILTIN, LOW);
        // Set TONE LOW
        digitalWrite(CW_TONE, LOW);
      }
    }
  }
}

void checkNewLetter() {
  // If key isn't pressed and we have a releaseStart time
  if (buttonPressed == false && releaseStart != -1) {
    // Check how long the key has been released
    unsigned long releaseTimer = millis() - releaseStart;
    // If key has been released longer than charPadTime (default 200ms)
    if (releaseTimer > charPadTime) {
      // Reset the releaseStart time
      releaseStart = -1;
      // Determine Character
      const char* c = doc[cBuffer];
      // Clear Character buffer
      cBuffer = "";
      // Set letter to Uppercase
      String letter = String(c);
      letter.toUpperCase();
      // If we have a real letter, then broadcast it over websockets
      if (letter != "") {
        sendBroadcast(letter);
      }
      // Add letter to the word buffer
      wBuffer += letter;
      Serial.print("|");
    }
  }
}

void checkNewWord() {
  // If key isn't pressed and we have a longReleaseStart time
  if (buttonPressed == false && longReleaseStart != -1) {
    // Get the long release time
    unsigned long longReleaseTimer = millis() - longReleaseStart;
    // If long release time is greater than wordPadTime (Default 1000ms aka 1s)
    if (longReleaseTimer > wordPadTime) {
      // We have a new word, reset longReleaseStart, clear word buffer
      longReleaseStart = -1;
      Serial.println("");
      Serial.println(wBuffer);
      wBuffer = "";
    }
  }
}
