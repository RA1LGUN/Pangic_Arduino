#include <IRremote.h>
//#include <Servo.h>
//Servo myservo;  // 创建一个舵机控制对象

int IR_PIN = 11;
int pos = 0;
int Sum = 0;
int pulsewidth;

IRrecv irDetect(IR_PIN);

decode_results irIn;

void setup()
{
  Serial.begin(9600);
  //myservo.attach(9);  // 该舵机由 arduino 第九脚控制
  irDetect.enableIRIn(); // Start the Receiver
}

void loop() {
  if (irDetect.decode(&irIn)) {
    decodeIR();
    if (Sum > 2) {
      for (int i = 0; i <= 50; i++ ) {
        servopulse(9, 60);
      }
      
      delay(4000);
      for (int i = 0; i <= 50; i++ ) {
        servopulse(9, 120);
      }
      Serial.println("enter the loop");
      delay(4000);
      Sum = 0;
    }
    Serial.println(Sum);
    delay(15);
    Serial.println("================================\n");
    irDetect.resume();
  }
}


void servopulse(int servopin, int myangle) //定义一个脉冲函数
{

  pulsewidth = (myangle * 11) + 500; //将角度转化为500-2480 的脉宽值

  digitalWrite(servopin, HIGH); //将舵机接口电平至高

  delayMicroseconds(pulsewidth);//延时脉宽值的微秒数

  digitalWrite(servopin, LOW); //将舵机接口电平至低

  delay(20 - pulsewidth / 1000);

  //Serial.println("enter the function");


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
      if (Sum > 1) {
        Sum = 3;
      }
      else {
        Sum = 0;
      }
      break;

    case 0xFF30CF:
      Serial.println("4");
      Sum = 0;
      break;

    case 0xFF18E7:
      Serial.println("5");
      if (Sum > 0) {
        Sum = 2;
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
      if (Sum > -1) {
        Sum = 1;
      }
      else {
        Sum = 0;
      }
      break;

    case 0xFF38C7:
      Serial.println("8");
      Sum = 0;
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
      Sum = 0;
      break;

    case 0xFF52AD:
      Serial.println("#");
      Sum = 0;
      break;

    default:
      break;
  }
}
