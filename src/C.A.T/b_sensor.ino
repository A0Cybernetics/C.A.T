//////////////////////////////////////////
//////   VL53L1X TOF SENSOR Vars   ///////
//////////////////////////////////////////
//unsigned long lastReadingTime = 0;        // For timing testing of functions
bool newDistanceFlag = 0;                 // Indicate if there is a new distance reading
uint16_t distance = 0;                    // Store distance reading of sensor
uint16_t distanceFiltered = 0;            // Store filtered distance reading

unsigned long autoRangeTime = 10000;      // Time that auto range function runs for
uint16_t rangeMax = 200;                  // Upper limit of used sensor distance range, set in autoRange
uint16_t rangeMin = 100;                  // Lower limit of used sensor distance range, set in autoRange
uint16_t strokeLength = 100;              // Length of stroke, strokeLength = rangeMax-rangeMin , set in autoRange
bool invertOutput = 0;                    // Invert mapping of distance to output variable, if 1: increase distance = decrease output
int16_t mouseMinPx = 0;                   // -127 (-128) max without fancy Mouse.move Maximum Value -32768
int16_t mouseMaxPx = 125;                 // 127 max (125 for 5 px increment) without fancy Mouse.move fn // Maximum Value 32767
int16_t mouseMaxPxLimit = 250;            // 127 max (125 for 5 px increment) without fancy Mouse.move fn // Maximum Value 32767
int16_t mousePxRaw = 0;                   // Raw mapping value of distance to position range in pixels
int16_t mousePxFiltered = 0;              // Filtered position value
int8_t mouseAxis = 0;           // Set axis of mouse movement 0 = x axis, 1 = y axis
int16_t mousePxLast = 0;       // Position of mouse after the last Mouse.move()
int16_t mousePxChange = 0;     // Mouse uses int8_t value for moving mouse, keep movement range per Mouse.move() : -128 to 127 px; will limit to abs 127 per move (limited by Mouse.move number type int8_t)
bool mouseGrab = 0;             // Press or release mouse button to grab; 1 = mouse button pressed, 0 = mouse button released
bool mousePaused = 0;           // Pause moving mouse 0 = mouse button released, 1 = mouse paused

// 1 Euro Filter vars
float FREQUENCY = 50.0;       // [Hz]
float MINCUTOFF = 3.4E+38;    // [Hz] needs to be tuned according to your application
float BETA = 3.4E+38;         // needs to be tuned according to your application
unsigned long filterStartTime = 0;


//////////////////////////////////////////
//////      VL53L1X TOF SENSOR     ///////
//////////////////////////////////////////
void VL53L1XSetup() {
   // Start VL53L1X = sensorV1X
  sensorV1X.setTimeout(500);
  if (!sensorV1X.init()) {
    while (1);
  }

  // Use long distance mode and allow up to 50000 us (50 ms) for a measurement.
  // You can change these settings to adjust the performance of the sensor, but
  // the minimum timing budget is 20 ms for short distance mode and 33 ms for
  // medium and long distance modes. See the VL53L1X datasheet for more

  //************** This library sets up the sensor to use the LOWPOWER_AUTONOMOUS preset mode, and in that mode, John Kvam recommends using an 
  //inter-measurement period (the value passed to startContinuous() in our library) of at least 5 ms longer than the timing budget.
  // So using startContinuous(25) seems to work and consistently give me readings every 25 ms. Or if you really need readings
  // every 20 ms, it also seems to work if I do setMeasurementTimingBudget(15000) and startContinuous(20), even though 15 ms is below the supposed minimum timing budget of 20 ms .

  // information on range and timing limits.
  sensorV1X.setDistanceMode(VL53L1X::Short);
  sensorV1X.setMeasurementTimingBudget(15000);   //2000
  //sensorV1X.setDistanceMode(VL53L1X::Medium);
  //sensorV1X.setMeasurementTimingBudget(33000);
  //sensorV1X.setDistanceMode(VL53L1X::Long);
  //sensor.V1XsetMeasurementTimingBudget(50000);

  //sensorV1X.setROISize(16, 16);  // Default
  //sensorV1X.setROISize(4, 4);    // Minimum

  // Start continuous readings at a rate of one measurement every 50 ms (the
  // inter-measurement period). This period should be at least as long as the
  // timing budget.
  sensorV1X.startContinuous(20);    //20
  //sensorV1X.startContinuous(33);
  //sensorV1X.startContinuous(50); 

  //sensorV1X.stopContinuous();
}

// Check distance and set new distance flag if there is a new distance reading
void checkDistance() {
  if (sensorV1X.dataReady()) {            // if new measurement for the taking!
    sensorV1X.read();
    if (sensorV1X.ranging_data.range_status == 0) {   // if a valid distance reading
      distance = sensorV1X.ranging_data.range_mm;
      newDistanceFlag = 1;
    }
    else {
      newDistanceFlag = 0;
    }
  }
  else {
    newDistanceFlag = 0;
  }
}

//////////////////////////////////////////
//////      1 Euro Filter FNS      ///////
//////////////////////////////////////////

void setupDistanceFilter () {
    distanceFilter.begin(FREQUENCY, MINCUTOFF, BETA);
    filterStartTime = micros();
}

// Apply 1 Euro Filter to raw distance reading to set distanceFiltered
int16_t filterDistance() {
  distanceFiltered = distanceFilter.filter(distance);
  return distanceFiltered;
}

//////////////////////////////////////////
//////       Mouse Hack FNS        ///////
//////////////////////////////////////////
// Set mousePXFiltered based on distanceFiltered and settings vars
int16_t mouseProcessPxFiltered() {
    // Map TOF distanceFiltered inverted to mousePxFiltered (increase distance = decrease mousePxRaw); if invert and x axis, or if not inverted and y axis
  if (invertOutput && (mouseAxis == 0) || !invertOutput && (mouseAxis == 1)) {
    mousePxFiltered = map(constrain(distanceFiltered, rangeMin, rangeMax), rangeMin, rangeMax, mouseMaxPx, mouseMinPx);
  }
    // Map TOF distanceFiltered to mousePxFiltered (increase distance = increase mousePxRaw); if not invertOutput and x axis, or if inverted and y axis
  else {
    mousePxFiltered = map(constrain(distanceFiltered, rangeMin, rangeMax), rangeMin, rangeMax, mouseMinPx, mouseMaxPx);
  }
  return mousePxFiltered;
}

  // Move the mouse
  // Disable mouse acceleration on the connected computer for pixel to pixel movement, acceleration makes it non linear and unrepeatable (mouse ballistics)
void mouseMove() {
  mousePxChange = mousePxFiltered - mousePxLast;
  if (mousePxChange != 0) {
    while (abs(mousePxChange) > 127) {
      if (mousePxChange > 127) {
        if (mouseAxis == 0) {   //move X axis
          Mouse.move (127, 0); //Mouse.move(int8_t x, int8_t y, int8_t wheel, int8_t pan);
        }
        else if (mouseAxis == 1) {
          Mouse.move (0, 127);
        }
        mousePxLast += 127;
        mousePxChange = mousePxFiltered - mousePxLast;
      }
      else if (mousePxChange < -127) {
        if (mousePxChange < -127) {
          if (mouseAxis == 0) {
            Mouse.move (-127, 0);
          }
          else if (mouseAxis == 1) {
            Mouse.move (0, -127);
          }
          mousePxLast += -127;
          mousePxChange = mousePxFiltered - mousePxLast;
        }
      }
    }
    if (mousePxChange != 0) {
      if (mouseAxis == 0) {
        Mouse.move (mousePxChange, 0);
      }
      else if (mouseAxis == 1) {
        Mouse.move (0, mousePxChange);
      }
      mousePxLast += mousePxChange;
    }
  }
}

  // Main function for mouse hack, call to run whole mouse hack mode
void mouseHackFn() {
  checkDistance();
  if (newDistanceFlag) {
    filterDistance();
    mouseProcessPxFiltered();
    if (!mousePaused) {mouseMove();}
  }
}

//////////////////////////////////////////
//////     Mouse Hack Var FNS      ///////
//////////////////////////////////////////
  // Toggle Mouse Grab
void mouseToggleGrab() { 
  if (mouseGrab) {
    mouseGrab = 0;
    Mouse.release(MOUSE_LEFT);  //Default MOUSE_LEFT, MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE
    return;
  }
  else {
    mouseGrab = 1;
    Mouse.press(MOUSE_LEFT);
    return;
  }
}

  // Toggle Mouse Paused
void mouseTogglePaused() {
  if (mousePaused) {
    mousePaused = 0;
  }
  else {
    mousePaused = 1;
  }
}

  // Toggle Invert Output
void toggleInvertOutput() {
  if (invertOutput) {
    invertOutput = 0;
  }
  else {
    invertOutput = 1;
  }
}

  // Inc Mouse Axis 
void mouseIncAxis() {
  switch (mouseAxis) {
    case 0:
      mouseAxis = 1;
      return;
    case 1:
      mouseAxis = 0;
      return;
  }
}

  // Increment mouse max output by 5 px
void mouseIncMaxPx() {
  if (mouseMaxPx < mouseMaxPxLimit) {
    mouseMaxPx += 5;
  }
  else {
    mouseMaxPx = 5;
  }
}

//////////////////////////////////////////
//////       Auto Range Fn         ///////
//////////////////////////////////////////
  // Set valid sensing range
void autoRange() {
  rangeMax = 0;
  rangeMin = 0;
  strokeLength = 0;
  tft.fillScreen(backgroundColor);
  tft.drawString((String) "Auto Ranging...", 0, 0);
  tft.drawString((String) "Distance: " + distance + "        ", 0, fontHeight);
  tft.drawString((String) "Min Range: " + rangeMin + "        ", 0, fontHeight*2);
  tft.drawString((String) "Max Range: " + rangeMax + "        ", 0, fontHeight*3);
  tft.drawString((String) "Stroke Length: " + strokeLength + "        ", 0, fontHeight*4);
  unsigned long endTime = millis() + autoRangeTime;
  do {
    checkDistance();
    if (newDistanceFlag){
      rangeMin = distance;
      rangeMax = distance;
    }
  } while (!newDistanceFlag);
  do {
    checkDistance();
    if (newDistanceFlag) {
      if (distance > rangeMax) {
        rangeMax = distance;
        strokeLength = rangeMax - rangeMin;
      }
      else if (distance < rangeMin) {
        rangeMin = distance;
        strokeLength = rangeMax - rangeMin;
      }
    tft.drawString((String) "Distance: " + distance + "        ", 0, fontHeight);
    tft.drawString((String) "Min Range: " + rangeMin + "        ", 0, fontHeight*2);
    tft.drawString((String) "Max Range: " + rangeMax + "        ", 0, fontHeight*3);
    tft.drawString((String) "Stroke Length: " + strokeLength + "        ", 0, fontHeight*4);
    }
  } while (millis() < endTime);
  tft.fillScreen(backgroundColor);
}
