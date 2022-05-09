//
// About sensors
//
function getGreenToRed(percent){
            r = percent<50 ? 255 : Math.floor(255-(percent*2-100)*255/100);
            g = percent>50 ? 255 : Math.floor((percent*2)*255/100);
            return 'rgb('+r+','+g+',0)';
        }
function setDistance (tl, tr, br, bl) {
	if ($.cookie("enable_sensors") == "Yes")
	{
		tl = (tl < 0) ? (0) : (tl);
		tl = (tl > 255) ? (255) : (tl);
		tr = (tr < 0) ? (0) : (tr);
		tr = (tr > 255) ? (255) : (tr);
		br = (br < 0) ? (0) : (br);
		br = (br > 255) ? (255) : (br);
		bl = (bl < 0) ? (0) : (bl);
		bl = (bl > 255) ? (255) : (bl);
		var distance = tl * 50 / 255;
		$('.tl').css('height', distance + '%');
		$('.tl').css('background-color', getGreenToRed(100 - distance * 2));
		$('.tl').css('top', (50 - distance) + "%");
		distance = tr * 50 / 255;
		$('.tr').css('height', distance + '%');
		$('.tr').css('background-color', getGreenToRed(100 - distance * 2));
		$('.tr').css('top', (50 - distance) + "%");
		distance = bl * 50 / 255;
		$('.bl').css('height', distance + '%');
		$('.bl').css('background-color', getGreenToRed(100 - distance * 2));
		distance = br * 50 / 255;
		$('.br').css('height', distance + '%');
		$('.br').css('background-color', getGreenToRed(100 - distance * 2));
	}
}