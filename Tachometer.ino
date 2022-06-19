#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>

/*
 * Tachometer
 * Zachary D. Skelton
 * 6/17/2022
 */

// Pins (A4/A5 for SDA/SCL)
#define IR 2
#define BUTTON_RED 8
#define BUTTON_GRN 7
#define LED 13

// Display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET 4
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Globals
int objects = 1;
int state = 0;
bool started = false;

// Button States
#define NONE 0
#define B_RED 1
#define B_GRN 2
#define BOTH 3

// Application States
#define STATE_LOAD 0
#define STATE_MENU 1
#define STATE_MEASURE 2

// Display Text
void printText(String msg) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.cp437(true);
  display.println(msg);
  display.display();
}

// Capture and Calculate RPM
void measureRPM() {
  // Local Variables
  float _timer = 0;
  float _rpm = 0;
  int _count = 0;
  bool _onhit = false;
  
  // Get Samples
  _timer = micros();
  while(((micros() - _timer)/1000000.0) < 1.0) {
    // Count Hit
    if(digitalRead(IR) == LOW && !_onhit) {
      _count++;
      _onhit = true;
    }
    // Shut off hit indicator
    if(digitalRead(IR) == HIGH && _onhit) {
      _onhit = false;
    }
  }

  // Calculate Elapsed Time
  _rpm = ((_count * 60) / objects);
  String message = String("RPM: " + String(_rpm));
  printText(message);

  // Debug Data
  Serial.print("Count: ");
  Serial.print(_count);
  Serial.print(" Hits.\n");

  Serial.print("RPM: ");
  Serial.print(_rpm);
  Serial.print("\n");
}

int getButtons() {
  // Variables
  int _key = NONE;
  bool _red = digitalRead(BUTTON_RED) == HIGH ? true : false;
  bool _grn = digitalRead(BUTTON_GRN) == HIGH ? true : false;

  // Conditions
  if (!_red && !_grn) {
    _key = NONE;
  } else if (_red && !_grn) {
    _key = B_RED;
  } else if (!_red && _grn) {
    _key = B_GRN;
  } else if (_red && _grn) {
    _key = BOTH;
  }
  
  // Return
  return _key;
}

// Initialize Device
void setup() {  
  // Start Serial
  Serial.begin(9600);

  // Start Screen
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed."));
    for(;;); // Loop Forever
  }

  // Set Pin Mode
  pinMode(IR, INPUT);
  pinMode(BUTTON_RED, INPUT);
  pinMode(BUTTON_GRN, INPUT);
  pinMode(LED, OUTPUT);

  // Set up Devices
  display.display();

  // Set State
  state = STATE_MENU;

  // Clear the Buffer
  display.clearDisplay();
}

// Main Loop
void loop() {
  // Handle Button Pushes
  int _butts = getButtons();
  switch (_butts) {
    case NONE:
      break;
    case B_GRN:
      if (state == STATE_MENU) {
        objects++;
      }
      break;
    case B_RED:
      if (state == STATE_MENU) {
        if (objects > 1) {
          objects--;
        }
      }
      break;
    case BOTH:
      if (state == STATE_MEASURE) {
        state = STATE_MENU;
        digitalWrite(LED, LOW);
      } else {
        state = STATE_MEASURE;
        digitalWrite(LED, HIGH);
      }
      break;
  }

  // Display Based on State
  if (state == STATE_MEASURE) {
    measureRPM();
  } else {
    String message = String("Objects: " + String(objects));
    printText(message);
  }
  
  // Wait
  delay(100);
}
