#include <stdlib.h>
#include <kernel.h>

#include <spike/hub/system.h>

#include <multi_task_test3.h>

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

// 以下を追加
#include "kernel_cfg.h"
#include "spike/hub/speaker.h"

int flag = 0;
double count = NOTE_C5;

void Main(intptr_t exinf)
{
  //プログラムをここから書く
  int a = 0;
  hub_button_t pressed;

  hub_speaker_set_volume(10);
  

  while(!(pressed&HUB_BUTTON_LEFT)){
    hub_button_is_pressed(&pressed);
  }

  // Main のみ動作
  while(a < 5){
    hub_display_number(a);
    a++;
    dly_tsk(1*1000*1000);
  }

  // SUB_TASK を起動
  act_tsk(SUB_TASK);

  // Main と Sub が動作
  while(a < 10){
    hub_display_number(a);
    a++;
    dly_tsk(1*1000*1000);
  }

  // 周期ハンドラ CYC_HDR を起動
  sta_cyc(CYC_HDR);

  while(a < 15){
    hub_display_number(a);
    a++;
    dly_tsk(1*1000*1000);
  }

  // 周期ハンドラ CYC_HDR が停止
  stp_cyc(CYC_HDR);

  // flagをたてる
  flag = 1;

  exit(0);
}

void Sub(intptr_t exinf)
{
  int a = 0;

  while(flag==0){
    if(a%2){
      hub_light_off();
    }
    else{
      hub_light_on_color(PBIO_COLOR_RED);
    }
    a++;
    dly_tsk(100*1000);
  }

  exit(0);
}

void cychdr(intptr_t exinf)
{
  hub_speaker_play_tone(count, 100);
  count*=1.059;
}