#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <sevendisplaylib.h>

LiquidCrystal_I2C lcd(0x27,16,2);
OneWire ds(A0);

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
float temperature = 0;
long lastTempUpdateTime = 0;

void setup() {
  lcd.init();                     
  lcd.backlight();
  lcd.print("Waiting for data");
  for (int i = 2; i <= 13; ++i)
  {
    pinMode(i, OUTPUT);
  }
  initSevenSegmentDisplay(10, 2);
  Serial.begin(9600);
  lastTempUpdateTime = millis();
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

unsigned int readTemp()
{
   // Определяем температуру от датчика DS18b20
  byte data[2]; // Место для значения температуры
  
  ds.reset(); // Начинаем взаимодействие со сброса всех предыдущих команд и параметров
  ds.write(0xCC); // Даем датчику DS18b20 команду пропустить поиск по адресу. В нашем случае только одно устрйоство 
  ds.write(0x44); // Даем датчику DS18b20 команду измерить температуру. Само значение температуры мы еще не получаем - датчик его положит во внутреннюю память

  if (millis() - lastTempUpdateTime > 1000)
  {
    lastTempUpdateTime = millis();
    ds.reset(); // Теперь готовимся получить значение измеренной температуры
    ds.write(0xCC); 
    ds.write(0xBE); // Просим передать нам значение регистров со значением температуры
   
    // Получаем и считываем ответ
    data[0] = ds.read(); // Читаем младший байт значения температуры
    data[1] = ds.read(); // А теперь старший
   
    // Формируем итоговое значение: 
    //    - сперва "склеиваем" значение, 
    //    - затем умножаем его на коэффициент, соответсвующий разрешающей способности (для 12 бит по умолчанию - это 0,0625)
    temperature =  ((data[1] << 8) | data[0]) * 0.0625;
  }
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
    showFloat(temperature);
    
    lcd.setCursor(0, 1);
    sprintf(strBuffer, "Mem%u/%uMB", info.mem.usedMb, info.mem.totalMb);
    fillUpString(strBuffer);
    lcd.print(strBuffer);
  }
  readTemp();
  showFloat(temperature);
}
