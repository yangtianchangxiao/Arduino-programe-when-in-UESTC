#include<Servo.h>
#include <Wire.h>
#include <JY901.h>
Servo servo_forward;
Servo servo_back;
float GyroZ;
float AngleZ_now=0;
float AngleZ_last=0;
float AngleZ;
float AngleY;
void ReadAngle();
float value_forward;
void setup() {
  // put your setup code here, to run once:
    servo_back.attach(9);
    servo_forward.attach(10);
    Serial.begin(115200);
}

void loop()
{
  ReadAngle();
  if(AngleY>1)
  {
    AngleZ_now=0.98*(AngleZ_last+GyroZ*0.11)+0.02*AngleZ;
    AngleZ_last=AngleZ_now;
    value_forward=(35-AngleZ_now);
    servo_forward.write(value_forward);
    servo_back.write(106);
  }
  else
  {
    servo_back.write(106);  
  }
 
}
void ReadAngle()
{
  AngleY= (float)JY901.stcAngle.Angle[1]/32768*180;
  AngleZ= (float)JY901.stcAngle.Angle[2]/32768*180;
}
void ReadGyro(float GyroX,float GyroY,float GyroZ)
{
  GyroZ=(float)JY901.stcGyro.w[2]/32768*2000;
  }
void serialEvent() 
{
  while (Serial.available()) 
  {
    JY901.CopeSerialData(Serial.read()); //Call JY901 data cope function
  }
}
