#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  lcd.init();                     
  lcd.backlight();// Включаем подсветку дисплея
  lcd.print("Hello, World!");
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()) 
  {
    String str = Serial.readString();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(str);
    if (str.length() > 16)
    {
      String str2 = str.substring(16);
      lcd.setCursor(0, 1);
      lcd.print(str2);
    }
  }
}
