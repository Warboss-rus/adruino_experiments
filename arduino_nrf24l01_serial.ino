#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "12345";

struct MeteoState
{
  float temperature;
  float humidity;
  unsigned long pressure;
};
#define METEO_STATE_DATA_SIZE 12

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (radio.available()) {
    MeteoState state;
    radio.read(&state, METEO_STATE_DATA_SIZE);
    const byte* data = (const byte*)&state;
    for (unsigned int i = 0; i < METEO_STATE_DATA_SIZE; ++i)
    {
      Serial.write(data[i]);
    }
  }
}
