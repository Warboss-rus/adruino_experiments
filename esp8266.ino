#include <ESP8266WiFi.h>
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

void printHTMLScript(WiFiClient& client)
{
  client.println("<script>");
  client.println("function drawGraph(history) {");
  client.println("  const canvas = document.getElementById(\"historyGraph\");");
  client.println("  canvas.hidden = false;");
  client.println("  const ctx = canvas.getContext(\"2d\");");
  client.println("  ctx.strokeStyle = \"grey\";");
  client.println("  ctx.beginPath();");
  client.println("  ctx.moveTo(0, 200);");
  client.println("  ctx.lineTo(1000, 200);");
  client.println("  ctx.stroke();");
  client.println("  ctx.fillText(\"50°C\", 0, 10);");
  client.println("  ctx.fillText(\"0°C\", 0, 200);");
  client.println("  ctx.fillText(\" - 50°C\", 0, 397);");
  client.println("  ctx.strokeStyle = \"red\";");
  client.println("  ctx.beginPath();");
  client.println("  if (history.length > 0) {");
  client.println("    ctx.moveTo(0, 200 - history[0] * 4);");
  client.println("  }");
  client.println("  for (var i = 1; i < history.length; i++) {");
  client.println("    ctx.lineTo(1000 * i / (history.length - 1), 200 - history[i].temp * 4);");
  client.println("  }");
  client.println("  ctx.stroke();");
  client.println("}");
  client.println("var xobj = new XMLHttpRequest();");
  client.println("xobj.open('GET', '/history', true);");
  client.println("xobj.onreadystatechange = function () {");
  client.println("  if (xobj.readyState == 4 && xobj.status == \"200\") {");
  client.println("    historyData = JSON.parse(xobj.responseText);");
  client.println("    drawGraph(historyData.records);");
  client.println("  }");
  client.println("};");
  client.println("xobj.send(null);");
  client.println("</script>");
}

void printHTML(WiFiClient& client)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();

  client.println("<!DOCTYPE html><html>");
  client.println("<link rel=\"icon\" href=\"data:,\">");

  printHTMLScript(client);

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
}
