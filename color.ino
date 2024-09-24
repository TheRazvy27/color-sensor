#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define redpin 3
#define greenpin 5
#define bluepin 6
#define buttonPin 2 

#define commonAnode true

byte gammatable[256];

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

int redCount = 0;
int greenCount = 0;
int blueCount = 0;
String lastColor = "None";

void setup() {
  Serial.begin(9600);
  
  lcd.init();
  lcd.backlight();
  
  if (tcs.begin()) {
    Serial.println("Found TCS34725 sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }

  pinMode(redpin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
  
  pinMode(buttonPin, INPUT); 
}

void loop() {
  float red, green, blue;
  
  tcs.setInterrupt(false);  
  delay(60);  
  tcs.getRGB(&red, &green, &blue);
  tcs.setInterrupt(true);  

  Serial.print("R:\t"); Serial.print(int(red)); 
  Serial.print("\tG:\t"); Serial.print(int(green)); 
  Serial.print("\tB:\t"); Serial.print(int(blue));
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Color: ");

  String currentColor = "None";
  
  if (int(red) >= 120) {
    currentColor = "Red";
  } else if (int(blue) >= 120) {
    currentColor = "Blue";
  } else if (int(green) >= 120) {
    currentColor = "Green";
  }

  lcd.print(currentColor);

  if (digitalRead(buttonPin) == LOW) {
    redCount = 0;
    greenCount = 0;
    blueCount = 0;
    lastColor = "None";
  }

  if (currentColor != lastColor) {
    if (currentColor == "Red") {
      redCount++;
    } else if (currentColor == "Blue") {
      blueCount++;
    } else if (currentColor == "Green") {
      greenCount++;
    }
    lastColor = currentColor;
  }

  lcd.setCursor(0, 1);
  lcd.print("R:");
  lcd.print(redCount);
  lcd.print(" G:");
  lcd.print(greenCount);
  lcd.print(" B:");
  lcd.print(blueCount);

  Serial.print("\n");

  analogWrite(redpin, gammatable[(int)red]);
  analogWrite(greenpin, gammatable[(int)green]);
  analogWrite(bluepin, gammatable[(int)blue]);

  delay(1000); 
  }
