// About virtual joystick
var enable_virtual_joystick = 0;
var init_x_virtual = 0;
var init_y_virtual = 0;
$("#tab1_up").on('touchmove',function(e){
  e.preventDefault();
});
$("#tab1_up").bind('mousemove touchmove', function(e) {
	if (enable_virtual_joystick == 1)
	{
		var pos   = $(this).offset();
		var elPos = { X:pos.left , Y:pos.top };
		var mPos  = { X:e.clientX-elPos.X, Y:e.clientY-elPos.Y };
		x = e.pageX - elPos.X;
		y = e.pageY - elPos.Y;
		if ((isNaN(mPos.X) || isNaN(mPos.Y)) && (e.touches.length == 1)){
			var touch = e.touches[0];
			var node = touch.target;
			mPos.X = touch.pageX - elPos.X;
			mPos.Y = touch.pageY - elPos.Y;
		}
		else
		{
			mPos.X = x;
			mPos.Y = y;
		}
		ttl_x_virtual = mPos.X - init_x_virtual;
		ttl_y_virtual = mPos.Y - init_y_virtual;
		$("#joystick_img_in").css("top", (mPos.Y - 12) + "px");
		$("#joystick_img_in").css("left", (mPos.X - 12) + "px");
	}
});
$("#tab1_up").bind('mouseup touchend', function(event) {
	//console.log("uh");
	enable_virtual_joystick = 0;
	init_x_virtual = 0;
	init_y_virtual = 0;
	ttl_x_virtual = 0;
	ttl_y_virtual = 0;
	$("#joystick_img_in").css("display", "none");
	$("#joystick_img_out").css("display", "none");
});
$("#tab1_up").bind('mousedown touchstart', function(e) {
	enable_virtual_joystick = 1;
	var pos   = $(this).offset();
	var elPos = { X:pos.left , Y:pos.top };
	var mPos  = { X:e.clientX-elPos.X, Y:e.clientY-elPos.Y };
	x = e.pageX - elPos.X;
	y = e.pageY - elPos.Y;
	if ((isNaN(mPos.X) || isNaN(mPos.Y)) && (e.touches.length == 1)){
		var touch = e.touches[0];
		var node = touch.target;
		mPos.X = touch.pageX - elPos.X;
		mPos.Y = touch.pageY - elPos.Y;
	}
	else
	{
		mPos.X = x;
		mPos.Y = y;
	}
	init_x_virtual = mPos.X;
	init_y_virtual = mPos.Y;
	$("#joystick_img_in").css("top", (mPos.Y - 12) + "px");
	$("#joystick_img_in").css("left", (mPos.X - 12) + "px");
	$("#joystick_img_in").css("display", "block");
	$("#joystick_img_out").css("top", (mPos.Y - 25) + "px");
	$("#joystick_img_out").css("left", (mPos.X - 25) + "px");
	$("#joystick_img_out").css("display", "block");
});