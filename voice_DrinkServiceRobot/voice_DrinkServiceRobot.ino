/*本程序使用HX711重量传感器、语音模块，完成倒酒机器人的开发*/
/*机器人的功能包括：语音控制倒酒过程、喝的酒的量，如果一次倒酒超过一辆就立刻停止倒酒*/

#include <Wire.h>
#include <Servo.h>
#define I2C_ADDR                    0x0f   //语音识别模块地址

#define ASR_ADD_WORD_ADDR           0x01   //词条添加地址

#define ASR_MODE_ADDR               0x02   //识别模式设置地址，值为0-2，0:循环识别模式 1:口令模式 ,2:按键模式，默认为循环检测

#define ASR_RGB_ADDR                0x03   //RGB灯设置地址,需要发两位，第一个直接为灯号1：蓝 2:红 3：绿 ,
                                           //第二个字节为亮度0-255，数值越大亮度越高
                                           
#define ASR_REC_GAIN                0x04   //识别灵敏度设置地址，灵敏度可设置为0x00-0x55，值越高越容易检测但是越容易误判，
                                           //建议设置值为0x40-0x55,默认值为0x40
                                           
#define ASR_CLEAR_ADDR              0x05   //清除掉电缓存操作地址，录入信息前均要清除下缓存区信息


#define ASR_KEY_FLAG                0x06  //用于按键模式下，设置启动识别模式

#define ASR_VOICE_FLAG              0x07   //用于设置是否开启识别结果提示音

#define ASR_RESULT                  0x08  //识别结果存放地址

#define ASR_BUZZER                  0x09 //蜂鸣器控制写1开启，写0关闭

#define ASR_NUM_CLECK               0x0a //录入词条数目校验

int val;//定义变量val
int servo_goal_angle=0;
int servo_current_angle=0;
Servo claw_servo;
Servo rotate_servo;

uint8_t HX_SCK = 3;
uint8_t HX_DT = 4;
long count0;
 float count_avg=0;
int num=0;
int init_gram=0;
int flag=0;
int gram_flag=0;
/****************************
      寄存器设置函数  
*****************************/
bool I2CWrite(unsigned char reg_addr,unsigned char date)
{
    Wire.beginTransmission(I2C_ADDR);  //发送Device地址
    Wire.write(reg_addr);              //发送要操作的寄存器地址 
    Wire.write(date);                  //发送要设置的值
    if(Wire.endTransmission()!=0)            //发送结束信号
      {
          delay(10);
          return false;
      }
      delay(10);
      return true;  
}

/****************************
      检测值读取函数  
*****************************/

bool WireReadData(unsigned char reg_addr,unsigned char *value,int num)
{   
    Wire.beginTransmission(I2C_ADDR);  //发送Device地址
    Wire.write(reg_addr);              //发送要操作的寄存器地址  
    delay(10);
    if(Wire.endTransmission()!=0)            //发送结束信号
     {
          delay(10);
          return false;
     }
      delay(10);


    Wire.requestFrom(I2C_ADDR, num);

    while(Wire.available())
    {
        char ff = Wire.read();    // receive a byte as character
        *value = ff;
        value++;
        delay(10);
    } 
        
     return true; 
 }

/*****************************
       RGB设置函数
******************************/
bool RGB_Set(unsigned char R,unsigned char G,unsigned char B)
{
      Wire.beginTransmission(I2C_ADDR);  //发送Device地址
      Wire.write(ASR_RGB_ADDR); 
      Wire.write(R);
      Wire.write(G);      
      Wire.write(B);
      if(Wire.endTransmission()!=0)            //发送结束信号
      {
          delay(10);
          return false;
      }
      delay(10);
      return true;
}


/*****************************
       添加词条函数
******************************/
bool AsrAddWords(unsigned char idNum,unsigned char * words)
{
      Wire.beginTransmission(I2C_ADDR);  //发送Device地址
      Wire.write(ASR_ADD_WORD_ADDR);     //发送存放词条寄存器地址
      Wire.write(idNum);                 //发送词条对应的识别号
      Wire.write(words,strlen(words));    
      if(Wire.endTransmission()!=0)            //发送结束信号
      {
          delay(10);
          return false;
      }
      delay(10);
      return true;
}
void setup() {
    unsigned char cleck = 0xff;

    Wire.begin();
    Wire.setClock(100000);
    Serial.begin(115200);  //串口波特率设置，打印数据时串口需要选择和这里一样的波特率
    //pinMode(7,OUTPUT);//风扇输出端口
/***************************************************************************************
录入词条和模式可以掉电保存，录入一次后下次如果无需修改可以将1设置为0，为0时这段程序不折行
****************************************************************************************/

#if 0
    I2CWrite(ASR_CLEAR_ADDR,0x40);//清除掉电保存区,录入前需要清除掉电保存区
    delay(5000);//清楚掉电保存区需要延时一段时间
    I2CWrite(ASR_MODE_ADDR,0);//设置检测模式               
    AsrAddWords(1,"hong deng");   
    AsrAddWords(2,"lv deng");
    AsrAddWords(3,"lan deng");
    AsrAddWords(4,"guan deng");
    AsrAddWords(5,"zheng shang");
    AsrAddWords(6,"dao jiu");
    AsrAddWords(7,"song kai");
    AsrAddWords(8,"hao le");
    AsrAddWords(9,"zhen hao");
    AsrAddWords(10,"lai le");
    AsrAddWords(11,"hai he");
    AsrAddWords(12,"fang song");
    while(cleck != 12)
    {
      WireReadData(ASR_NUM_CLECK,&cleck,1);
    //  Serial.println(cleck);
      delay(100);
      }     
#endif

    
    I2CWrite(ASR_REC_GAIN,0x55);  //识别的灵敏度，建议0x40-0x55
    I2CWrite(ASR_VOICE_FLAG,0);  //识别结果提示音开关设置
    RGB_Set(255,255,255);//设置模块的RGB灯为白色

    hx711_init();
    
    gram_estimate();
    delay(100);
    stable_gram();
    init_gram=count_avg;
    Serial.println("initial gram={}");
    Serial.println(init_gram);
    robot_init();      
}

void loop() {
        unsigned char result;
        WireReadData(ASR_RESULT,&result,1);//读取识别序号值，并赋值给result，默认是0xff
        delay(100);
       // Serial.println(result);
        
        if(result == 5)//判断读取的识别序号值是否是开启风扇
        {
          claw_drive(30,120);
          rotate_servo.write(125);
        }
        else if(result == 6)//判断读取的识别序号值是否是关闭风扇
        {
          claw_servo.write(30);
          rotate_drive(30,125);
          flag=0;
        }
        else if(result ==8)
        {
          rotate_drive(125,30);
        }
        else if(result == 7)
        {
          claw_drive(120,30);
          rotate_servo.write(125);
        }
          gram_estimate();
          if(gram_flag==0)
          {
          init_gram=count_avg;
          gram_flag=1;
          }
          Serial.println(count_avg);
         // Serial.println("50+init_gram");
         // Serial.println(50+init_gram);
          
          if((count_avg>50+init_gram)&&(flag==0))
          {
            delay(100);
            if(count_avg>50+init_gram)
            {
           rotate_drive(125,30); 
           flag=1;
            }
           
          }
          
          
        result=0;
}


/************************/
void robot_init(){
  claw_servo.attach(9);
  rotate_servo.attach(10);
  claw_servo.write(120);
  rotate_servo.write(125);
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
        delay(10);
      }
    }
   if(bias<0)
   {
      while(bias<0)
      {
        current_angle--;
        rotate_servo.write(current_angle);
        bias=goal_angle-current_angle;
        delay(40);
      }
   }
   
}

void hx711_init()
{
  pinMode(HX_SCK, OUTPUT);
  pinMode(HX_DT, INPUT);
  ReadCount();
  delay(100);
  count0 = 0;
  for(int i=0; i<8; i++)
    count0 += ReadCount();
  count0 /= 8;
 // Serial.print(count0);
}


/*测量重量*/
void gram_estimate()
{
  count_avg=0;
  while (true)
  {
    

    count_avg=count_avg+ReadCount() - count0;
    num++;
    if(num>9)
    {
      count_avg=count_avg/10;
      count_avg=count_avg/2500*6;
      num=0;
      break; 
     // Serial.println(count_avg);
    }
    
    delay(10);
  }
  
}

/*使HX711数据稳定后再测量重量*/
void stable_gram()
{
  int i=0;
  int a=0;
  gram_estimate();
  a=count_avg;
  for(;i<5;i++)
  {
    gram_estimate();
    if ((count_avg-1<a<count_avg+1))
    {
      i=0;
    }
    //Serial.println(a);
    Serial.println(count_avg);
  }
}


/*使用重量传感器获取重量，count需要做后续的处理*/
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

 
