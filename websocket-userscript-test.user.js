// ==UserScript==
// @name        booru-test
// @namespace   etnmich
// @include     http://*.booru.org/*
// @version     1
// @grant       none
// ==/UserScript==

//settings
var wsServer = "localhost";
var wsPort   =  50000;
//end settings

// run code when page DOM has loaded
window.onload = main;

// base
function main() {
    console.log("starting booru updater");

    var img = document.getElementById('image');
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
}

// WebSocket functionality
function initSocketSend(data) {
    // Another nice try-block that _maybe_ catches something
    try {
        var ws = new WebSocket("ws://" + wsServer + ':' + wsPort);
        console.log("ws init");
        ws.onopen = function(ev) {
            console.log("socket opened");
            ws.send(data);
            console.log("data sent")
        }
        ws.onerror = function(ev) {
            console.log("error occured");
            console.log(ev);
        }
        console.log(ws);
    } catch (err) {
        console.log(err);
    }
}
