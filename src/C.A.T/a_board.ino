////////// Battery //////////
uint batteryVoltage = 0;
const uint16_t batteryVoltageUpdateInterval = 10000;
unsigned long batteryVoltageNextUpdateTime = 0;

////////// ONBOARD BUTTONS //////////
  //INPUT_PULLUP => 1 HIGH when not pressed, 0 LOW when pressed
bool but1State = 1;
bool but1StateLast = 1;
bool but2State = 1;
bool but2StateLast = 1;

unsigned long currentTime = 0;  //global var to store millis() micros()
const uint8_t butDebounceDelay = 59;
const uint16_t butHoldDelay = 500;
unsigned long but1NextDebounceTime = 0;
unsigned long but2NextDebounceTime = 0;
unsigned long but1NextHoldTime = 0;
unsigned long but2NextHoldTime = 0;
bool but1HoldFlag = 0;
bool but2HoldFlag = 0;

////////// TFT, Colors & Font //////////
struct color {
  String name;
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

  // array 140 HTML colors color structs
const uint8_t colorArrSize = 140;
color colorArr[colorArrSize] = {	
	{"Alice Blue", 0xF0, 0xF8, 0xFF},
	{"Antique White", 0xFA, 0xEB, 0xD7},
	{"Aqua", 0x00, 0xFF, 0xFF},
	{"Aquamarine", 0x7F, 0xFF, 0xD4},
	{"Azure", 0xF0, 0xFF, 0xFF},
	{"Beige", 0xF5, 0xF5, 0xDC},
	{"Bisque", 0xFF, 0xE4, 0xC4},
	{"Black", 0x00, 0x00, 0x00},
	{"Blanched Almond", 0xFF, 0xEB, 0xCD},
	{"Blue", 0x00, 0x00, 0xFF},
	{"Blue Violet", 0x8A, 0x2B, 0xE2},
	{"Brown", 0xA5, 0x2A, 0x2A},
	{"Burly Wood", 0xDE, 0xB8, 0x87},
	{"Cadet Blue", 0x5F, 0x9E, 0xA0},
	{"Chartreuse", 0x7F, 0xFF, 0x00},
	{"Chocolate", 0xD2, 0x69, 0x1E},
	{"Coral", 0xFF, 0x7F, 0x50},
	{"Cornflower Blue", 0x64, 0x95, 0xED},
	{"Cornsilk", 0xFF, 0xF8, 0xDC},
	{"Crimson", 0xDC, 0x14, 0x3C},
	{"Cyan", 0x00, 0xFF, 0xFF},
	{"Dark Blue", 0x00, 0x00, 0x8B},
	{"Dark Cyan", 0x00, 0x8B, 0x8B},
	{"Dark Golden Rod", 0xB8, 0x86, 0x0B},
	{"Dark Gray", 0xA9, 0xA9, 0xA9},
	{"Dark Green", 0x00, 0x64, 0x00},
	{"Dark Khaki", 0xBD, 0xB7, 0x6B},
	{"Dark Magenta", 0x8B, 0x00, 0x8B},
	{"Dark Olive Green", 0x55, 0x6B, 0x2F},
	{"Dark Orange", 0xFF, 0x8C, 0x00},
	{"Dark Orchid", 0x99, 0x32, 0xCC},
	{"Dark Red", 0x8B, 0x00, 0x00},
	{"Dark Salmon", 0xE9, 0x96, 0x7A},
	{"Dark Sea Green", 0x8F, 0xBC, 0x8F},
	{"Dark Slate Blue", 0x48, 0x3D, 0x8B},
	{"Dark Sate Gray", 0x2F, 0x4F, 0x4F},
	{"Dark Turquoise", 0x00, 0xCE, 0xD1},
	{"Dark Violet", 0x94, 0x00, 0xD3},
	{"Deep Pink", 0xFF, 0x14, 0x93},
	{"Deep Sky Blue", 0x00, 0xBF, 0xFF},
	{"Dim Gray", 0x69, 0x69, 0x69},
	{"Dodger Blue", 0x1E, 0x90, 0xFF},
	{"Fire Brick", 0xB2, 0x22, 0x22},
	{"Floral White", 0xFF, 0xFA, 0xF0},
	{"Forest Green", 0x22, 0x8B, 0x22},
	{"Fuchsia", 0xFF, 0x00, 0xFF},
	{"Gainsboro", 0xDC, 0xDC, 0xDC},
	{"Ghost White", 0xF8, 0xF8, 0xFF},
	{"Gold", 0xFF, 0xD7, 0x00},
	{"Golden Rod", 0xDA, 0xA5, 0x20},
	{"Gray", 0x80, 0x80, 0x80},
	{"Green", 0x00, 0x80, 0x00},
	{"Green Yellow", 0xAD, 0xFF, 0x2F},
	{"Honey Dew", 0xF0, 0xFF, 0xF0},
	{"Hot Pink", 0xFF, 0x69, 0xB4},
	{"Indian Red", 0xCD, 0x5C, 0x5C},
	{"Indigo", 0x4B, 0x00, 0x82},
	{"Ivory", 0xFF, 0xFF, 0xF0},
	{"Khaki", 0xF0, 0xE6, 0x8C},
	{"Lavender", 0xE6, 0xE6, 0xFA},
	{"Lavender Blush", 0xFF, 0xF0, 0xF5},
	{"Lawn Green", 0x7C, 0xFC, 0x00},
	{"Lemon Chiffon", 0xFF, 0xFA, 0xCD},
	{"Light Blue", 0xAD, 0xD8, 0xE6},
	{"Light Coral", 0xF0, 0x80, 0x80},
	{"Light Cyan", 0xE0, 0xFF, 0xFF},
	{"Light Golden Rod Yellow", 0xFA, 0xFA, 0xD2},
	{"Light Gray", 0xD3, 0xD3, 0xD3},
	{"Light Green", 0x90, 0xEE, 0x90},
	{"Light Pink", 0xFF, 0xB6, 0xC1},
	{"Light Salmon", 0xFF, 0xA0, 0x7A},
	{"Light Sea Green", 0x20, 0xB2, 0xAA},
	{"Light Sky Blue", 0x87, 0xCE, 0xFA},
	{"Light Slate Gray", 0x77, 0x88, 0x99},
	{"Light Steel Blue", 0xB0, 0xC4, 0xDE},
	{"Light Yellow", 0xFF, 0xFF, 0xE0},
	{"Lime", 0x00, 0xFF, 0x00},
	{"Lime Green", 0x32, 0xCD, 0x32},
	{"Linen", 0xFA, 0xF0, 0xE6},
	{"Magenta", 0xFF, 0x00, 0xFF},
	{"Maroon", 0x80, 0x00, 0x00},
	{"Medium Aqua Marine", 0x66, 0xCD, 0xAA},
	{"Medium Blue", 0x00, 0x00, 0xCD},
	{"Medium Orchid", 0xBA, 0x55, 0xD3},
	{"Medium Purple", 0x93, 0x70, 0xDB},
	{"Medium Sea Green", 0x3C, 0xB3, 0x71},
	{"Medium Slate Blue", 0x7B, 0x68, 0xEE},
	{"Medium Spring Green", 0x00, 0xFA, 0x9A},
	{"Medium Turquoise", 0x48, 0xD1, 0xCC},
	{"Medium Violet Red", 0xC7, 0x15, 0x85},
	{"Midnight Blue", 0x19, 0x19, 0x70},
	{"Mint Cream", 0xF5, 0xFF, 0xFA},
	{"Misty Rose", 0xFF, 0xE4, 0xE1},
	{"Moccasin", 0xFF, 0xE4, 0xB5},
	{"Navajo White", 0xFF, 0xDE, 0xAD},
	{"Navy", 0x00, 0x00, 0x80},
	{"Old Lace", 0xFD, 0xF5, 0xE6},
	{"Olive", 0x80, 0x80, 0x00},
	{"Olive Drab", 0x6B, 0x8E, 0x23},
	{"Orange", 0xFF, 0xA5, 0x00},
	{"Orange Red", 0xFF, 0x45, 0x00},
	{"Orchid", 0xDA, 0x70, 0xD6},
	{"Pale Golden Rod", 0xEE, 0xE8, 0xAA},
	{"Pale Green", 0x98, 0xFB, 0x98},
	{"Pale Turquoise", 0xAF, 0xEE, 0xEE},
	{"Pale Violet Red", 0xDB, 0x70, 0x93},
	{"Papaya Whip", 0xFF, 0xEF, 0xD5},
	{"Peach Puff", 0xFF, 0xDA, 0xB9},
	{"Peru", 0xCD, 0x85, 0x3F},
	{"Pink", 0xFF, 0xC0, 0xCB},
	{"Plum", 0xDD, 0xA0, 0xDD},
	{"Powder Blue", 0xB0, 0xE0, 0xE6},
	{"Purple", 0x80, 0x00, 0x80},
	{"Red", 0xFF, 0x00, 0x00},
	{"Rosy Brown", 0xBC, 0x8F, 0x8F},
	{"Royal Blue", 0x41, 0x69, 0xE1},
	{"Saddle Brown", 0x8B, 0x45, 0x13},
	{"Salmon", 0xFA, 0x80, 0x72},
	{"Sandy Brown", 0xF4, 0xA4, 0x60},
	{"Sea Green", 0x2E, 0x8B, 0x57},
	{"Sea Shell", 0xFF, 0xF5, 0xEE},
	{"Sienna", 0xA0, 0x52, 0x2D},
	{"Silver", 0xC0, 0xC0, 0xC0},
	{"Sky Blue", 0x87, 0xCE, 0xEB},
	{"Slate Blue", 0x6A, 0x5A, 0xCD},
	{"Slate Gray", 0x70, 0x80, 0x90},
	{"Snow", 0xFF, 0xFA, 0xFA},
	{"Spring Green", 0x00, 0xFF, 0x7F},
	{"Steel Blue", 0x46, 0x82, 0xB4},
	{"Tan", 0xD2, 0xB4, 0x8C},
	{"Teal", 0x00, 0x80, 0x80},
	{"Thistle", 0xD8, 0xBF, 0xD8},
	{"Tomato", 0xFF, 0x63, 0x47},
	{"Turquoise", 0x40, 0xE0, 0xD0},
	{"Violet", 0xEE, 0x82, 0xEE},
	{"Wheat", 0xF5, 0xDE, 0xB3},
	{"White", 0xFF, 0xFF, 0xFF},
	{"White Smoke", 0xF5, 0xF5, 0xF5},
	{"Yellow", 0xFF, 0xFF, 0x00},
	{"Yellow Green", 0x9A, 0xCD, 0x32},
};

uint8_t foregroundColorIndex = 56;     // White 136, Indigo 56
uint16_t foregroundColor = 0x4810;     // White 0xFFFF, Indigo 0x4810
uint8_t backgroundColorIndex = 7;      // Black 7
uint16_t backgroundColor = 0x0000;     // Black 0x0000

const uint8_t screenSize = 128;        //T QT Pro screen 128 x 128 px
const uint8_t screenWidth = 128;       // T-Display-S3 screen is 170 X 320 px
const uint8_t screenHeight = 128;
const uint8_t screenCenter = 63;
uint16_t tftFont = 1;
uint8_t fontHeight = 8;

//////////////////////////////////////////
//////       I2C Connections       ///////
//////////////////////////////////////////
void wireSetup() {
    // Start wire for QWIIC I2C
  Wire.begin(PIN_QWIIC_SDA, PIN_QWIIC_SCL, 400000);
}

//////////////////////////////////////////
/////////       Prefrences       /////////
//////////////////////////////////////////
  // load prefrences and initialize pref vars
void loadPreferences() {
  preferences.begin("SS", false);
  foregroundColorIndex = preferences.getUChar("FC", 76);  // Default Orange
  backgroundColorIndex = preferences.getUChar("BC", 7);   // Default Black
  preferences.end();

  foregroundColor = rgb16Color(colorArr[foregroundColorIndex]);
  backgroundColor = rgb16Color(colorArr[backgroundColorIndex]);
}

  //save vars to prefs
void savePreferences() {
  preferences.begin("SS", false);
  preferences.putUChar("FC", foregroundColorIndex);
  preferences.putUChar("BC", backgroundColorIndex);
  preferences.end();
}

//////////////////////////////////////////
/////////         Battery        /////////
//////////////////////////////////////////
uint getBatteryVoltage() {
  return batteryVoltage = analogRead(PIN_BAT_VOLT)*2;   //est
}

//////////////////////////////////////////
//////       TFTScreen & Color     ///////
//////////////////////////////////////////
void tftSetup(){  // Screen Setup
  tft.init();
  tft.setRotation(2);   // Rotate screen - 0 USB on top, 2 USB on Bottom
  tft.fillScreen(backgroundColor);
  tft.setTextSize(1);
  tft.setTextFont(tftFont);
  fontHeight = tft.fontHeight(tftFont);
  tft.setTextColor(foregroundColor, backgroundColor);

  drawA0Animation(63, 59, 7);
}

  // Startup Animation
  //drawA0Animation(63, 59, 7); // dependent on screen size/ desired size of animation:  (63, 60, 7);
void drawA0Animation(int32_t centerX, int32_t centerY, int size) {
  //tft.fillScreen(backgroundColor);
  unsigned long startTime = 0;
  unsigned long targetTime = 0; 
  unsigned long renderTime = random(333);
  for (int i = 1; i < size+1; i++){
    startTime = millis();
    targetTime = startTime + renderTime;
    int im1= i-1;
    int im18 = 8*im1;
    int i8 = 8*i;
    int im17 = 7*im1;
    int i7 = 7*i;
    int im14 = 4*im1;
    int i4 = 4*i;
    tft.drawLine (centerX, centerY-im18, centerX, centerY-i8, foregroundColor);
    tft.drawLine (centerX, centerY+im18, centerX, centerY+i8, foregroundColor);
    tft.drawLine (centerX-im14, centerY-im17, centerX-i4, centerY-i7, foregroundColor);
    tft.drawLine (centerX+im14, centerY+im17, centerX+i4, centerY+i7, foregroundColor);
    tft.drawLine (centerX-im17, centerY-im14, centerX-i7, centerY-i4, foregroundColor);
    tft.drawLine (centerX+im17, centerY+im14, centerX+i7, centerY+i4, foregroundColor);
    tft.drawLine (centerX-im18, centerY, centerX-i8, centerY, foregroundColor);
    tft.drawLine (centerX+im18, centerY, centerX+i8, centerY, foregroundColor);
    tft.drawLine (centerX-im17, centerY+im14, centerX-i7, centerY+i4, foregroundColor);
    tft.drawLine (centerX+im17, centerY-im14, centerX+i7, centerY-i4, foregroundColor);
    tft.drawLine (centerX-im14, centerY+im17, centerX-i4, centerY+i7, foregroundColor);
    tft.drawLine (centerX+im14, centerY-im17, centerX+i4, centerY-i7, foregroundColor);
    while (millis() <= targetTime) {}
  }
  tft.setTextDatum(1);
	tft.drawString("A0Cybernetics", 63, 119);
	tft.setTextDatum(0);
  delay(2222);
}

  // returns a 16bit rgb color from a color struct for TFT_eSPI uses
uint16_t rgb16Color(struct color colorStruct) {
	return ((colorStruct.r & 0xF8)<<8 | (colorStruct.g & 0xFC)<<3 | colorStruct.b>>3);
}

  // Increment foregroundColorIndex var
void incForegroundColor() {
  if (foregroundColorIndex < colorArrSize - 1) {
    foregroundColorIndex++;
  }
  else {
    foregroundColorIndex = 0;
  }
  foregroundColor = rgb16Color(colorArr[foregroundColorIndex]);
}

  // Increment background Color var
void incBackgroundColor() {
  if (backgroundColorIndex < colorArrSize - 1) {
    backgroundColorIndex++;
  }
  else {
    backgroundColorIndex = 0;
  }
  backgroundColor = rgb16Color(colorArr[backgroundColorIndex]);
}


//////////////////////////////////////////
//////       Onboard Buttons       ///////
//////////////////////////////////////////
void onboardButtonsDT(void (*but1Action)(), void (*but2Action)()) {
  but1State = digitalRead(PIN_BUTTON_1);
  but2State = digitalRead(PIN_BUTTON_2);
    // button 1 actions
	if (but1State != but1StateLast) {
    but1StateLast = but1State;
		but1NextDebounceTime = millis() + butDebounceDelay;
	}
	else if (but1State == 0) {
    currentTime = millis();
	  if (but1HoldFlag == 0) {
			if (currentTime > but1NextDebounceTime) {
				  // button 1 keydown past debounce
				but1Action();
				but1HoldFlag = 1;
        but1NextHoldTime = currentTime + butHoldDelay;
			}
		}
		else if (but1HoldFlag == 1) {
			if (currentTime > but1NextHoldTime) {
				  // button 1 held down past hold
        but1Action();
				but1NextHoldTime = currentTime + butHoldDelay;
			}
		}
	}
	else if (but1HoldFlag == 1) {
		if (but1State == 1) {
      currentTime = millis();
			if (currentTime > but1NextDebounceTime) {
				  // button 1 keyup past debounce
				but1HoldFlag = 0;
			}
		}
	}

  //button 2 actions
	if (but2State != but2StateLast) {
    but2StateLast = but2State;
		but2NextDebounceTime = millis() + butDebounceDelay;
	}
	else if (but2State == 0) {
    currentTime = millis();
	  if (but2HoldFlag == 0) {
			if (currentTime > but2NextDebounceTime) {
				  // button 2 keydown past debounce
        but2Action();
				but2HoldFlag = 1;
				but2NextHoldTime = currentTime + butHoldDelay;
			}
		}
		else if (but2HoldFlag == 1) {
			if (currentTime > but2NextHoldTime) {
				  // button 2 held down past hold
        but2Action();
				but2NextHoldTime = currentTime + butHoldDelay;
			}
		}
	}
	else if (but2HoldFlag == 1) {
		if (but2State == 1) {
      currentTime = millis();
			if (currentTime > but2NextDebounceTime) {
				  // button 2 keyup past debounce
				but2HoldFlag = 0;
			}
		}
	}
}
