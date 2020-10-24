#include <Servo.h>
Servo claw_servo;
Servo rotate_servo;
int goal_angle=0;
int current_angle=0;

void robot_init(){
  claw_servo.attach(9);
  rotate_servo.attach(10);
  Serial.begin(9600);
  claw_servo.write(120);
  rotate_servo.write(20);
}

void claw_drive(int goal_angle,int current_angle)
{
   int bias=0;
   bias=goal_angle-current_angle;
  
   if(bias>0)
   {
      while(bias>0)
      {
        current_angle++;
        claw_servo.write(current_angle);
        bias=goal_angle-current_angle;
        delay(20);
      }
    }
   if(bias<0)
   {
      while(bias<0)
      {
        current_angle--;
        claw_servo.write(current_angle);
        bias=goal_angle-current_angle;
        delay(20);
      }
   }
}

void rotate_drive(int goal_angle,int current_angle)
{
   int bias=0;
   bias=goal_angle-current_angle;
  
   if(bias>0)
   {
      while(bias>0)
      {
        current_angle++;
        rotate_servo.write(current_angle);
        bias=goal_angle-current_angle;
        delay(20);
      }
    }
   if(bias<0)
   {
      while(bias<0)
      {
        current_angle--;
        rotate_servo.write(current_angle);
        bias=goal_angle-current_angle;
        delay(20);
      }
   }
}
