#define cyclePlus 11
#define jiansu 20
//经计算得，一共需要转动224个脉冲，即90°，取60
int coderA=2;
int coderB=3;
int num=0;

int AB1=9;
int AB2=10;
float kp=0.72;
float kd=0.1;
int value=0;
int x;
int k=0;

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
void setup() {
  // put your setup code here, to run once:
pinMode(coderA,INPUT);
pinMode(coderB,INPUT);
pinMode(AB1,OUTPUT);
pinMode(AB2,OUTPUT);
attachInterrupt(0,read_coder,CHANGE);
//attachInterrupt(1,read_coder,CHANGE);
num=0;//需要将脉冲数与角度对应起来
Serial.begin(9600);
}

void read_coder()
{
  num=num+value;
  x=num; 
}

void pid1(int goal_plus)
{
  int err=0;
  int output=0;
  int err_last=0;
  int times=0;
  while (1)
  {
    err=goal_plus-x;
    if(x>222)
    {
      break;
    } 
    if(err==err_last)
    {
      break;
    } 
    output=int(255-(kp*err+kd*(err-err_last)));
    err_last=err;
    if(output<0)
    {
      output=0;
    }
     drive(output,goal_plus);    
    }  
}

void pid2(int goal_plus)
{
  int err=0;
  int output=0;
  int err_last=0;
  int times=0;
  while (1)
  {
    err=x;
    if(x<2)
    {
      break;
    } 
    if(err==err_last)
    {
      break;
    }
   
    output=int(255-(kp*err+kd*(err-err_last)));
    err_last=err;
    if(output<0)
    {
      output=0;
    }
     drive(output,goal_plus);    
    }  
}
void drive(int dutycycle,int goal_plus)
{
  if (goal_plus==224)
  {
    value=1;
    analogWrite(AB1,dutycycle);
    analogWrite(AB2,255);
    delay(100);
  }
  else {
    value=-1;
    analogWrite(AB2,dutycycle);
    analogWrite(AB1,255);
    delay(100);    
  }
}
void loop() {
  // put your main code here, to run repeatedly:


  pid1(224); 
        analogWrite(AB2,255);
      analogWrite(AB1,255);
  delay(500);
  pid2(0); 
        analogWrite(AB2,255);
      analogWrite(AB1,255);
  delay(500);
  
  

    
}
