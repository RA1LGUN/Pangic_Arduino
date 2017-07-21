#include <IRremote.h>
#include <Servo.h>
#define SensorINPUT 2

int Sum = 0;
int IR_PIN = 11;
int state = 0;
int state_time = 0;
int sum_time = 0;


IRrecv irDetect(IR_PIN);
decode_results irIn;

Servo myservo;

void setup() {
  Serial.begin(9600);
  irDetect.enableIRIn(); // Start the Receiver
  attachInterrupt(0, shock, FALLING);
}

void loop() {
  if (state != 0)
  {
    state = 0;
    if (state_time > 2) {
      Open();
      state_time = 0;
      delay(500);
    } else {
      state_time = state_time + 1;
      delay(500);
    }
  }
  if (state_time != 0) {
    sum_time = sum_time + 1;
    if (sum_time > 10000) {
      state_time = 0;
    }
  }
  if (irDetect.decode(&irIn)) {
    decodeIR();
    if (Sum > 2) {
      Open();
    }
    delay(15);
    //    Serial.println(Sum);
    irDetect.resume();
  }

}

void Open() {
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


void shock() {//触发中端
  state++;
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
