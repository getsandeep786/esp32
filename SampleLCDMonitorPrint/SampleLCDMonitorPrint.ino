#include "LiquidCrystal_I2C.h"

//0x3F or 0x27
LiquidCrystal_I2C lcd(0x3F, 16, 2);  //LCD Object

void setup() {
  //put your setup code here, to run once
  lcd.init();
  lcd.backlight();
  lcd.print("Kunja I love you !!!");
}

void loop() {
  //put your main code here to run repeatedly
}