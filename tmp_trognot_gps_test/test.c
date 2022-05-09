#include <gps.h>
#include <printf.h>

int			main()
{
  struct gps_data_t	gps_data;
  int			gps_data_ret;

  if ((gps_data_ret = gps_open("localhost", "2947", &gps_data)) == 0)
    {
      gps_stream(&gps_data, WATCH_DISABLE, NULL);
      while (1)
	{
	  if (gps_waiting(&gps_data, 3000000) == -1)
	    {

	    }
	  else
	    {
	      printf("%f %f\n", gps_data.fix.latitude, gps_data.fix.longitude);
	    }
	}
    }
  else
    {
      printf("NOK\n");
    }
  return (0);
}
