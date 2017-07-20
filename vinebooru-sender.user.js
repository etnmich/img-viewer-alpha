// ==UserScript==
// @name        vinebooru-sender
// @namespace   etnmich
// @include     https://booru.vineshroom.net/*
// @require     https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js
// @require     https://raw.githubusercontent.com/js-cookie/js-cookie/master/src/js.cookie.js
// @version     3
// @grant       none
// ==/UserScript==

// redefine logging because I'm too lazy to search and replace
// ...plus I just had to try if it worked on JS like this
conlog = console.log;
function mylog(txt) {
    conlog('[vinebooru-sender] ' + txt);
}
console.log = mylog;


console.log("script running");

//default settings
var wsServer = "localhost";
var wsPort   =  50000;
var wsSend   =  true;
//end settings

// get settings from cookies
if (Cookies.get('ws_on')) {
    wsSend = parseInt(Cookies.get('ws_on'));
}
if (Cookies.get('ws_port')) {
    wsPort = parseInt(Cookies.get('ws_port'));
}
if (Cookies.get('ws_host')) {
    wsServer = Cookies.get('ws_host');
}

// script-global image variable
var img = $('#main_image')[0];

// autodisplay images
if (wsSend) {
    showImage();
} else {
    console.log('automatic showing is turned off');
}

/*
var img = $('#main_image')[0];

if (img) {
    console.log("found image!");
    var obj = {};
    obj.type = 'img';
    obj.payload = img.src;
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
*/



// image display function
function showImage() {
    if (img) {
        console.log("found image!");
        var obj = {};
        obj.type = 'img';
        obj.payload = img.src;
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
        //console.log(ws);
    } catch (err) {
        console.log(err);
    }
}

// Site changes
// buttons
var sec = document.createElement('section');
sec.id = 'WebsocketLeft';
$('nav').prepend(sec);

var header = document.createElement('h3');
header.innerText = 'WebSocket Control';
$(header).attr('data-toggle-sel', '#WebsocketLeft');
$(header).addClass('shm-toggler');
$(sec).append(header);
var basediv = document.createElement('div');
$(sec).append(basediv);

var div = document.createElement('div');
$(basediv).append(div);

var show = makeButton('Display image on viewer');

var rot_l = makeButton('Rotate 90° counterclockwise');
var rot_r = makeButton('Rotate 90° clockwise');

$(div).append(show, rot_l, rot_r);

//options
var showOpts = document.createElement('a');
showOpts.innerText = 'Show options';
showOpts.href = '#';
var hideOpts = document.createElement('a');
hideOpts.innerText = 'Hide options';
hideOpts.href = '#';
var opts = document.createElement('div');

$(basediv).append(showOpts, hideOpts, opts);
$(hideOpts).hide();
$(opts).hide();

var lbl1 = document.createElement('label');
lbl1.htmlFor = 'ws_send';
lbl1.innerText = 'Send img URL automatically to WebSocket server';

var check = document.createElement('input');
check.type = 'checkbox';
check.checked = wsSend;
check.id = 'ws_send';

var lbl2 = document.createElement('label');
lbl2.htmlFor = 'ws_port';
lbl2.innerText = 'WebSocket server port';

var port = document.createElement('input');
port.type = 'number';
port.value = wsPort;
port.min = 1024;
port.max = 65535;
port.id = 'ws_port';

var lbl3 = document.createElement('label');
lbl3.htmlFor = 'ws_host';
lbl3.innerText = 'WebSocket server host';

var host = document.createElement('input');
host.type = 'text';
host.value = wsServer;

$(opts).append(lbl1, check, lbl2, port, lbl3, host);

// binds
$('#WebsocketLeft > h3').click(function () {
    $(this).next().slideToggle('slow');
});
$(show).click(showImage);
$(rot_l).click(function() {
    var obj = {};
    obj.type = 'rotate';
    obj.payload = 'left';
    try {
        var txt = JSON.stringify(obj);
        initSocketSend(txt);
    } catch (err) {
        console.log(err);
    }
    $(img).rotate(-90);
});
$(rot_r).click(function() {
    var obj = {};
    obj.type = 'rotate';
    obj.payload = 'right';
    try {
        var txt = JSON.stringify(obj);
        initSocketSend(txt);
    } catch (err) {
        console.log(err);
    }
    $(img).rotate(90);
});

$(showOpts).click(function() {
    $(this).hide();
    $(opts).slideDown('slow');
    $(hideOpts).show();
});
$(hideOpts).click(function() {
    $(this).hide();
    $(opts).slideUp('slow');
    $(showOpts).show();
});
$(check).change(function() {
    wsSend = this.checked;
    Cookies.set('ws_on', wsSend ? 1 : 0);
});
$(port).change(function() {
    wsPort = this.value;
    if (wsPort < 1024 || wsPort > 65535) {
        wsPort = 50000;
        this.value = wsPort;
    }
    Cookies.set('ws_port', wsPort);
});
$(host).change(function() {
    wsServer = this.value;
    Cookies.set('ws_host', wsServer);
});

// helper functions
function makeButton(text) {
    var button = document.createElement('input');
    button.type = 'submit';
    button.value = text;
    return button;
}
