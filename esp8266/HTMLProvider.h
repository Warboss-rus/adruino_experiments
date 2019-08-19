static const char HTML_SCRIPT[] PROGMEM = "var meteoHistory=[],displaySettings={showTemp:!0,showPressure:!0,showHumidity:!0,end:null,size:null,mousePosX:null,mousePosY:null};function drawGraph(){var a=displaySettings.end?displaySettings.end:new Date,b=displaySettings.size?new Date(a-displaySettings.size):0<meteoHistory.length?new Date(1e3*meteoHistory[0].time):new Date,c=document.getElementById('historyGraph');c.hidden=!1;var d=c.getContext('2d');d.fillStyle='black',d.clearRect(0,0,c.width,c.height),d.textAlign='left',d.textBaseline='top',d.fillText('50\\xB0C',0,0),d.textBaseline='middle',d.fillText('0\\xB0C',0,c.height/2),d.textBaseline='bottom',d.fillText('-50\\xB0C',0,c.height),d.fillText(b.toLocaleString(),d.measureText('-50\\xB0C').width+10,c.height),d.textAlign='right',d.textBaseline='top',d.fillText('2000 hPa',c.width,0),d.textBaseline='middle',d.fillText('1000 hPa',c.width,c.height/2),d.textBaseline='bottom',d.fillText('0 hPa',c.width,c.height),d.fillText(a.toLocaleString(),c.width-d.measureText('0 hPa').width-10,c.height),d.strokeStyle='red',drawGraphLine(c,d,'temp',-50,50,b,a),d.strokeStyle='blue',drawGraphLine(c,d,'pres',0,2e5,b,a),d.strokeStyle='green',drawGraphLine(c,d,'hum',0,100,b,a),displaySettings.mousePosX&&displaySettings.mousePosY&&0<meteoHistory.length&&drawTooltip(c,d,b,a)}function drawGraphLine(a,b,c,d,e,f,g){b.beginPath();for(var h=0;h<meteoHistory.length;h++){var j=(new Date(1e3*meteoHistory[h].time)-f)*a.width/(g-f),k=a.height-(meteoHistory[h][c]-d)*a.height/(e-d);0==h?b.moveTo(j,k):b.lineTo(j,k)}b.stroke()}function drawTooltip(a,b,c,d){for(var e,f=meteoHistory[0],g=1e7,h=0;h<meteoHistory.length;h++)e=(new Date(1e3*meteoHistory[h].time)-c)*a.width/(d-c),Math.abs(displaySettings.mousePosX-e)<g&&(f=meteoHistory[h],g=Math.abs(displaySettings.mousePosX-e));var j=1.5*b.measureText('M').width,k=b.measureText('M').width,l=new Date(1e3*f.time).toLocaleString(),m='T: '+f.temp+'\\xB0C',n='P: '+f.pres+' Pa ('+Math.round(100*(f.pres/133.3224))/100+' mmHg)',o='H: '+f.hum+'%',p=Math.max(b.measureText(l).width,b.measureText(m).width,b.measureText(n).width,b.measureText(o).width)+2*k,q=5*j,e=displaySettings.mousePosX+p+k>a.width?displaySettings.mousePosX-p-k:displaySettings.mousePosX+k,r=displaySettings.mousePosY+q+k>a.height?displaySettings.mousePosY-q-k:displaySettings.mousePosY+k;b.fillStyle='#40404080',b.fillRect(e,r,p,q),b.textAlign='left',b.textBaseline='top',b.fillStyle='black',e+=k,r+=j/2,b.fillText(l,e,r),r+=j,b.fillText(m,e,r),r+=j,b.fillText(n,e,r),r+=j,b.fillText(o,e,r)}function onMouseMove(a){a.preventDefault(),a.stopPropagation();var b=document.getElementById('historyGraph'),c=b.getBoundingClientRect();displaySettings.mousePosX=a.clientX-c.left,displaySettings.mousePosY=a.clientY-c.top,drawGraph()}function onMouseLeave(a){a.preventDefault(),a.stopPropagation(),displaySettings.mousePosX=null,displaySettings.mousePosY=null,drawGraph()}function onMouseWheel(a){if(size=displaySettings.size,size||(size=0<meteoHistory.length?1e3*(meteoHistory[meteoHistory.length-1].time-meteoHistory[0].time):null),size){const b=1.5;displaySettings.size=0>a.deltaY?size/b:size*b}onMouseMove(a);var b=document.getElementById('historyScale');b.selectedIndex=0}function changeScale(a){displaySettings.size='-1'==a.value?null:a.value,drawGraph()}function receiveWebSocketMessage(a){data=JSON.parse(a.data),document.getElementById('temp').innerText=data.temp,document.getElementById('pressure').innerText=Math.round(100*(data.pres/133.3224))/100,document.getElementById('pressurePa').innerText=data.pres,document.getElementById('humidity').innerText=data.hum,document.getElementById('timestamp').innerText=new Date(1e3*data.time).toLocaleString(),meteoHistory.push(data),drawGraph()}function initCanvas(){var a=document.getElementById('historyGraph');a.onmousemove=onMouseMove,a.onmouseleave=onMouseLeave,a.onmousewheel=onMouseWheel}function sendHistoryRequest(){var a=new XMLHttpRequest;a.open('GET','/history',!0),a.onreadystatechange=function(){4==a.readyState&&200==a.status&&(historyData=JSON.parse(a.responseText),meteoHistory=Array.from(historyData.records),drawGraph())},a.send(null)}document.addEventListener('DOMContentLoaded',initCanvas),sendHistoryRequest();var socket=new WebSocket('ws://'+window.location.hostname+':9000');socket.onmessage=receiveWebSocketMessage;";

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
  htmlContent += "</span>%<br>Timestamp: <span id='timestamp'></span></p><br>Display scale: <select id='historyScale' onchange='changeScale(this)'><option value='' disabled hidden>Custom</option><option value='-1' selected>Everything</option><option value='3600000'>Last Hour</option><option value='86400000'>Last Day</option><option value='2592000000'>Last Month</option><option value='31536000000'>Last Year</option></select><br><canvas id=\"historyGraph\" width=\"1000px\" height=\"400px\" style=\"border: 1px solid #000000;\" hidden></canvas></body></html>";
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
