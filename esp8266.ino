#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#define SERVER_PORT 80
#define HISTORY_SIZE 100

struct MeteoState
{
  float temperature;
  unsigned int pressure;
  float humidity;
};

const char* ssid = "********";
const char* password = "********";

WiFiServer server(SERVER_PORT);
MeteoState currentState = {0.0f, 0, 0.0f};
MeteoState history[HISTORY_SIZE] = {};
unsigned int historyStart = 0;
unsigned int historyEnd = 0;

void readDataFromSerial()
{
  byte data[2];
  data[0] = Serial.read();
  data[1] = Serial.read();
  currentState.temperature = ((data[1] << 8) | data[0]);
  currentState.temperature /= 100;
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
                           "  ctx.fillText(\"50°C\", 0, 10);"
                           "  ctx.fillText(\"0°C\", 0, 200);"
                           "  ctx.fillText(\" - 50°C\", 0, 397);"
                           "  ctx.strokeStyle = \"red\";"
                           "  ctx.beginPath();"
                           "  if (history.length > 0) {"
                           "    ctx.moveTo(0, 200 - history[0] * 4);"
                           "  }"
                           "  for (var i = 1; i < history.length; i++) {"
                           "    ctx.lineTo(1000 * i / (history.length - 1), 200 - history[i].temp * 4);"
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

void printHTML(WiFiClient& client)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();

  client.println("<!DOCTYPE html><html>");
  client.println("<link rel=\"icon\" href=\"data:,\">");

  client.println(FPSTR(HTML_SCRIPT));

  client.println("<body><h1>ESP8266 Temp monitor</h1>");
  client.print("<p>Current temperature: ");
  client.print(currentState.temperature);
  client.println("°C<br>");
  client.print("Current pressure: ");
  client.print(currentState.pressure);
  client.println(" hPa<br>");
  client.print("Current humidity: ");
  client.print(currentState.humidity);
  client.println("%</p><br>");
  client.println("<canvas id=\"historyGraph\" width=\"1000px\" height=\"400px\" style=\"border: 1px solid #000000;\" hidden></canvas>");
  client.println("</body></html>");
  client.println();
}

void printJSON(WiFiClient& client)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:application/json");
  client.println("Connection: close");
  client.println();

  client.println("{\"records\"=[");
  for (unsigned int i = historyStart; i != historyEnd; i = (i + 1) % HISTORY_SIZE)
  {
    if (i != historyStart)
    {
      client.print(",");
    }
    client.print(stateToJSON(currentState));
  }
  client.println("]}");
  client.println();
}

void processWiFiClient(WiFiClient& client)
{
  String header;
  String currentLine;
  while (client.connected())
  {
    if (client.available())
    {
      char c = client.read();
      Serial.write(c);
      header += c;
      if (c == '\n')
      {
        if (currentLine.length() == 0)
        {
          if (header.indexOf("GET /history") >= 0)
          {
            printJSON(client);
          }
          else
          {
            printHTML(client);
          }
        }
        else
        {
          currentLine = "";
        }
      }
    }
  }
  delay(1);
  client.stop();
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

  setupOTA();

  server.begin();
}

void loop()
{
  while (Serial.available() >= 2)
  {
    readDataFromSerial();
  }

  if (WiFiClient client = server.available())
  {
    processWiFiClient(client);
  }

  ArduinoOTA.handle();
}
