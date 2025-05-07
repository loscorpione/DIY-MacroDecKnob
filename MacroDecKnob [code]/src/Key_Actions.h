#ifndef KEY_ACTIONS_H
#define KEY_ACTIONS_H

#include <BleKeyboard.h>
#include <Arduino.h>
#include "Config.h"

// --------------Struttura per contenere modificatori, tasto principale e percorso dell'icona --------------
struct KeyAction {
  uint8_t mod1;
  uint8_t mod2;
  uint8_t mainKey;
  const char* iconFile;
};

// --------------Definisce azioni e icone per ogni tasto: [profile][row][col] --------------
static const KeyAction profileKeyActions[NUM_KEY_PROFILES][ROWS][COLS] = {
  { // Profile 0 - Filmora
    {{KEY_LEFT_CTRL, 0, 'z', "/Annulla.bmp"}, {KEY_LEFT_CTRL, 0, 'y', "/Rifai.bmp"}, {0, 0, KEY_DELETE, "/Cancella.bmp"}, {KEY_LEFT_CTRL, 0, 's', "/Salva.bmp"}},
    {{KEY_LEFT_CTRL, KEY_LEFT_ALT, 'c', "/Copia_eff.bmp"}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, 'v', "/Incolla_eff.bmp"}, {KEY_LEFT_CTRL, 0, 'd', "/Duplica.bmp"}, {0, KEY_LEFT_ALT, 'r', "/Registra.bmp"}},
    {{KEY_LEFT_CTRL, 0, KEY_NUM_MINUS, "/Timeline-.bmp"}, {KEY_LEFT_CTRL, 0, KEY_NUM_PLUS, "/Timeline+.bmp"}, {KEY_LEFT_CTRL, 0, 'b', "/Dividi.bmp"}, {KEY_LEFT_SHIFT, 0, KEY_DELETE, "/Elimina_Salti.bmp"}}
  },
  { // Profile 1 - Produzione
    {{KEY_LEFT_CTRL, 0, 'z', "/Annulla.bmp"}, {KEY_LEFT_CTRL, 0, 'y', "/Rifai.bmp"}, {0, 0, KEY_DELETE, "/Cancella.bmp"}, {KEY_LEFT_CTRL, 0, 's', "/Salva.bmp"}},
    {{0, 0, 0, "/OBS.bmp"}, {0, 0, 0, "/Filmora.bmp"}, {0, 0, 0, "/www.bmp"}, {0, 0, 0, "/Calcol.bmp"}},
    {{KEY_LEFT_CTRL, 0, 'x', "/Taglia.bmp"}, {KEY_LEFT_CTRL, 0, 'c', "/Copia.bmp"}, {KEY_LEFT_CTRL, 0, 'v', "/Incolla.bmp"}, {KEY_LEFT_CTRL, 0, 'd', "/Duplica.bmp"}}
  },
  { // Profile 2 - OBS Studio
    {{KEY_LEFT_CTRL, KEY_LEFT_ALT, '1', "/Scena1.bmp"}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, '2', "/Scena2.bmp"}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, '3', "/Scena3.bmp"}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, '4', "/Scena4.bmp"}},
    {{KEY_LEFT_CTRL, KEY_LEFT_ALT, '5', "/Vuota.bmp"}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, '6', "/Vuota.bmp"}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, '7', "/Vuota.bmp"}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, '8', "/Vuota.bmp"}},
    {{KEY_LEFT_CTRL, KEY_LEFT_ALT, '9', "/Rec.bmp"}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, 'Q', "/Pausa.bmp"}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, 'Z', "/Stream.bmp"}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, 'X', "/Vuota.bmp"}}
  }
};

#endif // KEY_ACTIONS_H
