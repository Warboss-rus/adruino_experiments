static const char HTML_SCRIPT[] PROGMEM = "<script>"
    "var meteoHistory = [];"
    "function drawGraph() {"
    "  const canvas = document.getElementById(\"historyGraph\");"
    "  canvas.hidden = false;"
    "  const ctx = canvas.getContext(\"2d\");"
    "  ctx.clearRect(0, 0, canvas.width, canvas.height);"
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
    "  if (meteoHistory.length > 0) {"
    "    ctx.moveTo(0, 200 - meteoHistory[0].temp * 4);"
    "  }"
    "  for (var i = 1; i < meteoHistory.length; i++) {"
    "    ctx.lineTo(1000 * i / (meteoHistory.length - 1), 200 - meteoHistory[i].temp * 4);"
    "  }"
    "  ctx.stroke();"
    "  ctx.strokeStyle = \"blue\";"
    "  ctx.beginPath();"
    "  if (meteoHistory.length > 0) {"
    "    ctx.moveTo(0, 400 - meteoHistory[0].pres / 500);"
    "  }"
    "  for (var i = 1; i < meteoHistory.length; i++) {"
    "    ctx.lineTo(1000 * i / (meteoHistory.length - 1), 400 - meteoHistory[i].pres / 500);"
    "  }"
    "  ctx.stroke();"
    "}"
    "var xobj = new XMLHttpRequest();"
    "xobj.open('GET', '/history', true);"
    "xobj.onreadystatechange = function () {"
    "  if (xobj.readyState == 4 && xobj.status == \"200\") {"
    "    historyData = JSON.parse(xobj.responseText);"
    "    meteoHistory = Array.from(historyData.records);"
    "    drawGraph();"
    "  }"
    "};"
    "xobj.send(null);"
    "var socket = new WebSocket(\"ws://\" + window.location.hostname + \":9000\");"
    "socket.onmessage = function(event) {"
    "  data = JSON.parse(event.data);"
    "  document.getElementById('temp').innerText = data.temp;"
    "  document.getElementById('pressure').innerText = Math.round(data.pres / 133.3224 * 100) / 100;"
    "  document.getElementById('pressurePa').innerText = data.pres;"
    "  document.getElementById('humidity').innerText = data.hum;"
    "  document.getElementById('timestamp').innerText = new Date(data.time * 1000).toLocaleString();"
    "  drawGraph();"
    "}"
    "</script>";

String formatHTML(const MeteoState& currentState)
{
  float pressureMmHg = currentState.pressure / 133.3224f;
  String htmlContent = "<!DOCTYPE html><html><link rel=\"icon\" href=\"data:,\">";
  htmlContent += FPSTR(HTML_SCRIPT);
  htmlContent += "<body><h1>ESP8266 Temp monitor</h1><p>Current temperature: <span id='temp'>";
  htmlContent += String(currentState.temperature, 2);
  htmlContent += "</span>&degC<br>Current pressure: <span id='pressure'>";
  htmlContent += String(pressureMmHg);
  htmlContent += "</span> mmHg (<span id='pressurePa'>";
  htmlContent += String(currentState.pressure);
  htmlContent += "</span> Pa)<br>Current humidity: <span id='humidity'>";
  htmlContent += String(currentState.humidity, 2);
  htmlContent += "</span>%<br>Timestamp: <span id='timestamp'>";
  htmlContent += "</span></p><br><canvas id=\"historyGraph\" width=\"1000px\" height=\"400px\" style=\"border: 1px solid #000000;\" hidden></canvas></body></html>";
  return htmlContent;
}

String stateToJSON(const MeteoState& state)
{
  String result = "{\"temp\":";
  result += String(state.temperature, 2);
  result += ",\"pres\":";
  result += String(state.pressure);
  result += ",\"hum\":";
  result += String(state.humidity, 2);
  result += ",\"time\":";
  result += String(state.timestamp);
  result += "}";
  return result;
}

String historyJSON(const MeteoStorage& storage)
{
  String historyData = "{\"records\":[";
  size_t historyCount = storage.size();
  for (size_t i = 0; i < historyCount; ++i)
  {
    if (i != 0)
    {
      historyData += ',';
    }
    historyData += stateToJSON(storage[i]);
  }
  historyData += "]}";
  return historyData;
}
