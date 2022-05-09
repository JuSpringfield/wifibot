//
// About the websocket
//
function initWebSocket()
{
	console.log("hello");
	websocket = new WebSocket('ws://' + $.cookie("ip_and_port_server"));
	websocket.binaryType = 'arraybuffer';
	websocket.onopen = function(evt) { onOpen(evt) };
	websocket.onclose = function(evt) { onClose(evt) };
	websocket.onmessage = function(evt) { onMessage(evt) };
	websocket.onerror = function(evt) { onError(evt) };
}
function onOpen(evt) {
	$("#connection_icon").css("background-color", "#48a200");
	conok = 1;
}
function onClose(evt) {
	$("#connection_icon").css("background-color", "red");
	conok = 0;
	ttl_y_virtual = 0;
	ttl_y_physical = 0;
	ttl_x_virtual = 0;
	ttl_x_physical = 0;
	display_odometry_left.setValue(0);
	display_odometry_right.setValue(0);
	display_intensite.setValue("00.0");
	Gauge.Collection.get('gauge2').setValue(0);
	Gauge.Collection.get('gauge1').setValue(0);
	setDistance(0, 0, 0, 0);
	setTimeout(function() {
		initWebSocket();
	}, 200);
}
function onMessage(evt) {
	var data = evt.data;
	var dv = new DataView(data);
	var speedLeft = parseInt((dv.getUint8(1) << 8) + dv.getUint8(0));
	if (speedLeft > 32767) speedLeft -= 65536;
	var speedRight= parseInt((dv.getUint8(10) << 8) + dv.getUint8(9));
	if (speedRight > 32767) speedRight -= 65536;
	var speedAvg = Math.abs((speedLeft + speedRight) / 2);
	speedAvg = (speedAvg / 100 < 2) ? (speedAvg) : (200);
	var battery = parseInt(dv.getUint8(2));
	var odometry_left = (dv.getUint8(8) << 24) + (dv.getUint8(7) << 16) + (dv.getUint8(6) << 8) + (dv.getUint8(5));
	var odometry_right = (dv.getUint8(16) << 24) + (dv.getUint8(15) << 16) + (dv.getUint8(14) << 8) + (dv.getUint8(13));
	display_odometry_left.setValue(odometry_left.toString());
	display_odometry_right.setValue(odometry_right.toString());
	display_intensite.setValue(pad((dv.getUint8(17) / 10).toFixed(1).toString(), 4).toString());
	Gauge.Collection.get('gauge2').setValue(speedAvg / 100);
	Gauge.Collection.get('gauge1').setValue(battery / 10);
	setDistance(dv.getUint8(3), dv.getUint8(11), dv.getUint8(12), dv.getUint8(4));
}
function onError(evt) {}
function doSend(message) {}
setTimeout(function() {
	initWebSocket();
}, 1000);


//
// About sending X and Y to the server
//
setInterval(function() {
	if (conok == 1) {
		arr = new ArrayBuffer(8);
		view = new DataView(arr);
		view.setInt16(0, -ttl_x_virtual - ttl_x_physical, false);
		view.setInt16(2, ttl_y_virtual + ttl_y_physical, false);
		view.setInt16(4, -ttl_x_virtual - ttl_x_physical, false);
		view.setInt16(6, ttl_y_virtual + ttl_y_physical, false);
		websocket.send(arr);
	}
	gauge.draw();
	/*LAT -= 0.000001 * (Math.random() * 10);
	LNG += 0.000001 * (Math.random() * 10);*/
	if ($.cookie("enable_gps_mapping") == "Yes")
	{
		if (marker)
		{
			//marker.setPosition({lat: LAT, lng: LNG});
	                 //infowindow.setContent("<div style='min-width: 100px'><h4 style='color: black'>My Wifibot</h4><span style='color: black;'>Lat : "+Math.round(LAT * 1000000) / 1000000+"<br />Lng : "+Math.round(LNG * 1000000) / 1000000+"</span></div>");
	 		//var path = poly.getPath();
		    //path.push(new google.maps.LatLng(LAT, LNG));
		    //poly.setPath(path);
		    if (follow == true)
		    {
			    var latLng = marker.getPosition();
				map.setCenter(latLng);
			}
		}
		else
		{
			//poly_marker.push({lat: LAT, lng: LNG});
		}
	}
}, 1 / 30 * 5000);