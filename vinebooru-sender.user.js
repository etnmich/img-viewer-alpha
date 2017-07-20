// ==UserScript==
// @name        vinebooru-sender
// @namespace   etnmich
// @include     https://booru.vineshroom.net/*
// @require     https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js
// @require     https://raw.githubusercontent.com/js-cookie/js-cookie/master/src/js.cookie.js
// @version     2
// @grant       none
// ==/UserScript==

console.log("script start");

//settings
var wsServer = "localhost";
var wsPort   =  50000;
var wsSend   =  'true';
//end settings

if (Cookies.get('ws_on')) {
    wsSend = Cookies.get('ws_on');
}

var img = $('#main_image')[0];

if (img) {
    console.log("found image!");
    var obj = {};
    obj.type = 'img';
    obj.url = img.src;
    // I'm not sure if this block really catches anything
    try {
        var txt = JSON.stringify(obj);
        initSocketSend(txt);
    } catch (err) {
        console.log(err);
    }
} else {
    console.log("no image found");
}

// WebSocket functionality
function initSocketSend(data) {
    // Another nice try-block that _maybe_ catches something
    if (wsSend == 'false') {
        console.log('automatic sending is turned off');
        return;
    }
    try {
        var ws = new WebSocket("ws://" + wsServer + ':' + wsPort);
        console.log("ws init");
        ws.onopen = function(ev) {
            console.log("socket opened");
            ws.send(data);
            console.log("data sent");
        };
        ws.onerror = function(ev) {
            console.log("error occured");
            console.log(ev);
        };
        console.log(ws);
    } catch (err) {
        console.log(err);
    }
}

// Site changes
var sec = document.createElement('section');
sec.id = 'WebsocketLeft';
$('nav').prepend(sec);
var header = document.createElement('h3');
header.innerText = 'WebSocket Control';
$(header).attr('data-toggle-sel', '#WebsocketLeft');
$(header).addClass('shm-toggler');
$(sec).append(header);
var tbl = document.createElement('table');
$(sec).append(tbl);
var lbl = document.createElement('label');
lbl.htmlFor = 'ws_send';
lbl.innerText = 'Send img URL automatically to WebSocket server';
$(tbl).append(lbl);
var check = document.createElement('input');
check.type = 'checkbox';
check.checked = wsSend;
check.id = 'ws_send';
$(check).change(function() {
    wsSend = this.checked;
    Cookies.set('ws_on', wsSend);
});
$(tbl).append(check);
