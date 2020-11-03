int coderA=2;
int num=0;
void setup() {
  // put your setup code here, to run once:
pinMode(coderA,INPUT);
Serial.begin(9600);
attachInterrupt(0,read_coder,FALLING);
}

void read_coder()
{
  num=num+1;
  Serial.print(num);
}
void loop() {
  // put your main code here, to run repeatedly:

}
