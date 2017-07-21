i#include <IRremote.h>
#include <Servo.h>
#define AR_SIZE(a) sizeof(a) / sizeof(a[0])
#define SersorLED 13
#define SensorINPUT 2
//#define MOTO_A 3
//#define MOTO_B 4  //电机驱动
//#define OPENTIME 500  //开门电机转动时间ms
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
int Sing_flag = 1;  //当前歌曲标志
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
int Sum = 0;
int IR_PIN = 11;

IRrecv irDetect(IR_PIN);
decode_results irIn;

Servo myservo;

void setup() {
  Serial.begin(9600);
  pinMode(SersorLED, OUTPUT);
  irDetect.enableIRIn(); // Start the Receiver
  pinMode(SensorINPUT, INPUT);
  attachInterrupt(0, shock, FALLING);
}

void loop() {

  if (state != 0)//红外感应器
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

      //      Sing_flag++;//换歌
      //      if (Sing_flag > MAXSING - 1) {
      //        Sing_flag = 0;
      //      }
    }
  }


  if (irDetect.decode(&irIn)) {//红外模块
    decodeIR();
    if (Sum > 2) {
      openDoor();
    }
    delay(15);
    //    Serial.println(Sum);
    irDetect.resume();
  }
}

void shock() {//触发中端
  state++;
  game_switch = 1;//激活计时
}

void openDoor() {
  myservo.attach(9);  // 该舵机由 arduino 第九脚控制
  myservo.write(60);
  delay(8000);
  myservo.detach();
  delay(8000);
  myservo.attach(9);  // 该舵机由 arduino 第九脚控制
  myservo.write(120);
  delay(10500);
  //      Serial.println("after");
  myservo.detach();
  //      Serial.println("late");
  Sum = 0;
}

void decodeIR() // Indicate what key is pressed
{
  switch (irIn.value)
  {
    case 0xFF629D:
      Serial.println("Up Arrow");
      Sum = 0;
      break;

    case 0xFF22DD:
      Serial.println("Left Arrow");
      Sum = 0;
      break;

    case 0xFF02FD:
      Serial.println("OK");
      Sum = 0;
      break;

    case 0xFFC23D:
      Serial.println("Right Arrow");
      Sum = 0;
      break;

    case 0xFFA857:
      Serial.println("Down Arrow");
      Sum = 0;
      break;

    case 0xFF6897:
      Serial.println("1");
      Sum = 0;
      break;

    case 0xFF9867:
      Serial.println("2");
      Sum = 0;
      break;

    case 0xFFB04F:
      Serial.println("3");
      Sum = 0;
      break;

    case 0xFF30CF:
      Serial.println("4");
      Sum = 0;
      break;

    case 0xFF18E7:
      Serial.println("5");
      if (Sum > -1) {
        Sum = 1;
      }
      else {
        Sum = 0;
      }
      break;

    case 0xFF7A85:
      Serial.println("6");
      Sum = 0;
      break;

    case 0xFF10EF:
      Serial.println("7");
      Sum = 0;
      break;

    case 0xFF38C7:
      Serial.println("8");
      if (Sum > 1) {
        Sum = 3;
      }
      else {
        Sum = 0;
      }
      break;

    case 0xFF5AA5:
      Serial.println("9");
      Sum = 0;
      break;

    case 0xFF42BD:
      Serial.println("*");
      Sum = 0;
      break;

    case 0xFF4AB5:
      Serial.println("0");
      if (Sum > 0) {
        Sum = 2;
      }
      else {
        Sum = 0;
      }
      break;

    case 0xFF52AD:
      Serial.println("#");
      Sum = 0;
      break;

    default:
      break;
  }
}
