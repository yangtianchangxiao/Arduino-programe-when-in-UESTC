#include <Wire.h>
#define I2C_ADDR                    0x0f   //语音识别模块地址

#define ASR_ADD_WORD_ADDR           0x01   //词条添加地址

#define ASR_MODE_ADDR               0x01   //识别模式设置地址，值为0-2，0:循环识别模式 1:口令模式 ,2:按键模式，默认为循环检测

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
    pinMode(7,OUTPUT);//风扇输出端口
/***************************************************************************************
录入词条和模式可以掉电保存，录入一次后下次如果无需修改可以将1设置为0，为0时这段程序不折行
****************************************************************************************/

#if 1
    I2CWrite(ASR_CLEAR_ADDR,0x40);//清除掉电保存区,录入前需要清除掉电保存区
    delay(5000);//清楚掉电保存区需要延时一段时间
    I2CWrite(ASR_MODE_ADDR,0);//设置检测模式               
    AsrAddWords(1,"hong deng");   
    AsrAddWords(2,"lv deng");
    AsrAddWords(3,"lan deng");
    AsrAddWords(4,"guan deng");
    AsrAddWords(5,"dao jiu");
    AsrAddWords(6,"hao le");
    while(cleck != 6)
    {
      WireReadData(ASR_NUM_CLECK,&cleck,1);
      Serial.println(cleck);
      delay(100);
      }     
#endif

    
    I2CWrite(ASR_REC_GAIN,0x40);  //识别的灵敏度，建议0x40-0x55
    I2CWrite(ASR_VOICE_FLAG,1);  //识别结果提示音开关设置
    RGB_Set(255,255,255);//设置模块的RGB灯为白色

    robot_init();
          
}

void loop() {
        unsigned char result;
        WireReadData(ASR_RESULT,&result,1);//读取识别序号值，并赋值给result，默认是0xff
        delay(100);
        Serial.println(result);
  
        if(result == 5)//判断读取的识别序号值是否是开启风扇
        {
          claw_drive(0,120);
        }
        else if(result == 6)//判断读取的识别序号值是否是关闭风扇
        {
            //digitalWrite(7,0);
            Serial.print("okok");
        }

}
