/*
This is a source code for ESP8266 HTML page. After changing the code minify it and place into HTMLProvider.h file's HTML_SCRIPT constant
*/

var meteoHistory = [];
function drawGraph() {
    var canvas = document.getElementById('historyGraph');
    canvas.hidden = false;
    var ctx = canvas.getContext('2d');
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.strokeStyle = 'grey';
    ctx.beginPath();
    ctx.moveTo(0, canvas.height / 2);
    ctx.lineTo(canvas.width, canvas.height / 2);
    ctx.stroke();
    ctx.fillText('50\\xB0C', 0, 10);
    ctx.fillText('0\\xB0C', 0, canvas.height / 2);
    ctx.fillText(' -50\\xB0C', 0, canvas.height - 3);
    ctx.fillText('2000 hPa', canvas.width - 45, 10);
    ctx.fillText('1000 hPa', canvas.width - 45, canvas.height / 2);
    ctx.fillText('0 hPa', canvas.width - 30, canvas.height - 3);
    ctx.strokeStyle = 'red';
    ctx.beginPath();
    if (meteoHistory.length > 0) {
        ctx.moveTo(0, canvas.height / 2 - meteoHistory[0].temp * 4);
    }
    for (var i = 1; i < meteoHistory.length; i++) {
        ctx.lineTo(canvas.width * i / (meteoHistory.length - 1), canvas.height / 2 - meteoHistory[i].temp * 4);
    }
    ctx.stroke();
    ctx.strokeStyle = 'blue';
    ctx.beginPath();
    if (meteoHistory.length > 0) {
        ctx.moveTo(0, canvas.height - meteoHistory[0].pres / 500);
    }
    for (var i = 1; i < meteoHistory.length; i++) {
        ctx.lineTo(canvas.width * i / (meteoHistory.length - 1), canvas.height - meteoHistory[i].pres / 500);
    }
    ctx.stroke();
}

var xobj = new XMLHttpRequest();
xobj.open('GET', '/history', true);
xobj.onreadystatechange = function () {
    if (xobj.readyState == 4 && xobj.status == 200) {
        historyData = JSON.parse(xobj.responseText);
        meteoHistory = Array.from(historyData.records);
        drawGraph();
    }
};
xobj.send(null);

var socket = new WebSocket('ws://' + window.location.hostname + ':9000');
socket.onmessage = function (event) {
    data = JSON.parse(event.data);
    document.getElementById('temp').innerText = data.temp;
    document.getElementById('pressure').innerText = Math.round(data.pres / 133.3224 * 100) / 100;
    document.getElementById('pressurePa').innerText = data.pres;
    document.getElementById('humidity').innerText = data.hum;
    document.getElementById('timestamp').innerText = new Date(data.time * 1000).toLocaleString();
    meteoHistory.push(data);
    drawGraph();
}

