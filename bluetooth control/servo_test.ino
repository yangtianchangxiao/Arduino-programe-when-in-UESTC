#include <Servo.h>
Servo myservo;
int goal_angle=0;
int current_angle=0;
int in1=8;
int in2=13;
void setup() {
myservo.attach(9);
Serial.begin(9600);
myservo.write(0);
pinMode(in1,OUTPUT);
pinMode(in2,OUTPUT);

}


void loop() {


 while(Serial.available())
  {
    char c=Serial.read();
    if(c=='2')
    {
    
      servo_drive(0,90);
      delay(500);
      servo_drive(120,0);
      delay(500);
      
      Serial.println("ok");
    }
    if(c=='1')
    {
      servo_drive(90,0);
      delay(500);
      motor_drive(0);
      Serial.println("ok");
      
    }
    if(c=='3')
    {
       Serial.println("3ok");
    }
  }

}

void motor_drive(int flag)
{
  if(flag==1)//wiseclock
  {
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    delay(3000);
    digitalWrite(in1,LOW);
  }
    if(flag==0)
  {
    digitalWrite(in2,HIGH);
    digitalWrite(in1,LOW);
    delay(4000);
    digitalWrite(in2,LOW);
  }
  
}
void servo_drive(int goal_angle,int current_angle)
{
   int bias=0;
   bias=goal_angle-current_angle;
  
   if(bias>0)
   {
      while(bias>0)
      {
        current_angle++;
        myservo.write(current_angle);
        bias=goal_angle-current_angle;
        delay(10);
      }
    }
   if(bias<0)
   {
      while(bias<0)
      {
        current_angle--;
        myservo.write(current_angle);
        bias=goal_angle-current_angle;
        delay(10);
      }
   }
}
