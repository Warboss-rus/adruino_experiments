#include <nRF24L01.h>
#include <RF24.h>
#include <U8g2lib.h>

U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, 6, 3, 5);
RF24 radio(7, 8); // CE, CSN

const byte address[6] = "12345";

struct MeteoState
{
  float temperature;
  float humidity;
  uint32_t pressure;
} state = {};
#define METEO_STATE_DATA_SIZE 12

String lastMessage;
bool messageEnded = false;

void printState()
{
  String tempString = "Temp: ";
  tempString += String(state.temperature);
  tempString += "°C";
  String pressureString = "Pressure: ";
  pressureString += String(state.pressure);
  pressureString += " Pa";
  String humidityString = "Humidity: ";
  humidityString += state.humidity;
  humidityString += "%";
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(0, 11, tempString.c_str());
    u8g2.drawStr(0, 28, pressureString.c_str());
    u8g2.drawStr(0, 45, humidityString.c_str());
    u8g2.drawStr(0, 62, lastMessage.c_str());
    u8g2.sendBuffer();
  } while ( u8g2.nextPage() );
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  u8g2.begin();
  printState();
}

void loop()
{
  // put your main code here, to run repeatedly:
  bool needPrintState = false;
  while (Serial.available())
  {
    int symbol = Serial.read();
    switch (symbol)
    {
      case -1:
      case '\n':
      case '\r':
        messageEnded = true;
        break;
      default:
        if (messageEnded)
        {
          lastMessage = String();
          messageEnded = false;
        }
        lastMessage += (char)symbol;
    }
    needPrintState = true;
  }
  if (radio.available())
  {
    radio.read(&state, METEO_STATE_DATA_SIZE);
    const byte* data = (const byte*)&state;
    for (unsigned int i = 0; i < METEO_STATE_DATA_SIZE; ++i)
    {
      Serial.write(data[i]);
    }
    needPrintState = true;
  }
  if (needPrintState)
  {
    printState();
  }
}
