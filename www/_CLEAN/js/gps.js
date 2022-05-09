//
// About map
//
if ($.cookie("enable_gps_mapping") == "Yes")
{
    var jsElm = document.createElement("script");
    jsElm.type = "application/javascript";
    jsElm.src = "https://maps.googleapis.com/maps/api/js";
    document.body.appendChild(jsElm);
}
var LAT = 48.8584;
var LNG = 2.2945;
var map_already_opened = false;
var marker;
var map;
var infowindow;
var poly;
var poly_marker = [];
var follow = true;
function myMap() {
  var mapCanvas = document.getElementById("map");
  var mapOptions = {
    center: new google.maps.LatLng(LAT, LNG),
    zoom: 18,
    mapTypeId: google.maps.MapTypeId.ROADMAP
  }
  map = new google.maps.Map(mapCanvas, mapOptions);
  marker = new google.maps.Marker({
    position: {lat: LAT, lng: LNG},
    map: map,
    title: 'My Wifibot'
  });
  infowindow = new google.maps.InfoWindow({
    content: "NULL"
  });
  infowindow.open(map, marker);
  poly = new google.maps.Polyline({
    map: map,
    path: poly_marker,
	geodesic: true,
	strokeColor: '#FF0000',
	strokeOpacity: 0.7,
	strokeWeight: 3
  });
}
$('a[data-toggle="tab"]').on('shown.bs.tab', function (e) {
  if ($(e.target).attr("href") == "#tab1default")
  {
  	if (ffmpeg_canvas == 2)
  	{
  		$("#ffmpeg_canvas").appendTo("#ffmpeg_1");
  		ffmpeg_canvas = 1;
  	}
  }
  if ($(e.target).attr("href") == "#gps")
  {
  	if (ffmpeg_canvas == 1)
  	{
  		$("#ffmpeg_canvas").appendTo("#ffmpeg_2");
  		ffmpeg_canvas = 2;
  	}
  	if (map_already_opened == false)
  	{
  		if ($.cookie("enable_gps_mapping") == "Yes")
	  		myMap();
  		map_already_opened = true;
  	}
  	google.maps.event.trigger(map, "resize");
	var latLng = marker.getPosition();
	map.setCenter(latLng);
  }
});
$("#recenter_map").on("click", function()
{
	var latLng = marker.getPosition();
	map.setCenter(latLng);
});
$("#trace_map").on("click", function()
{
	if ($("#trace_map").html() == "Disable trace")
	{
		poly.setOptions({
		        strokeWeight: 0
		      });
		$("#trace_map").html("Enable trace");
	}
	else
	{
		poly.setOptions({
		        strokeWeight: 2
		      });
		$("#trace_map").html("Disable trace");
	}
});
$("#reset_trace_map").on("click", function()
{
	poly.setPath([]);
});
$("#follow_map").on("click", function()
{
	console.log("salut");
	if ($("#follow_map").html() == "Follow my Wifibot")
	{
		follow = true;
		$("#follow_map").html("Unfollow my Wifibot");
	}
	else
	{
		follow = false;
		$("#follow_map").html("Follow my Wifibot");
	}
});
var socket = io.connect("http://" + $.cookie("ip_and_port_gps"));
socket.on('message', function(message) {
    var coord = message.split(",");
    console.log(coord);
    if (marker)
    {
      marker.setPosition({lat: parseFloat(coord[1]), lng: parseFloat(coord[0])});
      infowindow.setContent("<div style='min-width: 100px'><h4 style='color: black'>My Wifibot</h4><span style='color: black;'>Lat : "+parseFloat(coord[1])+"<br />Lng : "+parseFloat(coord[0])+"</span></div>");
      var path = poly.getPath();
        path.push(new google.maps.LatLng(parseFloat(coord[1]), parseFloat(coord[0])));
        poly.setPath(path);
        if (follow == true)
        {
          var latLng = marker.getPosition();
        map.setCenter(latLng);
      }
    }
    else
    {
      poly_marker.push({lat: parseFloat(coord[1]), lng: parseFloat(coord[0])});
    }
})