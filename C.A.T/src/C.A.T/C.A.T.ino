#include <Arduino.h>
#include <Wire.h>

// Prefrences library for saving data to onboard flash from espressif
// https://github.com/espressif/arduino-esp32
// https://docs.espressif.com/projects/arduino-esp32/en/latest/api/preferences.html
#include <Preferences.h>
Preferences preferences;

// USB Mouse - from espressif board library
#include "USB.h"
#include "USBHIDMouse.h"
USBHIDMouse Mouse;

// Screen
// *Have to modify User_Setup_Select.h to select board. Uncomment //#include <User_Setups/Setup211_LilyGo_T_QT_Pro_S3.h> and comment out //#include <User_Setup.h>
// *Also: Rollback esp32 Boards Manager to 2.0.14 - Screen wouldnt work on newer releases
// https://github.com/Bodmer/TFT_eSPI
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

// VL53L1X Sensor lib
// https://github.com/pololu/vl53l1x-arduino
#include <VL53L1X.h>
VL53L1X sensorV1X;

// 1Euro Filter for VL53L1X data
// https://github.com/casiez/OneEuroFilterArduino
#include "1euroFilter.h"
static OneEuroFilter distanceFilter;

// Board Pin Definitions
#define PIN_BUTTON_1 47 // KEY onboard button
#define PIN_BUTTON_2 0  // IO0 onboard button
#define PIN_BAT_VOLT 4  // Battery voltage analog; analogRead(PIN_BAT_VOLT)
#define PIN_LCD_BL 10 // Screen backlight pin, screen backlight takes 20mA

// QWIIC pin defs
#define PIN_QWIIC_SDA 43
#define PIN_QWIIC_SCL 44

//////////////////////////////////////////
/////////     Main Functions     /////////
//////////////////////////////////////////

 void setup() {
  Serial.begin(115200);     //9600  115200
    // Setup onboard buttons
  pinMode(PIN_BUTTON_1, INPUT_PULLUP);
  pinMode(PIN_BUTTON_2, INPUT_PULLUP);
    // load prefrences and initialize pref vars
  loadPreferences();
    // Setup I2C
  wireSetup();
    // Setup screen
  tftSetup();
    // Setup ToF Sensor
  VL53L1XSetup();
    // Select starting menu page
  mainMenuSelect();
    // Initialize mouse control:
  Mouse.begin();
  USB.begin();
}

void loop() {
  menuFn();
}
