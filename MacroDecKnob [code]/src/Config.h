#ifndef CONFIG_H
#define CONFIG_H

// ---------------- PIN ----------------
#define BACKLIGHT_PIN        19
#define WAKEUP_PIN           0
#define KEYPAD_MODE_PIN      15
#define ENCODER_MODE_PIN     17
#define BATTERY_ADC_PIN      35

// ---------------- COSTANTI ----------------
#define SLEEP_TIME           600000    // 10 minuti
#define NUM_KEY_PROFILES     3
#define NUM_ENCODER_PROFILES 3

// ---------------- TASTIERA ----------------
#define ROWS 3
#define COLS 4

// ---------------- PROFILI ---------------- 
extern const char* profiles[NUM_KEY_PROFILES];

// ---------------- DISPLAY ----------------
extern int KeyProfileIndex;
extern int EncoderProfileIndex;

#endif // CONFIG_H
