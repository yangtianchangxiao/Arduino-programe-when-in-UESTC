int AB2=7;
int AB1=8;
int EN=11;
int flag=0;
int cycleDuty=0;
#define key 2 //weidong key
void setup() {
  // put your setup code here, to run once:
pinMode(AB1,OUTPUT);
pinMode(AB2,OUTPUT);
pinMode(EN,OUTPUT);
pinMode(key, INPUT_PULLUP);
attachInterrupt(0,read_key,FALLING);
Serial.begin(9600);
}

void read_key()
{
  flag=1;
}
void loop() {
  // put your main code here, to run repeatedly:
  if (flag==1)
  {
    flag=0;
    cycleDuty=cycleDuty+256/4-3;
    if(cycleDuty>255)
    {
      cycleDuty=0;
    }
    analogWrite(EN,cycleDuty);
    delay(100);
     
  }
  

}
