//
// Some basics
//
var conok = 0;
var ttl_x_virtual = 0;
var ttl_x_physical = 0;
var ttl_y_virtual = 0;
var ttl_y_physical = 0;
var ffmpeg_canvas = 1;

//
// Gauge init
//
var gauge = new Gauge({
	renderTo: 'gauge'
});

//
// Camera init
//
var client = new WebSocket( 'ws://'+$.cookie("ip_and_port_ffmpeg")+'/' );
var canvas = document.getElementById('videoCanvas');
var player = new jsmpeg(client, {canvas:canvas});