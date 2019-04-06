#include <OneWireSTM.h>
#include<Wire.h>
#include <Adafruit_BMP085_U.h>
#include <nRF24L01.h>
#include <RF24.h>

Adafruit_BMP085_Unified  bmp;
OneWire  ds(PA3);
RF24 radio(PB0, PA4); // CE, CSN

unsigned int lastTempUpdateTime = 0;
const byte address[6] = "12345";

struct MeteoState
{
  float temperature;
  float humidity;
  unsigned int pressure;
};
#define METEO_STATE_DATA_SIZE 12

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  if(!bmp.begin())
  {
    Serial.println("BMP180 init failed");
  }
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
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
   
    float temperature =  ((data[1] << 8) | data[0]) * 0.0625;
    Serial.print("Temperature from DS18B20: ");
    Serial.print(temperature, 2);
    Serial.println("C");
    bmp.getTemperature(&temperature);
    Serial.print("Temperature from BMP180: ");
    Serial.print(temperature, 2);
    Serial.println("C");
    float P = 0;
    bmp.getPressure(&P);
    Serial.print("Pressure: ");
    Serial.print(P, 2);
    Serial.println("hPa");

    MeteoState state = {};
    state.temperature = temperature;
    state.pressure = (unsigned int)P;
    radio.write(&state, METEO_STATE_DATA_SIZE);
    delay(300);
  }
}
