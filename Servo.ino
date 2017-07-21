#include <Servo.h>   
   
Servo servo1;  
  
void setup()   
{  
  Serial.begin(115200);  
  Serial.println(":-]");  
  delay(300);  
}   
  
void loop()   
{  
  if (Serial.available()) {  
    char val = Serial.read();  
    if (val == ',') {  
      servo1.attach(9);  
      servo1.writeMicroseconds(1300);  
    } else if (val == '.') {  
      servo1.attach(9);  
      servo1.writeMicroseconds(1700);  
    } else if (val == '/') {  
      servo1.detach();  
    }  
  }  
}   
