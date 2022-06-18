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

// Pins
#define IR 2
#define LED 13

// Display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET 4
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Globals
int objects = 1;

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
  pinMode(LED, OUTPUT);

  // Set up Devices
  display.display();

  // Clear the Buffer
  display.clearDisplay();
}

// Main Loop
void loop() {
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

  // Wait
  delay(100);
}
