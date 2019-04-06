#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#define SERVER_PORT 80
#define HISTORY_SIZE 100

struct MeteoState
{
  float temperature;
  float humidity;
  unsigned int pressure;
};
#define METEO_STATE_DATA_SIZE 12

const char* ssid = "********";
const char* password = "********";

ESP8266WebServer server(SERVER_PORT);
MeteoState currentState = {0.0f, 0, 0.0f};
MeteoState history[HISTORY_SIZE] = {};
unsigned int historyStart = 0;
unsigned int historyEnd = 0;

void readDataFromSerial()
{
  byte* data = (byte*)&currentState;
  for (unsigned int i = 0; i < METEO_STATE_DATA_SIZE; ++i)
  {
    data[i] = Serial.read();
  }
  history[historyEnd] = currentState;
  historyEnd = (historyEnd + 1) % HISTORY_SIZE;
  if (historyStart == historyEnd)
  {
    historyStart = (historyStart + 1) % HISTORY_SIZE;
  }
}

String stateToJSON(const MeteoState& state)
{
  String result = "{\"temp\":";
  result += String(state.temperature, 2);
  result += ",\"pres\":";
  result += String(state.pressure);
  result += ",\"hum\":";
  result += String(state.humidity, 2);
  result += "}";
  return result;
}

static const char HTML_SCRIPT[] PROGMEM = "<script>"
    "function drawGraph(history) {"
    "  const canvas = document.getElementById(\"historyGraph\");"
    "  canvas.hidden = false;"
    "  const ctx = canvas.getContext(\"2d\");"
    "  ctx.strokeStyle = \"grey\";"
    "  ctx.beginPath();"
    "  ctx.moveTo(0, 200);"
    "  ctx.lineTo(1000, 200);"
    "  ctx.stroke();"
    "  ctx.fillText(\"50\\xB0C\", 0, 10);"
    "  ctx.fillText(\"0\\xB0C\", 0, 200);"
    "  ctx.fillText(\" -50\\xB0C\", 0, 397);"
    "  ctx.fillText(\"2000 hPa\", 955, 10);"
    "  ctx.fillText(\"1000 hPa\", 955, 200);"
    "  ctx.fillText(\"0 hPa\", 970, 397);"
    "  ctx.strokeStyle = \"red\";"
    "  ctx.beginPath();"
    "  if (history.length > 0) {"
    "    ctx.moveTo(0, 200 - history[0].temp * 4);"
    "  }"
    "  for (var i = 1; i < history.length; i++) {"
    "    ctx.lineTo(1000 * i / (history.length - 1), 200 - history[i].temp * 4);"
    "  }"
    "  ctx.stroke();"
    "  ctx.strokeStyle = \"blue\";"
    "  ctx.beginPath();"
    "  if (history.length > 0) {"
    "    ctx.moveTo(0, 400 - history[0].pres / 500);"
    "  }"
    "  for (var i = 1; i < history.length; i++) {"
    "    ctx.lineTo(1000 * i / (history.length - 1), 400 - history[i].pres / 500);"
    "  }"
    "  ctx.stroke();"
    "}"
    "var xobj = new XMLHttpRequest();"
    "xobj.open('GET', '/history', true);"
    "xobj.onreadystatechange = function () {"
    "  if (xobj.readyState == 4 && xobj.status == \"200\") {"
    "    historyData = JSON.parse(xobj.responseText);"
    "    drawGraph(historyData.records);"
    "  }"
    "};"
    "xobj.send(null);"
    "</script>";

void printHTML()
{
  float pressureMmHg = currentState.pressure / 133.3224f;
  String htmlContent = "<!DOCTYPE html><html><link rel=\"icon\" href=\"data:,\">";
  htmlContent += FPSTR(HTML_SCRIPT);
  htmlContent += "<body><h1>ESP8266 Temp monitor</h1><p>Current temperature: ";
  htmlContent += String(currentState.temperature, 2);
  htmlContent += "&degC<br>Current pressure: ";
  htmlContent += String(pressureMmHg);
  htmlContent += " mmHg (";
  htmlContent += String(currentState.pressure);
  htmlContent += " Pa)<br>Current humidity: ";
  htmlContent += String(currentState.humidity, 2);
  htmlContent += "%</p><br><canvas id=\"historyGraph\" width=\"1000px\" height=\"400px\" style=\"border: 1px solid #000000;\" hidden></canvas></body></html>";

  server.send(200, "text/html", htmlContent);
}

void printJSON()
{
  String historyData = "{\"records\":[";
  for (unsigned int i = historyStart; i != historyEnd; i = (i + 1) % HISTORY_SIZE)
  {
    if (i != historyStart)
    {
      historyData += ',';
    }
    historyData += stateToJSON(history[i]);
  }
  historyData += "]}";

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
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
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
}

void loop()
{
  while (Serial.available() >= METEO_STATE_DATA_SIZE)
  {
    readDataFromSerial();
  }

  server.handleClient();

  MDNS.update();

  ArduinoOTA.handle();
}
