/*
This is a source code for ESP8266 HTML page. After changing the code minify it and place into HTMLProvider.h file's HTML_SCRIPT constant
*/

var meteoHistory = [];
var displaySettings = {
    showTemp: true,
    showPressure: true,
    showHumidity: true,
    end: null,
    size: null,
    mousePosX: null,
    mousePosY: null,
};

function drawGraph() {
    var end = displaySettings.end ? displaySettings.end : new Date();
    var start = displaySettings.size ? new Date(end - displaySettings.size) : (meteoHistory.length > 0 ? new Date(meteoHistory[0].time * 1000) : new Date());
    var canvas = document.getElementById('historyGraph');
    canvas.hidden = false;
    var ctx = canvas.getContext('2d');
    ctx.fillStyle = 'black';
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.textAlign = 'left';
    ctx.textBaseline = 'top';
    ctx.fillText('50\\xB0C', 0, 0);
    ctx.textBaseline = 'middle';
    ctx.fillText('0\\xB0C', 0, canvas.height / 2);
    ctx.textBaseline = 'bottom';
    ctx.fillText('-50\\xB0C', 0, canvas.height);
    ctx.fillText(start.toLocaleString(), ctx.measureText('-50\\xB0C').width + 10, canvas.height);
    ctx.textAlign = 'right';
    ctx.textBaseline = 'top';
    ctx.fillText('2000 hPa', canvas.width, 0);
    ctx.textBaseline = 'middle';
    ctx.fillText('1000 hPa', canvas.width, canvas.height / 2);
    ctx.textBaseline = 'bottom';
    ctx.fillText('0 hPa', canvas.width, canvas.height);
    ctx.fillText(end.toLocaleString(), canvas.width - ctx.measureText('0 hPa').width - 10, canvas.height);
    ctx.strokeStyle = 'red';
    drawGraphLine(canvas, ctx, 'temp', -50, 50, start, end);
    ctx.strokeStyle = 'blue';
    drawGraphLine(canvas, ctx, 'pres', 0, 200000, start, end);
    ctx.strokeStyle = 'green';
    drawGraphLine(canvas, ctx, 'hum', 0, 100, start, end);
    if (displaySettings.mousePosX && displaySettings.mousePosY && meteoHistory.length > 0)
    {
        drawTooltip(canvas, ctx, start, end);
    }
}

function drawGraphLine(canvas, ctx, valueName, min, max, start, end)
{
    ctx.beginPath();
    for (var i = 0; i < meteoHistory.length; i++) {
        var x = (new Date(meteoHistory[i].time * 1000) - start) * canvas.width / (end - start);
        var y = canvas.height - (meteoHistory[i][valueName] - min) * canvas.height / (max - min);
        if (i === 0)
        {
            ctx.moveTo(x, y);
        }
        else
        {
            ctx.lineTo(x, y);
        }
    }
    ctx.stroke();
}

function drawTooltip(canvas, ctx, start, end) {
    var historyItem = meteoHistory[0];
    var distance = 10000000
    for (var i = 0; i < meteoHistory.length; i++)
    {
        var x = (new Date(meteoHistory[i].time * 1000) - start) * canvas.width / (end - start);
        if (Math.abs(displaySettings.mousePosX - x) < distance)
        {
            historyItem = meteoHistory[i];
            distance = Math.abs(displaySettings.mousePosX - x);
        }
    }
    var lineHeight=ctx.measureText('M').width * 1.5;
    var margin = ctx.measureText('M').width;
    var timestampText = new Date(historyItem.time * 1000).toLocaleString();
    var tempText = 'T: ' + historyItem.temp + '\\xB0C';
    var pressureText = 'P: ' + historyItem.pres + ' Pa (' + Math.round(historyItem.pres / 133.3224 * 100) / 100 + ' mmHg)';
    var humText = 'H: ' + historyItem.hum + '%';
    
    var width = Math.max(ctx.measureText(timestampText).width, ctx.measureText(tempText).width, ctx.measureText(pressureText).width, ctx.measureText(humText).width) + 2 * margin;
    var height = lineHeight * 5;
    var x = displaySettings.mousePosX + width + margin > canvas.width ? displaySettings.mousePosX - width - margin : displaySettings.mousePosX + margin;
    var y = displaySettings.mousePosY + height + margin > canvas.height ? displaySettings.mousePosY - height - margin : displaySettings.mousePosY + margin;

    ctx.fillStyle = '#40404080';
    ctx.fillRect(x, y, width, height);

    ctx.textAlign = 'left';
    ctx.textBaseline = 'top';
    ctx.fillStyle = 'black';
    x += margin;
    y += lineHeight / 2;
    ctx.fillText(timestampText, x, y);
    y += lineHeight;
    ctx.fillText(tempText, x, y);
    y += lineHeight;
    ctx.fillText(pressureText, x, y);
    y += lineHeight;
    ctx.fillText(humText, x, y);
}

function onMouseMove(e) {
    e.preventDefault();
    e.stopPropagation();
    var canvas = document.getElementById('historyGraph');
    var rect = canvas.getBoundingClientRect();
    displaySettings.mousePosX = e.clientX - rect.left;
    displaySettings.mousePosY = e.clientY - rect.top;
    drawGraph();
}

function onMouseLeave(e) {
    e.preventDefault();
    e.stopPropagation();
    displaySettings.mousePosX = null;
    displaySettings.mousePosY = null;
    drawGraph();
}

function onMouseWheel(e) {
    size = displaySettings.size;
    if (!size)
    {
        size = meteoHistory.length > 0 ? (meteoHistory[meteoHistory.length - 1].time - meteoHistory[0].time) * 1000 : null;
    }
    if (size)
    {
        const MULTIPLIER = 1.5;
        displaySettings.size = e.deltaY < 0 ? size / MULTIPLIER : size * MULTIPLIER;
    }
    onMouseMove(e);
    var scaleSelect = document.getElementById('historyScale');
    scaleSelect.selectedIndex = 0;
}

function changeScale(select) {
    displaySettings.size = select.value == '-1' ? null : select.value;
    drawGraph();
}

function receiveWebSocketMessage(event) {
    data = JSON.parse(event.data);
    document.getElementById('temp').innerText = data.temp;
    document.getElementById('pressure').innerText = Math.round(data.pres / 133.3224 * 100) / 100;
    document.getElementById('pressurePa').innerText = data.pres;
    document.getElementById('humidity').innerText = data.hum;
    document.getElementById('timestamp').innerText = new Date(data.time * 1000).toLocaleString();
    meteoHistory.push(data);
    drawGraph();
}

function initCanvas() {
    var canvas = document.getElementById('historyGraph');
    canvas.onmousemove = onMouseMove;
    canvas.onmouseleave = onMouseLeave;
    canvas.onmousewheel = onMouseWheel;
}

function sendHistoryRequest() {
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
}

document.addEventListener('DOMContentLoaded', initCanvas);
sendHistoryRequest();

var socket = new WebSocket('ws://' + window.location.hostname + ':9000');
socket.onmessage = receiveWebSocketMessage;