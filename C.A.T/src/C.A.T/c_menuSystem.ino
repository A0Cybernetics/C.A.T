////////// Menu Vars //////////
uint8_t menuPageSelected = 0;
uint8_t menuOptionSelected = 0;
uint8_t menuOptionSelectedLast = 0;
const uint16_t menuReadingUpdateInterval = 200;
unsigned long menuReadingNextUpdateTime = 0;

//////////////////////////////////////////
//////         Menu options        ///////
//////////////////////////////////////////
struct menuOption {
  bool selectable;    //set true to make an option selectible by the increment options function
	String (*text)();   //returns the text to write for option line in menu
  void (*function)(); //function performed when option is selected
};

void emptyFn() {}  //empty fn to fill fn spot for options that are not selectable or have no fn (wont even be called)

///////// Main Menu Page Option  ////////
menuOption mainMenuOption {
  1,
  mainMenuOptionText,
  mainMenuOptionSelect,
};

String mainMenuOptionText() {
  return ((String) "Main Menu");
}

void mainMenuOptionSelect() {
  mainMenuSelect();
}

////////// Settings Page Options /////////
menuOption settingsOption {
  1,
  settingsOptionText,
  settingsOptionSelect,
};

String settingsOptionText() {
  return ((String) "Settings");
}

void settingsOptionSelect() {
  settingsSelect();
}

///////////// Battery Voltage Option //////////////
menuOption batteryVoltageOption {
  0,
  batteryVoltageOptionText,
  emptyFn,
};

String batteryVoltageOptionText() {
  return ((String) "Battery : " + getBatteryVoltage() + " mV" + "                  ");
}

////////////// Save Option ///////////////
menuOption saveOption {
  1,
  saveOptionText,
  saveOptionSelect,
};

String saveOptionText() {
  return ((String) "Save");
}

void saveOptionSelect() {
  savePreferences();
  settingsDraw();
}

///////////// Foreground Color Option //////////////
menuOption foregroundColorOption {
  1,
  foregroundColorOptionText,
  foregroundColorOptionSelect,
};

String foregroundColorOptionText() {
  return ((String) "FGND: " + colorArr[foregroundColorIndex].name + "                    ");
}

void foregroundColorOptionSelect() {
  incForegroundColor();
  tft.setTextColor(foregroundColor, backgroundColor);
  settingsDraw();
}

///////////// Background Color Option //////////////
menuOption backgroundColorOption {
  1,
  backgroundColorOptionText,
  backgroundColorOptionSelect,
};

String backgroundColorOptionText() {
  return ((String) "BGND: " + colorArr[backgroundColorIndex].name + "                  ");
}

void backgroundColorOptionSelect() {
  incBackgroundColor();
  tft.setTextColor(foregroundColor, backgroundColor);
  settingsDraw();
}


////////// Mouse Hack Page Options /////////
menuOption mouseHackOption {
  1,
  mouseHackOptionText,
  mouseHackOptionSelect,
};

String mouseHackOptionText() {
  return ((String) "Mouse Hack");
}

void mouseHackOptionSelect() {
  mouseHackSelect();
}

////////////// Mouse Grab Option ///////////////
menuOption mouseGrabOption {
  1,
  mouseGrabOptionText,
  mouseGrabOptionSelect,
};

String mouseGrabOptionText() {
  if (mouseGrab) {
    return ((String) "Mouse: Pressed          ");
  }
  else {
    return ((String) "Mouse: Released         ");
  }
}

void mouseGrabOptionSelect() {
  mouseToggleGrab();
  mouseHackDraw();
}

////////////// Mouse Paused Option ///////////////
menuOption mousePausedOption {
  1,
  mousePausedOptionText,
  mousePausedOptionSelect,
};

String mousePausedOptionText() {
  if (mousePaused) {
    return ((String) "Mouse: Paused        ");
  }
  else {
    return ((String) "Mouse: On            ");
  }
}

void mousePausedOptionSelect() {
  mouseTogglePaused();
  mouseHackDraw();
}

////////////// Invert Ouput Option ///////////////
menuOption invertOutputOption {
  1,
  invertOutputOptionText,
  invertOutputOptionSelect,
};

String invertOutputOptionText() {
  if (invertOutput == 0) {
    return ((String) "Invert Output: False    ");
  }
  else {
    return ((String) "Invert Output: True     ");
  }
}

void invertOutputOptionSelect() {
  toggleInvertOutput();
  mouseHackDraw();
}

////////////// Mouse Axis Option ///////////////
menuOption mouseAxisOption {
  1,
  mouseAxisOptionText,
  mouseAxisOptionSelect,
};

String mouseAxisOptionText() {
  if (mouseAxis == 0) {
    return ((String) "Mouse Axis: X");
  }
  else {
    return ((String) "Mouse Axis: Y");
  }
}

void mouseAxisOptionSelect() {
  mouseIncAxis();
  mouseHackDraw();
}

////////////// Max Output Option ///////////////
menuOption mouseMaxPxOption {
  1,
  mouseMaxPxOptionText,
  mouseMaxPxOptionSelect,
};

String mouseMaxPxOptionText() {
  return ((String) "Max Output: " + mouseMaxPx + "              ");
}

void mouseMaxPxOptionSelect() {
  mouseIncMaxPx();
  mouseHackDraw();
}

////////////// Auto Range Option ///////////////
menuOption autoRangeOption {
  1,
  autoRangeOptionText,
  autoRangeOptionSelect,
};

String autoRangeOptionText() {
  return ((String) "Automatic Range");
}

void autoRangeOptionSelect() {
  autoRange();
  mainMenuDraw();
}

///////////// Distance Option //////////////
menuOption distanceOption {
  0,
  distanceOptionText,
  emptyFn,
};

String distanceOptionText() {
  return ((String) "Distance mm: " + distance + "                  ");
}


//////////////////////////////////////////
//////   Menu Pages and Functions  ///////
//////////////////////////////////////////
const uint8_t maxOptionsArrSize = 15;       // Limit of one page based on text height and screen height
struct menuPage {
	String title;   //up to 9 Chars goes in menu bar for drawMenuBarButtons, limited by words on sides of title
  void (*draw)();
  void (*select)();
  void (*specialFunction)();
	uint8_t optionsArrSize;
	menuOption optionsArr[maxOptionsArrSize];
};

const uint8_t menuPagesSize = 3;
menuPage menuPages[menuPagesSize] = {
  {
    "Main Menu",
    mainMenuDraw,
    mainMenuSelect,
    mainMenuSpecialFunction,
    4,
    {mouseHackOption, autoRangeOption, settingsOption, batteryVoltageOption,},
  },
  {
    "Settings",
    settingsDraw,
    settingsSelect,
    settingsSpecialFunction,
    4,
    {mainMenuOption, foregroundColorOption, backgroundColorOption, saveOption,},
  },
    {
    "Mouse Hack",
    mouseHackDraw,
    mouseHackSelect,
    mouseHackSpecialFunction,
    7,
    {mouseGrabOption, mousePausedOption, invertOutputOption, mouseAxisOption, mouseMaxPxOption, mainMenuOption, distanceOption,},
  },
};

///////////// Main Menu Page //////////////
void mainMenuDraw() {
	drawMenu(menuPages[0]);
}

void mainMenuSelect() {
	menuPageSelected = 0;
	menuOptionSelected = 0;
  menuOptionSelectedLast = 0;
	mainMenuDraw();
}

void mainMenuSpecialFunction() {
  currentTime = millis();
  // Update Battery Voltage reading at set interval
  if (currentTime > batteryVoltageNextUpdateTime) {
    drawMenuOption(3);
    batteryVoltageNextUpdateTime = currentTime + batteryVoltageUpdateInterval;
  }
}

///////////// Settings Page //////////////
void settingsDraw() {
	drawMenu(menuPages[1]);
}

void settingsSelect() {
	menuPageSelected = 1;
	menuOptionSelected = 0;
  menuOptionSelectedLast = 0;
	settingsDraw();
}

void settingsSpecialFunction() {
}

///////////// Mouse Hack Page //////////////
void mouseHackDraw() {
	drawMenu(menuPages[2]);
}

void mouseHackSelect() {
	menuPageSelected = 2;
	menuOptionSelected = 0;
  menuOptionSelectedLast = 0;
	mouseHackDraw();
}

void mouseHackSpecialFunction() {
  mouseHackFn();

  currentTime = millis();
  // Update distance reading at set interval
  if (newDistanceFlag) {
    if (currentTime > menuReadingNextUpdateTime) {
      drawMenuOption(6);
      menuReadingNextUpdateTime = currentTime + menuReadingUpdateInterval;
    }
  }
}



//////////////////////////////////////////
////// Draw and control menu pages ///////
//////////////////////////////////////////
  // Draw 1 unselected menu option
void drawMenuOption(uint8_t optionToDraw) {
  tft.drawString(menuPages[menuPageSelected].optionsArr[optionToDraw].text(), 0, (optionToDraw+1)*fontHeight);
}

  // Draw 1 selected menu option
void drawMenuOptionSelected(uint8_t optionToUpdate) {
  tft.setTextColor(backgroundColor, foregroundColor);
  tft.drawString(menuPages[menuPageSelected].optionsArr[optionToUpdate].text(), 0, (optionToUpdate+1)*fontHeight);
  tft.setTextColor(foregroundColor, backgroundColor);
}

  // Update menuOptionSelected and menuOptionSelecetdLast on display. used in incOptions
void drawMenuOptions() {
  if (menuOptionSelected == menuOptionSelectedLast) {
    return;
  }
	drawMenuOptionSelected(menuOptionSelected);
  drawMenuOption(menuOptionSelectedLast);
}

  // Draw top menu bar, no button info
void drawMenuBar(){
  tft.setTextDatum(1);    // Set text reference to top middle of string
  tft.drawString(menuPages[menuPageSelected].title, 63, 0);
  tft.setTextDatum(0);    // Set text reference to default top left of string
  tft.drawFastHLine(0, fontHeight-1, screenWidth, foregroundColor);
}

  // Draw top menu bar, with button info
void drawMenuBarButtons(){
  tft.setTextDatum(1);    //set text reference to top middle of string
  tft.drawString (menuPages[menuPageSelected].title, 63, 0);
  tft.setTextDatum(0);    //set text reference to top left of string
  tft.drawString("Next", 0, 0);
  tft.setTextDatum(2);    //set text reference to top right of string
  tft.drawString("Enter", 127, 0);
  tft.setTextDatum(0);    //set text reference to default top left of string
  
  tft.drawFastHLine(0, fontHeight-1, screenWidth, foregroundColor);
  tft.drawFastVLine(31, 0, fontHeight-1, foregroundColor);
  tft.drawFastVLine(92, 0, fontHeight-1, foregroundColor);
}

  // Draw the whole menu page
void drawMenu(struct menuPage page) {
	tft.fillScreen(backgroundColor);
    // Pick menu bar style
  drawMenuBar();
  //drawMenuBarButtons();
	for (int i = 0; i < page.optionsArrSize; i++) {
		if (i == menuOptionSelected) {
			drawMenuOptionSelected(i);
		}
		else {
			drawMenuOption(i);
		}
	}
}

  // Increment through menu options on a page
void incOptions() {
	do {
    menuOptionSelected ++;
    if (menuOptionSelected >= menuPages[menuPageSelected].optionsArrSize) {
      menuOptionSelected = 0;
    }
  } while (!menuPages[menuPageSelected].optionsArr[menuOptionSelected].selectable);
  drawMenuOptions();
  menuOptionSelectedLast = menuOptionSelected;
}


  // Master menu function. Checks board buttons, performs button actions, calls menu page special function
  // Only need to call this to run whole menu system
void menuFn() {
  onboardButtonsDT(incOptions, menuPages[menuPageSelected].optionsArr[menuOptionSelected].function);
  menuPages[menuPageSelected].specialFunction();
}

