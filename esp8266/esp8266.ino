#ifdef ESP32
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#endif
#include <WebSocketsServer.h>
#include "MeteoStorage.h"
#include "HTMLProvider.h"
#include "WiFiCredentials.h"
#include "OTA.h"
#include "NTP.h"

#define SERVER_PORT 80
#define WEBSOCKET_PORT 9000
#define METEO_STATE_DATA_SIZE 12

#ifdef ESP32
WebServer server(SERVER_PORT);
#else
ESP8266WebServer server(SERVER_PORT);
#endif
WebSocketsServer webSocket(WEBSOCKET_PORT);
MeteoStorage storage;
NTPTime ntp;

#include <nRF24L01.h>
#include <RF24.h>
#ifdef ESP32
RF24 radio(17, 5); // CE, CSN
#else
RF24 radio(5, 4); // CE, CSN
#endif
const byte address[6] = "12345";

void readDataFromWireless()
{
  while (radio.available())
  {
    MeteoState currentState = {};
    radio.read(&currentState, METEO_STATE_DATA_SIZE);
    currentState.timestamp = ntp.getTimeStamp();
    storage.PushNewState(currentState);
    String stateText = stateToJSON(currentState);
    webSocket.broadcastTXT(stateText);
    Serial.print("T=");
    Serial.print( currentState.temperature);
    Serial.print("C P=");
    Serial.print( currentState.pressure);
    Serial.print("Pa H=");
    Serial.print(currentState.humidity);
    Serial.println("%");
  }
}

void printHTML()
{
  MeteoState currentState = storage.GetCurrentState();
  String htmlContent = formatHTML(currentState);
  server.send(200, "text/html", htmlContent);
}

void printJSON()
{
  String historyData = historyJSON(storage);
  server.send(200, "application/json", historyData);
}

void printNotFound() {
  String message = "URL Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  ntp.init();

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  if (!MDNS.begin("esp8266")) {
    Serial.println("MDNS responder failed");
  }
  MDNS.addService("http", "tcp", 80);

  setupOTA();

  server.on("/", printHTML);
  server.on("/history", printJSON);
  server.onNotFound(printNotFound);
  server.begin();

  webSocket.begin();

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop()
{
  readDataFromWireless();

  server.handleClient();
  webSocket.loop();
#ifndef ESP32
  MDNS.update();
#endif
  loopOTA();
}
