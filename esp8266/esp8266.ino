#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>
#include "MeteoStorage.h"
#include "HTMLProvider.h"
#include "OTA.h"

#define SERVER_PORT 80
#define WEBSOCKET_PORT 9000
#define METEO_STATE_DATA_SIZE 12

const char* ssid = "********";
const char* password = "********";

ESP8266WebServer server(SERVER_PORT);
WebSocketsServer webSocket(WEBSOCKET_PORT);
MeteoStorage storage;

void readDataFromSerial()
{
  while (Serial.available() >= METEO_STATE_DATA_SIZE)
  {
    MeteoState currentState = {};
    byte* data = (byte*)&currentState;
    for (unsigned int i = 0; i < METEO_STATE_DATA_SIZE; ++i)
    {
      data[i] = Serial.read();
    }
    storage.PushNewState(currentState);
    String stateText = stateToJSON(currentState);
    webSocket.broadcastTXT(stateText);
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
  Serial.println();

  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  setupOTA();

  server.on("/", printHTML);
  server.on("/history", printJSON);
  server.onNotFound(printNotFound);
  server.begin();

  webSocket.begin();
}

void loop()
{
  readDataFromSerial();

  server.handleClient();
  webSocket.loop();

  MDNS.update();

  loopOTA();
}
