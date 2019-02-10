#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27,16,2);

struct HardwareInfoData
{
  struct Cpu
  {
    unsigned char load;
    unsigned char temp;
    unsigned int freq; // 2 bytes
  } cpu;
  struct Memory
  {
    unsigned int usedMb; // 2 bytes
    unsigned int totalMb; // 2 bytes
  } mem;
};
const unsigned int HardwareInfoDataSize = 8;
const byte startByte = 0x3C;

void setup() {
  lcd.init();                     
  lcd.backlight();
  lcd.print("Waiting for data");
  Serial.begin(9600);
}

void loop() {
  // wait for start byte
  while ((byte(Serial.read()) != startByte))
  {
    delay(1);
  }
  // wait until we get enough data
  while(Serial.available() < HardwareInfoDataSize)
  {
    delay(1);
  }
  // read data
  HardwareInfoData info = {};
  unsigned char* data = (unsigned char*)&info;
  for (int i = 0; i < HardwareInfoDataSize; ++i)
  {
    data[i] = Serial.read();
  }

  char strBuffer[16];
  lcd.setCursor(0, 0);
  sprintf(strBuffer, "CPU%u%%%uC%uMhz", info.cpu.load, info.cpu.temp, info.cpu.freq);
  lcd.clear();
  lcd.print(strBuffer);
  
  lcd.setCursor(0, 1);
  sprintf(strBuffer, "Mem%u/%uMB", info.mem.usedMb, info.mem.totalMb);
  lcd.print(strBuffer);
}
