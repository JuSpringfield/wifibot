//
// About the Settings tab
//
if (!$.cookie("ip_and_port_server"))
	$.cookie("ip_and_port_server", "192.168.2.106:7681", { expires: 20 * 365 });
if (!$.cookie("ip_and_port_ffmpeg"))
	$.cookie("ip_and_port_ffmpeg", "192.168.2.106:8084", { expires: 20 * 365 });
if (!$.cookie("ip_and_port_gps"))
	$.cookie("ip_and_port_gps", "192.168.2.106:8004", { expires: 20 * 365 });
if (!$.cookie("enable_gps_mapping"))
	$.cookie("enable_gps_mapping", "No", { expires: 20 * 365 });
if (!$.cookie("enable_sensors"))
	$.cookie("enable_sensors", "No", { expires: 20 * 365 });
$("#ip_and_port_server").val($.cookie("ip_and_port_server"));
$("#ip_and_port_ffmpeg").val($.cookie("ip_and_port_ffmpeg"));
$("#enable_gps_mapping").val($.cookie("enable_gps_mapping"));
$("#ip_and_port_gps").val($.cookie("ip_and_port_gps"));
$("#enable_sensors").val($.cookie("enable_sensors"));
$("#update_config").on("click", function()
{
	$.cookie("ip_and_port_server", $("#ip_and_port_server").val(), { expires: 20 * 365 });
	$.cookie("ip_and_port_ffmpeg", $("#ip_and_port_ffmpeg").val(), { expires: 20 * 365 });
	$.cookie("ip_and_port_gps", $("#ip_and_port_gps").val(), { expires: 20 * 365 });
	$.cookie("enable_gps_mapping", $("#enable_gps_mapping").val(), { expires: 20 * 365 });
	$.cookie("enable_sensors", $("#enable_sensors").val(), { expires: 20 * 365 });
	location.reload();
});