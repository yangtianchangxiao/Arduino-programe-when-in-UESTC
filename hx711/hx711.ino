uint8_t HX_SCK = 3;
uint8_t HX_DT = 4;
long count0;
long count_avg=0;
int num=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(HX_SCK, OUTPUT);
  pinMode(HX_DT, INPUT);
  ReadCount();
  delay(100);
  count0 = 0;
  for(int i=0; i<8; i++)
    count0 += ReadCount();
  count0 /= 8;
  Serial.print(count0);
}

void loop() {
  // put your main code here, to run repeatedly:
  long count = ReadCount() - count0;
  
  count_avg=count_avg+count;
  num++;
  if(num>9)
  {
    count_avg=count_avg/10;
    Serial.println(count_avg/2500*6);
    num=0;
  }
  
  delay(10);
}

long ReadCount()
{
  digitalWrite(HX_SCK, LOW);
  while(digitalRead(HX_DT));
  unsigned long count = 0;
  for(int i=0; i<24; i++)
  {
    digitalWrite(HX_SCK, HIGH);
    count <<= 1;
    digitalWrite(HX_SCK, LOW);
    if(digitalRead(HX_DT)) count |= 1;
  }
  digitalWrite(HX_SCK, HIGH);
  if(count & 0x00800000) count |= 0xFF000000;
  digitalWrite(HX_SCK, LOW);
  return (long)count;
}
