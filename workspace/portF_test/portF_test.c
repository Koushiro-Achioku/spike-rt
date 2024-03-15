#include <stdlib.h>
#include <stdio.h>
#include <kernel.h>

#include <spike/hub/system.h>

#include <portF_test.h>

#include "spike/pup/motor.h"
#include "spike/pup/colorsensor.h"
#include "spike/pup/forcesensor.h"
#include "spike/pup/ultrasonicsensor.h"

#include "spike/hub/battery.h"
#include "spike/hub/button.h"
#include "spike/hub/display.h"
#include "spike/hub/imu.h"
#include "spike/hub/light.h"
#include "spike/hub/speaker.h"

#include <pbio/color.h>

#include "kernel_cfg.h"
#include "syssvc/serial.h"

pup_motor_t *motorF;

void Main(intptr_t exinf)
{
  // ここからプログラムを書く
  int i;

  // モータのセットアップ
  motorF = pup_motor_get_device(PBIO_PORT_ID_F);
  pup_motor_setup(motorF, PUP_DIRECTION_CLOCKWISE, true);

  // パワーの設定 (正回転)
  pup_motor_set_power(motorF, 100);
  dly_tsk(3*1000*1000);
  pup_motor_stop(motorF);

  dly_tsk(500*1000);

  // パワーの設定 (逆回転)
  pup_motor_set_power(motorF, -100);
  dly_tsk(3*1000*1000);
  pup_motor_stop(motorF);

  dly_tsk(500*1000);

  // ブレーキ関数のテスト
  pup_motor_set_power(motorF, 100);
  dly_tsk(3*1000*1000);
  pup_motor_brake(motorF);

  dly_tsk(500*1000);

  // 角度維持関数のテスト
  pup_motor_hold(motorF);
  dly_tsk(5*1000*1000);

  // スピードの設定 (正回転)
  pup_motor_set_speed(motorF, 360);
  dly_tsk(1*1000*1000);
  pup_motor_stop(motorF);

  dly_tsk(500*1000);

  // スピードの設定 (逆回転)
  pup_motor_set_speed(motorF, -360);
  dly_tsk(1*1000*1000);
  pup_motor_stop(motorF);

  dly_tsk(500*1000);

  // パワー値の取得
  pup_motor_set_power(motorF, 50);

  for(i=0;i<1000;i++){
    int now_power_F = pup_motor_get_power(motorF); // 今のパワーを得る
    hub_display_number(now_power_F); // ディスプレイに表示する
    dly_tsk(5*1000);
  }

  pup_motor_stop(motorF);
  dly_tsk(500*1000);

  // スピード値の取得
  pup_motor_set_speed(motorF, 360); // 「1秒あたり360度」の回転率で回す

  for(i=0;i<1000;i++){
    int now_speed_F = pup_motor_get_speed(motorF); // 今のパワーを得る
    hub_display_number(now_speed_F/10); // ディスプレイに表示する
    dly_tsk(5*1000);
  }

  pup_motor_stop(motorF);
  dly_tsk(500*1000);

  // 角度の取得
  pup_motor_reset_count(motorF); // モータA の角度をリセット

  while(abs(pup_motor_get_count(motorF)) < 500){
    // パワーの設定
    pup_motor_set_power(motorF, 50);
    hub_display_number(abs(pup_motor_get_count(motorF))/10); // ディスプレイに表示する
  }

  pup_motor_stop(motorF);
  dly_tsk(2*1000*1000);

  exit(0);
}
