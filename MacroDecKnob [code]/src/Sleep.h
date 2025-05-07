#ifndef SLEEP_H
#define SLEEP_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Preferences.h>
#include "Config.h" // Per BACKLIGHT_PIN e WAKEUP_PIN

extern TFT_eSPI tft;
extern Preferences preferences;
extern int KeyProfileIndex;
extern int EncoderProfileIndex;

// ---------------- Routine Deep Sleep mode ---------------
inline void enterDeepSleep() {
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(10, 20);
  tft.setTextSize(2);
  tft.print("Risparmio energia");

  preferences.begin("macrodeck", false);
  preferences.putInt("KeyProfile", KeyProfileIndex);
  preferences.putInt("EncoderProfile", EncoderProfileIndex);
  preferences.end();

  delay(3000);
  digitalWrite(BACKLIGHT_PIN, LOW); // Spegne retroilluminazione
  esp_sleep_enable_ext1_wakeup((1ULL << WAKEUP_PIN), ESP_EXT1_WAKEUP_ALL_LOW);
  //Serial.println("Going into deep sleep...");
  delay(100);
  //Serial.print("GPIO5 state before sleep: ");
  //Serial.println(digitalRead(WAKEUP_PIN));
  tft.fillScreen(TFT_BLACK);
  esp_deep_sleep_start();
}

#endif // SLEEP_H
