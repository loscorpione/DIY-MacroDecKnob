#ifndef USER_CONFIG_H
#define USER_CONFIG_H

#include <BleKeyboard.h>
#include <Arduino.h>
#include "Config.h"


// -----------------------------------------------------
// DEFINIZIONE TEMPO DI INATTIVITA' PER  SONNO PROFONDO 
// -----------------------------------------------------
#define SLEEP_TIME           600000                             // Time before Sleep 600000 millis = 10 minuti

// ----------------------------------------
// DEFINIZIONE NOMI DEI PROFILI DEL KEYPAD
// ----------------------------------------
const char* profiles[NUM_KEY_PROFILES] = {
  "Filmora",                                                     // Nome keypad Profile 0
  "Produzione",                                                  // Nome keypad Profile 1
  "OBS Studio"                                                   // Nome keypad Profile 2
};

// ------------------------------
// STRUTTURA DATI DEI TASTI
// ------------------------------
// Ogni KeyAction rappresenta:
// - mod1: primo modificatore (es: KEY_LEFT_CTRL)
// - mod2: secondo modificatore (es: KEY_LEFT_ALT)
// - mainKey: tasto principale (es: 'z')
// - iconFile: path al file BMP in SPIFFS da visualizzare
struct KeyAction {
  uint8_t mod1;
  uint8_t mod2;
  uint8_t mainKey;
  const char* iconFile;
};

// ------------------------------
// TABELLA SHORTCUT DEI PROFILI TASTIERA
// ------------------------------
static const KeyAction profileKeyActions[NUM_KEY_PROFILES][ROWS][COLS] = {

  { // Profile 0 - Filmora  --------------------------------------------------
    {{KEY_LEFT_CTRL, 0, 's', "/Salva.bmp"},                       // KEY 1 
    {0, 0, KEY_DELETE, "/Cancella.bmp"},                          // KEY 2
    {KEY_LEFT_CTRL, 0, 'z', "/Annulla.bmp"},                      // KEY 3
    {KEY_LEFT_CTRL, 0, 'y', "/Rifai.bmp"}},                       // KEY 4

    {{KEY_LEFT_CTRL, KEY_LEFT_ALT, 'c', "/Copia_eff.bmp"},        // KEY 5
    {KEY_LEFT_CTRL, KEY_LEFT_ALT, 'v', "/Incolla_eff.bmp"},       // KEY 6 
    {KEY_LEFT_CTRL, 0, 'd', "/Duplica.bmp"},                      // KEY 7
    {0, KEY_LEFT_ALT, 'v', "/Rec_Audio.bmp"}},                    // KEY 8

    {{KEY_LEFT_CTRL, 0, KEY_NUM_MINUS, "/Timeline-.bmp"},         // KEY 9
    {KEY_LEFT_CTRL, 0, KEY_NUM_PLUS, "/Timeline+.bmp"},           // KEY 10
    {KEY_LEFT_CTRL, 0, 'b', "/Dividi.bmp"},                       // KEY 11
    {KEY_LEFT_SHIFT, 0, KEY_DELETE, "/Elimina_Salti.bmp"}}        // KEY 12
  },

  { // Profile 1 - Produzione --------------------------------------------------
    {{KEY_LEFT_CTRL, 0, 's', "/Salva.bmp"},                       // KEY 1 
    {0, 0, KEY_DELETE, "/Cancella.bmp"},                          // KEY 2
    {KEY_LEFT_CTRL, 0, 'z', "/Annulla.bmp"},                      // KEY 3
    {KEY_LEFT_CTRL, 0, 'y', "/Rifai.bmp"}},                       // KEY 4

    {{0, 0, 0, "/Vuota.bmp"},                                     // KEY 5        
    {0, 0, 0, "/Vuota.bmp"},                                      // KEY 6 
    {0, 0, 0, "/Vuota.bmp"},                                      // KEY 7
    {0, 0, 0, "/Vuota.bmp"}},                                     // KEY 8

    {{KEY_LEFT_CTRL, 0, 'x', "/Taglia.bmp"},                      // KEY 9  
    {KEY_LEFT_CTRL, 0, 'c', "/Copia.bmp"},                        // KEY 10
    {KEY_LEFT_CTRL, 0, 'v', "/Incolla.bmp"},                      // KEY 11
    {KEY_LEFT_CTRL, 0, 'd', "/Duplica.bmp"}}                      // KEY 12
  },

  { // Profile 2 - OBS Studio --------------------------------------------------
    {{KEY_LEFT_CTRL, KEY_LEFT_ALT, '1', "/Scena1.bmp"},           // KEY 1
    {KEY_LEFT_CTRL, KEY_LEFT_ALT, '2', "/Scena2.bmp"},            // KEY 2
    {KEY_LEFT_CTRL, KEY_LEFT_ALT, '3', "/Scena3.bmp"},            // KEY 3
    {KEY_LEFT_CTRL, KEY_LEFT_ALT, '4', "/Scena4.bmp"}},           // KEY 4

    {{KEY_LEFT_CTRL, KEY_LEFT_ALT, '5', "/Scena5.bmp"},           // KEY 5
    {KEY_LEFT_CTRL, KEY_LEFT_ALT, '6', "/Scena6.bmp"},            // KEY 6
    {KEY_LEFT_CTRL, KEY_LEFT_ALT, '7', "/Scena7.bmp"},            // KEY 7
    {KEY_LEFT_CTRL, KEY_LEFT_ALT, '8', "/Scena8.bmp"}},           // KEY 8

    {{KEY_LEFT_CTRL, KEY_LEFT_ALT, '9', "/Stream.bmp"},           // KEY 9
    {KEY_LEFT_CTRL, KEY_LEFT_ALT, 'Q', "/Rec.bmp"},               // KEY 10
    {KEY_LEFT_CTRL, KEY_LEFT_ALT, 'Z', "/Mute_mic.bmp"},          // KEY 11
    {KEY_LEFT_CTRL, KEY_LEFT_ALT, 'X', "/Pausa.bmp"}}             // KEY 12
  }
};


// --------------------------------------------
// DEFINIZIONE NOMI DEI PROFILI DELL'ENCODER
// --------------------------------------------
const char* encoderNames[NUM_ENCODER_PROFILES] = {
  "Volume",                                                       // Nome keypad Profile 0
  "Timeline",                                                     // Nome keypad Profile 1
  "Scroll"                                                        // Nome keypad Profile 2
};


// --------------------------------------------------------
// FUNZIONI ASSOCIATE AL KEYPAD 
// Definiti come costanti simboliche in modo centralizzato
// Definiste azioni per la rotazione oraria e antioraria 
// dell'keypad per ogni Profilo
// --------------------------------------------------------

// Profile 0 - Volume
 #define DOWN_ENC_PRO_0 KEY_MEDIA_VOLUME_DOWN                     // Azione rotazione antioraria
 #define UP_ENC_PRO_0 KEY_MEDIA_VOLUME_UP                         // Azione rotazione oraria 

 // Profile 1 - Timeline
 #define DOWN_ENC_PRO_1 KEY_LEFT_ARROW                            // Azione rotazione antioraria
 #define UP_ENC_PRO_1 KEY_RIGHT_ARROW                             // Azione rotazione oraria

 // Profile 2 - Scroll
 #define DOWN_ENC_PRO_2 KEY_PAGE_UP                              // Azione rotazione antioraria
 #define UP_ENC_PRO_2 KEY_PAGE_DOWN                              // Azione rotazione oraria


#endif // USER_CONFIG_H
