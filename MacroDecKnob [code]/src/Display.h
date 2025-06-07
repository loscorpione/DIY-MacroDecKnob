// Display.h
#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h>
#include <FS.h>
#include <SPIFFS.h>
#include "Config.h"
#include "User_Config.h"


extern TFT_eSPI tft;
extern int KeyProfileIndex;
extern int EncoderProfileIndex;
extern const char* encoderNames[]; // Declaration; ensure definition exists in a .cpp file

// --- Funzioni di supporto per leggere BMP da SPIFFS ---
  static uint16_t read16(fs::File &f) {
   uint16_t result;
   ((uint8_t *)&result)[0] = f.read();
   ((uint8_t *)&result)[1] = f.read();
   return result;
  }

  static uint32_t read32(fs::File &f) {
    uint32_t result;
    ((uint8_t *)&result)[0] = f.read();
   ((uint8_t *)&result)[1] = f.read();
   ((uint8_t *)&result)[2] = f.read();
   ((uint8_t *)&result)[3] = f.read();
    return result;
  }

// Disegna un BMP 24-bit non compresso da SPIFFS
static void tftDrawBmp(const char *filename, int16_t x, int16_t y) {
  fs::File bmp = SPIFFS.open(filename, "r");
  if (!bmp) return;
  if (read16(bmp) != 0x4D42) { bmp.close(); return; } // signature 'BM'
  bmp.seek(10);
  uint32_t offset = read32(bmp);
  bmp.seek(18);
  int32_t w = read32(bmp);
  int32_t h = read32(bmp);
  bmp.seek(offset);
  uint8_t padding = (4 - ((w * 3) & 3)) & 3;
  for (int row = h - 1; row >= 0; row--) {
    for (int col = 0; col < w; col++) {
      uint8_t b = bmp.read();
      uint8_t g = bmp.read();
      uint8_t r = bmp.read();
      tft.drawPixel(x + col, y + row, tft.color565(r, g, b));
    }
    bmp.seek(bmp.position() + padding);
  }
  bmp.close();
}

//--------- Disegna icona batteria ---------
inline void drawBatteryIcon(int x, int y, float voltage) {
  int level = map(voltage * 100, 340, 420, 0, 100);
  level = constrain(level, 0, 100);
  tft.fillRect(x, y, 40, 20, TFT_BLACK);
  tft.drawRoundRect(x, y, 35, 16, 2, TFT_WHITE);
  tft.fillRect(x + 35, y + 5, 3, 6, TFT_WHITE);
  int fillW = map(level, 0, 100, 0, 33);
  if (fillW) {
    uint16_t c = (level > 66) ? TFT_GREEN : (level > 33) ? TFT_YELLOW : TFT_RED;
    tft.fillRoundRect(x + 1, y + 1, fillW, 14, 1, c);
  }
}

//--------- Aggiorna riquadri profili ---------
inline void updateDisplay(const char *keyName) {
  tft.fillRect(0, 0, 270, 48, TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.fillRoundRect(1, 1, 270, 25, 4, TFT_BLUE);
  tft.setCursor(7, 7); tft.print("KEY:"); tft.print(keyName);
  tft.fillRoundRect(1, 30, 270, 25, 4, TFT_BLUE);
  tft.setCursor(7, 34); tft.print("KNOB:");
  if (EncoderProfileIndex < NUM_ENCODER_PROFILES) {
    tft.print(encoderNames[EncoderProfileIndex]);
  } else {
    tft.print("Altro");
}

}

// --- Disegna la matrice 3x4 di icone con spaziatura ---
inline void drawKeyIcons() {
  const int iconSizeX = 76;
  const int iconSizeY = 55;
  const int spacingX = 5;
  const int spacingY = 5;
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      KeyAction act = profileKeyActions[KeyProfileIndex][r][c];
      if (act.iconFile && strlen(act.iconFile) > 0) {
        int x = c * (iconSizeX + spacingX);       // inizia da X = 0
        int y = 60 + r * (iconSizeY + spacingY);   // inizia da Y = 60
        tftDrawBmp(act.iconFile, x, y);
      }
    }
  }
}
 
//--------- Visualizza iconaBLE se connesso ---------
inline void drawBLEStatusIcon(bool connected) {
  int x = 278;  // Regola in base alla tua interfaccia
  int y = 32;

  if (connected) {
    // Mostra l'icona BLE
    fs::File icon = SPIFFS.open("/BLEOK.bmp", "r");
    if (icon) {
      tftDrawBmp("/BLEOK.bmp", x, y); // Assicurati che drawBmp sia disponibile o usa metodo simile
      icon.close();
    }
  } else {
    // Cancella l'area (ad esempio riempiendo con sfondo nero)
    tft.fillRect(x, y, 40, 20, TFT_BLACK);
  }
}

//--------- Visualizza logo iniziale ---------
inline void showLogo(const char* filename, uint16_t durationMs) {
  File logoFile = SPIFFS.open(filename, "r");
  if (!logoFile) {
    //Serial.println("❌ Logo non trovato!");
    return;
  }

  // Centra il logo sul display (modifica se il tuo display ha dimensioni diverse)
  int x = (tft.width() - 230) / 2;
  int y = (tft.height() - 230) / 2;

  //Serial.println("✅ Mostro il logo iniziale...");
  tft.fillScreen(TFT_BLACK);
  tftDrawBmp(filename, x, y);  // Assicurati che `drawBmp()` sia definita correttamente
  logoFile.close();
  delay(durationMs);
  tft.fillScreen(TFT_BLACK);
}


#endif // DISPLAY_H