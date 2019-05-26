static const char HTML_SCRIPT[] PROGMEM = "var meteoHistory=[];function drawGraph(){var e=document.getElementById('historyGraph');e.hidden=!1;var t=e.getContext('2d');t.clearRect(0,0,e.width,e.height),t.strokeStyle='grey',t.beginPath(),t.moveTo(0,e.height/2),t.lineTo(e.width,e.height/2),t.stroke(),t.fillText('50\\xB0C',0,10),t.fillText('0\\xB0C',0,e.height/2),t.fillText(' -50\\xB0C',0,e.height-3),t.fillText('2000 hPa',e.width-45,10),t.fillText('1000 hPa',e.width-45,e.height/2),t.fillText('0 hPa',e.width-30,e.height-3),t.strokeStyle='red',t.beginPath(),meteoHistory.length>0&&t.moveTo(0,e.height/2-4*meteoHistory[0].temp);for(var o=1;o<meteoHistory.length;o++)t.lineTo(e.width*o/(meteoHistory.length-1),e.height/2-4*meteoHistory[o].temp);t.stroke(),t.strokeStyle='blue',t.beginPath(),meteoHistory.length>0&&t.moveTo(0,e.height-meteoHistory[0].pres/500);for(o=1;o<meteoHistory.length;o++)t.lineTo(e.width*o/(meteoHistory.length-1),e.height-meteoHistory[o].pres/500);t.stroke()}var xobj=new XMLHttpRequest;xobj.open('GET','/history',!0),xobj.onreadystatechange=function(){4==xobj.readyState&&200==xobj.status&&(historyData=JSON.parse(xobj.responseText),meteoHistory=Array.from(historyData.records),drawGraph())},xobj.send(null);var socket=new WebSocket('ws://'+window.location.hostname+':9000');socket.onmessage=function(e){data=JSON.parse(e.data),document.getElementById('temp').innerText=data.temp,document.getElementById('pressure').innerText=Math.round(data.pres/133.3224*100)/100,document.getElementById('pressurePa').innerText=data.pres,document.getElementById('humidity').innerText=data.hum,document.getElementById('timestamp').innerText=new Date(1e3*data.time).toLocaleString(),meteoHistory.push(data),drawGraph()};";

String formatHTML(const MeteoState& currentState)
{
  float pressureMmHg = currentState.pressure / 133.3224f;
  String htmlContent = "<!DOCTYPE html><html><link rel=\"icon\" href=\"data:,\"><script>";
  htmlContent += FPSTR(HTML_SCRIPT);
  htmlContent += "</script><body><h1>ESP8266 Temp monitor</h1><p>Current temperature: <span id='temp'>";
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
