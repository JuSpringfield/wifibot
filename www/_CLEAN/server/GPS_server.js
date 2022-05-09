// Inclusion des libs nécessaires à la récupération des données GPS
var gpsd = require('node-gpsd');
var nmea = require('node-nmea');

// Initialisation dy système GPSd
var listener = new gpsd.Listener({
    port: 2947,
    hostname: 'localhost',
    logger:  {
        info: function() {},
        warn: console.warn,
        error: console.error
    },
   	parse: false
});
listener.connect(function() {
    console.log('Connected to the GPS');
});
listener.watch({class: 'WATCH', nmea: true});


// Inclusion des libs nécessaires à la création d'un websocket
var http = require('http');
var server = http.createServer(function(req, res) {
    res.writeHead(200, {"Content-Type": "text/html"});
    res.end("Websocket NodeJS");
});
var io = require('socket.io').listen(server);
io.sockets.on('connection', function (socket) {
    console.log('New client connected');
});

// Emission des coordonnées GPS lorsqu'une donnée GPGGA est reçue
listener.on('raw', function(data) {
	if (data.substring(0, 6) == "$GPGGA")
	{
		raw = nmea.parse(data.trim());
	  	console.log("Emit: " + raw.loc.geojson.coordinates[0] + "," + raw.loc.geojson.coordinates[1]);
	  	io.emit('message', raw.loc.geojson.coordinates[0] + "," + raw.loc.geojson.coordinates[1]);
	}
});

// Le serveur websocket est lancé sur le port 8004
server.listen(8004);
