// To enable humidity sensor define SI7021_ADAFRUIT or SI7021_SPARKFUN
// To enable pressure sensor define BMP180_ADAFRUIT or BMP180_SODAQ

#define SI7021_SPARKFUN

#ifdef SI7021_ADAFRUIT
#include <Adafruit_Sensor.h>
#include <Adafruit_Si7021.h>
Adafruit_Si7021 sensor;
#elif defined(SI7021_SPARKFUN)
#include "SparkFun_Si7021_Breakout_Library.h"
Weather sensor;
#endif
#ifdef BMP180_ADAFRUIT
#include <Adafruit_BMP085_U.h>
Adafruit_BMP085_Unified  bmp;
#elif defined(BMP180_SODAQ)
#include <Sodaq_BMP085.h>
Sodaq_BMP085 bmp;
#elif defined(BMP280_ADAFRUIT)
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp;
#endif
#include <OneWire.h>
#include <RF24.h>

OneWire ds(PB0);
RF24           radio(PB1, PB2);//CE, CS
const byte address[6] = "12345";

struct MeteoState
{
  float temperature;
  float humidity;
  unsigned long pressure;
};
#define METEO_STATE_DATA_SIZE 12

float readTempDallas()
{
  byte data[2];

  ds.reset();
  ds.write(0xCC);
  ds.write(0x44);

  ds.reset();
  ds.write(0xCC);
  ds.write(0xBE);

  data[0] = ds.read();
  data[1] = ds.read();

  return ((data[1] << 8) | data[0]) * 0.0625;
}

void setup() {
  // put your setup code here, to run once:
#if defined(BMP180_ADAFRUIT) || defined(BMP180_SODAQ) || defined(BMP280_ADAFRUIT)
  bmp.begin();
#endif
#if defined(SI7021_ADAFRUIT) || defined(SI7021_SPARKFUN)
  sensor.begin();
#endif
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
  radio.openWritingPipe(address);
  radio.stopListening();
  Serial.begin(9600);
}

void loop() {
  Serial.print("Hello, World!");
  // put your main code here, to run repeatedly:
  delay(1000); // let the sensor pick up data
  MeteoState state = {};
  state.temperature = readTempDallas();
#ifdef SI7021_ADAFRUIT
  state.humidity = sensor.readHumidity();
#elif defined(SI7021_SPARKFUN)
  state.humidity = sensor.getRH();
#endif
#ifdef BMP180_ADAFRUIT
  float P = 0;
  bmp.getPressure(&P);
  state.pressure = P;
#elif defined(BMP180_SODAQ) || defined(BMP280_ADAFRUIT)
  state.pressure = bmp.readPressure();
#endif
  radio.write(&state, METEO_STATE_DATA_SIZE);
}
