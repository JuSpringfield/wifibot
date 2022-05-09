var control_btn = false;
$("#control_btn").on("click", function()
{
	if (control_btn == false)
	{
		control_btn = true;
		$('#control_btn').removeClass('btn-success').addClass('btn-danger').addClass('active');
	}
	else
	{
		control_btn = false;
		$('#control_btn').addClass('btn-success').removeClass('btn-danger').removeClass('active');
	}
});