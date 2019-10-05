#include <ArduinoOTA.h>
#include "Display.h"

void setupOTA()
{
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
    Display::setString(0, "OTA start: " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
    Display::setString(0, "OTA end");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    Display::setString(0, "OTA progress: " + String((progress / (total / 100))));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
      Display::setString(0, "OTA Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
      Display::setString(0, "OTA Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
      Display::setString(0, "OTA Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
      Display::setString(0, "OTA Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
      Display::setString(0, "OTA End Failed");
    }
  });
  ArduinoOTA.begin();
}

void loopOTA()
{
  ArduinoOTA.handle();
}
