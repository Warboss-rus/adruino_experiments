#include <LiquidCrystal_I2C.h>
#include <sevendisplaylib.h>

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
  struct Gpu
  {
    unsigned char load;
    unsigned char temp;
    unsigned int freq;
    unsigned int memFreq;
  } gpu;
};
HardwareInfoData info = {};
const unsigned int HardwareInfoDataSize = 14;
const byte startByte = 0x3C;
bool startByteRead = false;
bool dataRead = false;

void setup() {
  lcd.init();                     
  lcd.backlight();
  lcd.print("Waiting for data");
  for (int i = 1; i <= 8; ++i) 
  {
    pinMode(i, OUTPUT);
  }
  for (int i = 10; i <= 13; ++i)
  {
    pinMode(i, OUTPUT);
  }
  Serial.begin(9600);
}

void fillUpString(char* str) 
{
  int len = strlen(str);
  for (int i = len; i < 16; ++i)
  {
    str[i] = ' ';
  }
  str[16] = '\0';
}

void loop() {
  // wait for start byte
  if (!startByteRead && Serial.available())
  {
    const byte data = Serial.read();
    if (data == startByte)
    {
      startByteRead = true;
    }
  }
  
  if (startByteRead && (Serial.available() >= HardwareInfoDataSize))
  {
    // read data
    unsigned char* data = (unsigned char*)&info;
    for (int i = 0; i < HardwareInfoDataSize; ++i)
    {
      data[i] = Serial.read();
    }
    startByteRead = false;
  
    char strBuffer[17];
    lcd.setCursor(0, 0);
    sprintf(strBuffer, "CPU%u%%%uC%uMhz", info.cpu.load, info.cpu.temp, info.cpu.freq);
    fillUpString(strBuffer);
    lcd.print(strBuffer);
    
    lcd.setCursor(0, 1);
    sprintf(strBuffer, "Mem%u/%uMB", info.mem.usedMb, info.mem.totalMb);
    fillUpString(strBuffer);
    lcd.print(strBuffer);
  }

  showNumber(info.mem.usedMb);
}
