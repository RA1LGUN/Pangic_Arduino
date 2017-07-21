//#include "LCD12864RSPI.h"
#define AR_SIZE(a) sizeof(a) / sizeof(a[0])
#define SersorLED 13
#define SensorINPUT 2
#define MOTO_A 3
#define MOTO_B 4  //电机驱动
#define OPENTIME 500  //开门电机转动时间ms
#define TIMEMAX 4000  //单次超时
#define TIMECOMMAX 12000  //总超时
#define FAUL_TRATE_SHOCK 30  //单次敲击时间容错
#define FAUL_TRATE_ERROR 2  //错误敲击次数容错
#define MAXSING 4  //曲目数量
int state = 0;  //震动计数

int sing_Shock = 0;  //歌曲次数
int user_Shock = 0;  //手敲击次数计数
int user_Error = 0;  //错误次数

int time_ms = 0;  //单次震动计时
int timeCom_ms = 0;  //总计时

int game_switch = 0;  //开始计时标志
int game_flag = 0;  //游戏状态：0待机，1游戏
int Sing_flag = 0;  //当前歌曲标志
unsigned char SystemArr[3][1] = {
  {
    0xA7
  },//失败
  {
    0xC9
  },//成功
  {
    0xE1
  },//开门提示
};
unsigned char SingNameArr[3][1] = {
  {0xB9},  //公主殿下
  {0xBB},  //虎纹鲨鱼
  {0xC4},  //东京吃货
};

int SingGameArr0[] = {165, 520, 98, 312, 116, 192, 165, 180, 165, 186, 180};  //公主殿下
int SingGameArr1[] = {384, 159, 135, 130, 365, 158, 127, 142, 360, 150, 142, 142, 150, 137};  //虎纹鲨鱼
int SingGameArr2[] = {117, 210, 190, 130, 210, 230, 210, 212, 250, 110, 298, 119, 233, 110};  //东京吃货

unsigned int UserGameArr[] = {};
void setup() {
//  LCDA Initialise();
  delay(100);
  pinMode(SersorLED, OUTPUT);
  pinMode(MOTO_A, OUTPUT);
  pinMode(MOTO_B, OUTPUT);
  pinMode(SensorINPUT, INPUT);
  attachInterrupt(0, shock, FALLING);
  Serial.begin(9600);
  displaySing();
}

void loop() {
  if (state != 0)
  {
    state = 0;
    digitalWrite(SersorLED, HIGH);
    if (time_ms > 90) {
      if (game_flag == 0) {
        switch (Sing_flag) {/*统计歌曲打击数*/
          case 0: sing_Shock = sizeof(SingGameArr0) / sizeof(int); break;
          case 1: sing_Shock = sizeof(SingGameArr0) / sizeof(int); break;
          case 2: sing_Shock = sizeof(SingGameArr0) / sizeof(int); break;
          case 3: sing_Shock = sizeof(SingGameArr0) / sizeof(int); break;
        }
        game_flag = 1;
      }

      if (user_Shock < sing_Shock) {
        /*敲击判断是否正确*/
        int Singshock_temp = 0;
        switch (Sing_flag) {
          case 0: Singshock_temp = SingGameArr0[user_Shock]; break;
          case 1: Singshock_temp = SingGameArr1[user_Shock]; break;
          case 2: Singshock_temp = SingGameArr2[user_Shock]; break;
//          case 3: Singshock_temp = SingGameArr3[user_Shock]; break;
        }
        if (time_ms <= (Singshock_temp + FAUL_TRATE_SHOCK) && time_ms >= (Singshock_temp - FAUL_TRATE_SHOCK))
        {
          /*判断成功不操作*/
          //Serial.println("su");
        }
        else {
          user_Error++;
        }
        user_Shock++;
        /*敲击判断是否正确不分结束*/

        char stringa[25];
        itoa(time_ms, stringa, 10);
        Serial.println(stringa);
        time_ms = 0;
      }
    }
  }
  else if (game_switch != 0) {
    delay(1);
    digitalWrite(SersorLED, LOW);
    time_ms++;
    timeCom_ms++;
    if (time_ms > TIMEMAX || timeCom_ms > TIMECOMMAX || (user_Shock == sing_Shock && sing_Shock != 0)) { //单次超时或总超时
      if (user_Shock == sing_Shock
          && user_Error <= FAUL_TRATE_ERROR
          && sing_Shock != 0
          && user_Shock != 0) {
//        LCDA.CLEAR();//清屏
        delay(100);
//        LCDA.DisplayString(2, 2, SystemArr[1], AR_SIZE(SystemArr[1])); //显示验证成功
        delay(3000);
        openDoor();
        Serial.println("su!!!");
      }
      else {
//        LCDA.CLEAR();
        delay(100);
//        LCDA.DisplayString(2, 2, SystemArr[0], AR_SIZE(SystemArr[0])); //显示验证失败
        delay(3000);
      }
      game_switch = 0; //超时后关闭计时
      time_ms = 0;
      timeCom_ms = 0;
      user_Shock = 0;
      user_Error = 0;
      sing_Shock = 0;
      game_flag = 0;//待机

      Sing_flag++;//换歌
      if (Sing_flag > MAXSING - 1) {
        Sing_flag = 0;
      }
      displaySing();
    }
  }
}

void shock() {//触发中端
  state++;
  game_switch = 1;//激活计时
}

void openDoor() {//开门
  digitalWrite(MOTO_A, LOW);
  digitalWrite(MOTO_B, HIGH);
  delay(OPENTIME);
  digitalWrite(MOTO_A, LOW);
  digitalWrite(MOTO_B, LOW);
}

void displaySing() {
//  LCDA.CLEAR();
  delay(100);
//  LCDA.DisplayString(0, 2, SystemArr[2], AR_SIZE(SystemArr[2]));
  delay(100);
//  LCDA.DisplayString(2, 2, SystemNameArr[Sing_flag], AR_SIZE(SingNameArr[Sing_flag]));
  delay(100);
}
