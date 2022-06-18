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

// 
void updateDisplay(bool hit) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.cp437(true);
  if (hit) {
    display.println("Object!");
  } else {
    display.println("...");
  }
  display.display();
}

// the setup function runs once when you press reset or power the board
void setup() {  
  // Start Serial
  Serial.begin(9600);

  // Start Screen
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed."));
    for(;;); // Loop Forever
  }

  // Set up Devices
  display.display();
  delay(200);
  
  // Set Pin Mode
  pinMode(IR, INPUT);
  pinMode(LED, OUTPUT);

  // Clear the Buffer
  display.clearDisplay();
}

// the loop function runs over and over again forever
void loop() {
  if (digitalRead(IR) == LOW) {
    digitalWrite(LED, HIGH);
    updateDisplay(true);
  } else {
    digitalWrite(LED, LOW);
    updateDisplay(false);
  }

  // Wait
  delay(100);
}
