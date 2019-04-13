#include "Meteo.h"
#include <OneWire.h>
#include <Adafruit_BMP085_U.h>
#include <nRF24L01.h>
#include <RF24.h>

OneWire ds(PA3);
Adafruit_BMP085_Unified  bmp;
RF24 radio(PB11, PA4); // CE, CSN

unsigned long lastTempUpdateTime = 0;
const byte address[6] = "12345";

struct MeteoState
{
  float temperature;
  float humidity;
  unsigned int pressure;
};
#define METEO_STATE_DATA_SIZE 12

void setupMeteo()
{
  bmp.begin();
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void updateMeteo()
{
  byte data[2];
  ds.reset();
  ds.write(0xCC);
  ds.write(0x44);

  if (millis() - lastTempUpdateTime > 1000)
  {
    lastTempUpdateTime = millis();
    ds.reset();
    ds.write(0xCC);
    ds.write(0xBE);

    data[0] = ds.read();
    data[1] = ds.read();

    MeteoState state = {0.0f, 0.0f, 0};
    state.temperature = ((data[1] << 8) | data[0]) * 0.0625;
    float P = 0;
    bmp.getPressure(&P);
    state.pressure = (unsigned int)P;
    radio.write(&state, METEO_STATE_DATA_SIZE);
  }
}
