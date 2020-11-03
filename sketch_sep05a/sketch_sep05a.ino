int key_io=2;//weidong key
int key_io2=8;//touch key
int Vcc=7;
int flag=0;
#define interrupt 1 //Interrupt io 0


void setup() {
  // put your setup code here, to run once:
pinMode(key_io,INPUT);
pinMode(Vcc,OUTPUT);
pinMode(key_io,INPUT);
pinMode(interrupt, INPUT_PULLUP);
attachInterrupt(0,read_key,FALLING);
Serial.begin(9600);

}
void read_key()
{
  flag=1;
}
void loop() {
  // put your main code here, to run repeatedly:
  if(flag==1)
  {
    digitalWrite(Vcc,1);
    Serial.println("hello word");
    delay(100);
    flag=0;
  }
  digitalRead(key_io2);
  
}
