//****************************
//* MacroDecKnob32 DIY       *
//* Autore:Scorpione.Maker   *
//* Youtube:@Scorpione.Maker *
//* ESP32 Wroom 32D          * 
//* Versione: MDK 2.2        *
//****************************

//------------------------Librerie------------------------
#include <Arduino.h>
#include <BleKeyboard.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <TFT_eSPI.h>
#include <AS5600.h>
#include <SPIFFS.h>
#include <Keypad.h>
#include <Preferences.h>
#include "Config.h"
#include "Display.h"
#include "User_Config.h"
#include "Sleep.h"


// ------------------------ Oggetti ----------------------
BleKeyboard bleKeyboard("MacroDecKnob", "ESP32-WROOM", 100);
TFT_eSPI tft = TFT_eSPI();
AMS_5600 ams5600;
Preferences preferences;

// --------------------- Mappa Tasti ---------------------
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'}
};

// ------------------- Pin Righe e Colonne ----------------
byte rowPins[ROWS] = {12, 14, 27};
byte colPins[COLS] = {26, 25, 33, 32};
Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


// -------------------- Profili tastiera -------------------
int KeyProfileIndex = 0; // nuovo indice per il profilo della tastiera (0 = Volume, 1 = Timeline, ecc...)
int EncoderProfileIndex = 0; // nuovo indice per il profilo dell’encoder (0 = Volume, 1 = Timeline, ecc...)
unsigned long lastActivityTime = 0;
unsigned long lastBatteryUpdate = 0;


// ------------------- Encoder Variabili --------------------
volatile int Knob = 0; 
volatile int oldKnob = -999; 
volatile int StepSize = 10; 
volatile int LastAngle = 0; 
volatile int Sogl = 40; 
volatile int CurrentAngle;  
volatile int delta;

// ------------------------ Wake-up flag ----------------------
bool justWokeUp = false;
bool lastEncoderButton = HIGH;

// --------------------- BLE connection flag-------------------
bool previousBLEState = false;

// ------------------ Funzione lettura tensione batteria ---------------
float readBatteryVoltage() {
  int adcRaw = analogRead(BATTERY_ADC_PIN);
  float voltageAdc = ((float)adcRaw / 4095.0) * 3.3; // ADC 12 bit su 3.3V
  float voltageBat = voltageAdc / 0.767;             // Calcolo Vbat reale
  return voltageBat;
}

// ------------------ Funzione per aggiornare la batteria (TFT + BLE) ------------------ 
void updateBatteryStatus() {
    float voltage = readBatteryVoltage();
    drawBatteryIcon(280, 5, voltage);                       // ridisegna la batteria su TFT
    int levelBatBLE = map(voltage * 100, 340, 420, 0, 100);
    bleKeyboard.setBatteryLevel(levelBatBLE);               // aggiorna livello batteria su BLE
}

void setup() {
  Serial.begin(115200);
  delay(500);

  // --------------Montaggio SPIFFS e test come già fatto…-----------
  if (!SPIFFS.begin(true)) { /* error handling */ }
  //Serial.println("✅ SPIFFS OK");

  /* // Debug stampa tutti i file in SPIFFS
  Serial.println("Contenuto SPIFFS:");
  fs::File root = SPIFFS.open("/");
  fs::File file  = root.openNextFile();
  while (file) {
  Serial.println(String("  ") + file.name());
  file = root.openNextFile();
  } */

  // ---------------------- Inizializza  ---------------------
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.println("wire begin");
  bleKeyboard.begin();
  //Serial.println("ble begin");
  customKeypad.setHoldTime(500);
  customKeypad.setDebounceTime(50);

  // ---------------------- Inizializza Display ---------------------
  tft.init(); 
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  pinMode(BACKLIGHT_PIN, OUTPUT);     // Pin abilitazione retroilluminazione TFT
  digitalWrite(BACKLIGHT_PIN, HIGH);  // Accende la retroilluminazione

  // ---------------------- visualizza logo iniziale ---------------------
  showLogo("/Logo.bmp", 3000);  // logo iniziale per 5 secondi

  pinMode(WAKEUP_PIN, INPUT);         // Pin per WAKEUP
  pinMode(KEYPAD_MODE_PIN, INPUT);    // Pin per selezione profilo keypad 
  pinMode(ENCODER_MODE_PIN, INPUT);   // Pin per selezione profilo encoder

  

  // ---------------------- Inizializza Wake-up ---------------------
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
    //Serial.print("Wakeup reason: ");
    //Serial.println(wakeup_reason);
  if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT1) {
    //Serial.println("✅ Wake-up da GPIO5 (EXT1). Attendo rilascio del pulsante...");
    while (digitalRead(WAKEUP_PIN) == LOW) {
      delay(10);
    }
    delay(50);
    justWokeUp = true;
  }

  // ---------------------- Inizializza profili ---------------------
    preferences.begin("macrodeck", false);
    KeyProfileIndex = preferences.getInt("KeyProfile", 0);
    EncoderProfileIndex = preferences.getInt("EncoderProfile", 0);
    preferences.end();

    lastActivityTime = millis();
    
  //---Visualizza Profili e icone tastiera/batteria/Connessione_BLE---
    updateDisplay(profiles[KeyProfileIndex]);
    delay(300);
    drawKeyIcons();
    updateBatteryStatus();
    //drawBLEStatusIcon(true); 
}

void loop() {
  // ---------------- Seleziona profilo keypad -----------------
  if (digitalRead(KEYPAD_MODE_PIN) == LOW && !justWokeUp) {
    KeyProfileIndex = (KeyProfileIndex + 1) % (sizeof(profiles) / sizeof(profiles[0]));
        updateDisplay(profiles[KeyProfileIndex]);         //aggiorna visualizzazione profilo 
        drawKeyIcons();                                   //aggiorna visualizzazione icone Keys
    lastActivityTime = millis();
    delay(300);
    while (digitalRead(KEYPAD_MODE_PIN) == LOW) delay(10);
  }
  justWokeUp = false;

  // ---------------------- Azione keypad ----------------------
  if (bleKeyboard.isConnected()) {
   char key = customKeypad.getKey();
   if (key) {
     for (int r = 0; r < 3; r++) {
       for (int c = 0; c < 4; c++) {
                            
         if (keys[r][c] == key) {
           KeyAction action = profileKeyActions[KeyProfileIndex][r][c];
            if (action.mainKey != 0) {
              if (action.mod1 != 0) bleKeyboard.press(action.mod1);
              if (action.mod2 != 0) bleKeyboard.press(action.mod2);
              bleKeyboard.press(action.mainKey);
              bleKeyboard.releaseAll();
            }  

            lastActivityTime = millis();
            return;
          }
        }
      }
   }
  }

  /* // debug per la connessione BLE
  if (bleKeyboard.isConnected()) {
   static bool wasConnected = false;
   if (!wasConnected) {
     Serial.println("BLE connesso!");
     wasConnected = true;
   }
  } else {
   static bool wasConnected = true;
   if (wasConnected) {
     Serial.println("BLE disconnesso!");
     wasConnected = false;
   }
  } */

  // ----------------- Seleziona profilo encoder ----------------
  bool currentEncoderButton = digitalRead(ENCODER_MODE_PIN);
  if (lastEncoderButton == HIGH && currentEncoderButton == LOW) {
    EncoderProfileIndex = (EncoderProfileIndex + 1) % NUM_ENCODER_PROFILES;
         updateDisplay(profiles[KeyProfileIndex]);
    lastActivityTime = millis();
    delay(300);
  }
  lastEncoderButton = currentEncoderButton;

  // ---------------------- Aziona encoder ----------------------
  CurrentAngle = ams5600.getRawAngle();
  delta = CurrentAngle - LastAngle;
  if (delta > 2048) delta -= 4096;
  if (delta < -2048) delta += 4096;

  if (abs(delta) > Sogl) {
    Knob += (delta > 0) ? StepSize : -StepSize;
    if (Knob != oldKnob) {
      switch (EncoderProfileIndex) {
        case 0: // azione encoder per profile0
          if (oldKnob > Knob) bleKeyboard.write(DOWN_ENC_PRO_0);
          if (oldKnob < Knob) bleKeyboard.write(UP_ENC_PRO_0);
          break;
        case 1: // azione encoder per profile1
          if (oldKnob > Knob) bleKeyboard.write(DOWN_ENC_PRO_1);
          if (oldKnob < Knob) bleKeyboard.write(UP_ENC_PRO_1);
          break;
        case 2: // azione encoder per profile2
          if (oldKnob > Knob) bleKeyboard.write(DOWN_ENC_PRO_2);
          if (oldKnob < Knob) bleKeyboard.write(UP_ENC_PRO_2);
          break;
      }
      oldKnob = Knob;
      LastAngle = CurrentAngle;
      lastActivityTime = millis();
      delay(100);
    }
  }

  if (millis() - lastActivityTime > SLEEP_TIME) {
    enterDeepSleep();
  }

  // ---------Aggiorna stato batteria e BLE ogni minuto---------
  if (millis() - lastBatteryUpdate > 5000) { 

    updateBatteryStatus();

    //Connessione BLE
      bool currentBLEState = bleKeyboard.isConnected();
    if (currentBLEState != previousBLEState) {
      drawBLEStatusIcon(currentBLEState);                   // aggiorna l'icona in base allo stato BLE
      previousBLEState = currentBLEState;  
    }

    lastBatteryUpdate = millis();
  }
}
