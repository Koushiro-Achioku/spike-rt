#include <stdlib.h>
#include <kernel.h>

#include <spike/hub/system.h>

#include <gyro_turn.h>

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

#include "math.h"

// IMU用の配列 0:ロール(x) 1:ピッチ(y) 2:ヨー(z)
float ang_v[3] = {0}; // 角度(角位置)
float imu_offset[3] = {0}; // 角度オフセット

// モータ用ポインタ
pup_motor_t *motorA;
pup_motor_t *motorB;

void Main(intptr_t exinf)
{
  // ここからプログラムを書く
  motorA = pup_motor_get_device(PBIO_PORT_ID_A);
  motorB = pup_motor_get_device(PBIO_PORT_ID_B);

  // モータのセットアップ
  pup_motor_setup(motorA, PUP_DIRECTION_CLOCKWISE, true);
  pup_motor_setup(motorB, PUP_DIRECTION_COUNTERCLOCKWISE, true);

  imu_setup(imu_offset); // IMU初期化・オフセット同定

  // 左ボタンが押されるまで待機 (LED: 緑)
  hub_button_t pressed;
  while(!(pressed&HUB_BUTTON_LEFT)){
    hub_button_is_pressed(&pressed);
    hub_light_on_color(PBIO_COLOR_GREEN);
  }

  dly_tsk(500*1000); // 500ms待機

  sta_cyc(CYC_HDR); // ジャイロセンサ監視開始

  // // 右90度回転
  // while(fabs(ang_v[2])<90){
  //   pup_motor_set_power(motorA, -30);
  //   pup_motor_set_power(motorB, 30);
  // }

  // 右90度回転 (比例制御)
  while(fabs(ang_v[2])<90){
    int power = (90 - abs((int)ang_v[2]))*0.5;
    int power_A = - 15 - power;
    int power_B = 15 + power;
    pup_motor_set_power(motorA, power_A);
    pup_motor_set_power(motorB, power_B);
  }

  // モータ停止
  pup_motor_stop(motorA);
  pup_motor_stop(motorB);

  dly_tsk(500*1000); // 500ms待機

  // IMU初期化
  hub_imu_init();
  ang_v[0] = 0;
  ang_v[1] = 0;
  ang_v[2] = 0;

  // // 左90度回転
  // while(fabs(ang_v[2])<90){
  //   pup_motor_set_power(motorA, 30);
  //   pup_motor_set_power(motorB, -30);
  // }

  // 左90度回転 (比例制御)
  while(fabs(ang_v[2])<90){
    int power = (90 - abs((int)ang_v[2]))*0.5;
    int power_A = 15 + power;
    int power_B = - 15 - power;
    pup_motor_set_power(motorA, power_A);
    pup_motor_set_power(motorB, power_B);
  }

  // モータ停止
  pup_motor_stop(motorA);
  pup_motor_stop(motorB);

  stp_cyc(CYC_HDR); // ジャイロセンサ監視終了

  exit(0);
}

void gyro_monitor(intptr_t exinf)
{
  // IMU角加速度 格納用配列
  float ang_raw[3];
  hub_imu_get_angular_velocity(ang_raw);

  // オフセット補正
  ang_v[0] += (ang_raw[0] - imu_offset[0]) * 0.001;
  ang_v[1] += (ang_raw[1] - imu_offset[1]) * 0.001;
  ang_v[2] += (ang_raw[2] - imu_offset[2]) * 0.001;
}

void imu_setup(float offset[3]){
  dly_tsk(3*1000*1000); // 3s待機
  
  hub_light_on_color(PBIO_COLOR_ORANGE); // LED: オレンジ

  hub_imu_init(); // IMUの初期化

  float ang_raw[3]; // IMU角加速度 raw値 格納用配列

  // オフセット同定 (1秒間で1000回測定して平均取る)
  for(int i=0; i<1000; i++){
    hub_imu_get_angular_velocity(ang_raw); //角加速度取得
    offset[0] += ang_raw[0];
    offset[1] += ang_raw[1];
    offset[2] += ang_raw[2];
    dly_tsk(1*1000); // 1ms待機
  }

  // オフセットをサンプル取得回数で割る
  offset[0] /= 1000;
  offset[1] /= 1000;
  offset[2] /= 1000;
}
