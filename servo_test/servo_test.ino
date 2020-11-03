#include <Servo.h>
Servo myservo;
int goal_angle=0;
int current_angle=0;
//int enable=10;
int in1=8;
int in2=13;
void setup() {
  // put your setup code here, to run once:
myservo.attach(9);
Serial.begin(9600);
myservo.write(0);
pinMode(in1,OUTPUT);
pinMode(in2,OUTPUT);
//pinMode(enable,OUTPUT);
//analogWrite(enable,0);
}


void loop() {
  // put your main code here, to run repeatedly:
 // servo_drive(10,100);
 // servo_drive(100,10);
 //   analogWrite(enable,100);
 //   digitalWrite(in1,HIGH);
 //   digitalWrite(in2,LOW)

//  while(Serial.available())
 // {
  //  char c=Serial.read();
  /*  if(c=='2')
    {*/
    
      servo_drive(0,120);
      delay(500);
      servo_drive(120,0);
      //motor_drive(1);
      delay(500);
      
      Serial.println("ok");
    /*}*/  
    /*if(c=='1')
    {
      //servo_drive(90,0);
      delay(500);
      motor_drive(0);
      Serial.println("ok");
      
    }
    if(c=='3')
    {
       Serial.println("3ok");
    }*/
//  }

}

void motor_drive(int flag)
{
  if(flag==1)//zheng
  {
   // analogWrite(enable,100);
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    delay(3000);
   // analogWrite(enable,0);
    digitalWrite(in1,LOW);
  }
    if(flag==0)
  {
   // analogWrite(enable,100);
    digitalWrite(in2,HIGH);
    digitalWrite(in1,LOW);
    delay(4000);
  //  analogWrite(enable,0);
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
