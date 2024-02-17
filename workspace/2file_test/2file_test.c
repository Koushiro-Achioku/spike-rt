#include <stdlib.h>
#include <kernel.h>

#include <spike/hub/system.h>

#include <2file_test.h>

#include "spike/pup/motor.h"
#include "spike/pup/colorsensor.h"
#include "spike/pup/forcesensor.h"
#include "spike/pup/ultrasonicsensor.h"

#include "spike/hub/battery.h"
#include "spike/hub/button.h"
#include "spike/hub/display.h"
#include "spike/hub/imu.h"
#include "spike/hub/light.h"

#include <pbio/color.h>

#include "functions.h"

void Main(intptr_t exinf)
{
  // ここからプログラムを書く

  // int c;
  // char str[8];

  // c = calc_sum(1,2);

  // sprintf(str,"%d",c);
  // serial_opn_por(SIO_USB_PORTID);

  // while (1)
  // {
  //   serial_wri_dat(SIO_USB_PORTID, str, sizeof(str));
  //   dly_tsk(1*1000*1000);
  // }

  hub_display_number(calc_sum(1,2));
  dly_tsk(5*1000*1000);
  
  exit(0);
}
