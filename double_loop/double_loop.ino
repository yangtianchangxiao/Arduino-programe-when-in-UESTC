#define cyclePlus 11
#define jiansu 20
//经计算得，一共需要转动220个脉冲，即90°
int coderA = 2;
int coderB = 3;
int num = 0;

int AB1 = 9;
int AB2 = 10;
float loc_kp1 = 2;
float loc_kd1 = 0.2;
int vel_kp1 = 0.5;
int vel_kd1 =0 ;
int vel_ki1 =10 ;


float loc_kp2 = 2;
float loc_kd2 = 0.4;
int vel_kp2 = 0.5;
int vel_kd2 =0 ;
int vel_ki2=9 ;


int value = 0;
int x;
int k = 0;
int counter_tims = 0;
long int interval = 0;
long int a = 0;
long int temp = 0;

int bond_l=30;
int bond_v=20;

int bond_l2=20;
int bond_v2=15;

void setup() {
  // put your setup code here, to run once:
  pinMode(coderA, INPUT);
  pinMode(coderB, INPUT);
  pinMode(AB1, OUTPUT);
  pinMode(AB2, OUTPUT);
  attachInterrupt(0, read_coder, CHANGE);
  attachInterrupt(1, read_coder, CHANGE);
  //attachInterrupt(1,read_coder,CHANGE);
  num = 0; //需要将脉冲数与角度对应起来
  Serial.begin(9600);
}

void read_coder()
{
   num = num + value;
   //interval may have a relative bias if just ultilizing the current one.
}

void pid1(int goal_plus)
{
  
  struct pid_init
  {
    float err = 0;
    float output = 0;
    float err_last = 0;
    float err_last2 = 0;
  };
  struct pid_init loc_pid;
  struct pid_init vel_pid;
  float velocity = 0;
  int temp_x=num;
  int temp_time=micros();

  
  float m=0;
  float u=0;
  int flag=1;
  
  while (1)
  {
    loc_pid.err = goal_plus - num;

    if (num > 200)
    {
        analogWrite(AB2, 255);
        analogWrite(AB1, 255);
      break;
    }
    if (loc_pid.err == loc_pid.err_last)
    {
      //Serial.println("break");
      break;
    }
    loc_pid.output = loc_kp1 * loc_pid.err + loc_kd1 * (loc_pid.err - loc_pid.err_last);    
    
    loc_pid.err_last = loc_pid.err;
    if (loc_pid.output > bond_l)
    {
      loc_pid.output = bond_l;
    }

   // Serial.println("loc_pid,output");
    
    
    velocity = (x-temp_x)/110*3.14159/ (micros()-temp_time) *1000000;
       
    vel_pid.err = (loc_pid.output /1.8/bond_l - velocity);
    m = vel_kp1 * (vel_pid.err - vel_pid.err_last) +vel_ki1 * vel_pid.err+ vel_kd1 * (vel_pid.err + vel_pid.err_last2 - 2 * vel_pid.err_last);
      
       //Serial.println(vel_pid.err - vel_pid.err_last);
       //Serial.println(vel_pid.err); 
      //Serial.println(vel_pid.err + vel_pid.err_last2 - 2 * vel_pid.err_last);
    vel_pid.output = u + m;
    vel_pid.err_last = vel_pid.err;
    vel_pid.err_last2 = vel_pid.err_last;
    if (vel_pid.output > bond_v)
    {
      vel_pid.output = bond_v;
    }
    if (vel_pid.output<4)
    {
      vel_pid.output=4;
    }
    
    if(((num-temp_x)==0)and(flag==1))
    {
       analogWrite(AB1, 225-61);
       analogWrite(AB2, 255);
       delay(80);
    }
       flag=0;
      drive(255 - int(vel_pid.output)-21, goal_plus);
   Serial.println(vel_pid.output);
     u = vel_pid.output;
  }
  
  temp_x=num;
  temp_time=micros();
}



void pid2(int goal_plus)
{
  struct pid_init2
  {
    float err = 0;
    float output = 0;
    float err_last = 0;
    float err_last2 = 0;
  };
  struct pid_init2 loc_pid2;
  struct pid_init2 vel_pid2;
  float velocity = 0;
  int temp_x=num;
  int temp_time=micros();

  
  float m=0;
  float u=0;
  int flag=1;
  
  while (1)
  {
    loc_pid2.err = num;

    if (num < 20)
    {
        analogWrite(AB2, 255);
        analogWrite(AB1, 255);
      break;
    }
  
     if (loc_pid2.err == loc_pid2.err_last)
    {
      Serial.println("break");
      break;
    }
    loc_pid2.output = loc_kp2 * loc_pid2.err + loc_kd2 * (loc_pid2.err - loc_pid2.err_last);    
    
    loc_pid2.err_last = loc_pid2.err;
    if (loc_pid2.output > bond_l2)
    {
      loc_pid2.output = bond_l2;
    }

   // Serial.println("loc_pid,output");
    
    
    velocity = (temp_x-num)/110*3.14159/ (micros()-temp_time) *1000000;
       
    vel_pid2.err = (loc_pid2.output /2/bond_l2 - velocity);
    m = vel_kp2 * (vel_pid2.err - vel_pid2.err_last) +vel_ki2 * vel_pid2.err+ vel_kd2 * (vel_pid2.err + vel_pid2.err_last2 - 2 * vel_pid2.err_last);
      
       //Serial.println(vel_pid.err - vel_pid.err_last);
       //Serial.println(vel_pid.err); 
      //Serial.println(vel_pid.err + vel_pid.err_last2 - 2 * vel_pid.err_last);
    vel_pid2.output = u + m;
    vel_pid2.err_last = vel_pid2.err;
    vel_pid2.err_last2 = vel_pid2.err_last;
    if (vel_pid2.output > bond_v2)
    {
      vel_pid2.output = bond_v2;
    }
    if (vel_pid2.output<1)
    {
      vel_pid2.output=1;
    }

    
    if(((temp_x-num)==0)and(flag==1))
    {
      
       analogWrite(AB1, 255);
       analogWrite(AB2, 255-91);
       Serial.println("zhixing");
       delay(100);
    }
    flag=0;

    value = -1;
    analogWrite(AB2, 255 - int(vel_pid2.output)-16);
    analogWrite(AB1, 255);
    delay(50);
      

    
    //drive(255 - int(vel_pid2.output)-23, goal_plus);

   
     u = vel_pid2.output;
  }
  
  temp_x=num;
  temp_time=micros();
}

void drive(int dutycycle, int goal_plus)
{
  if (goal_plus == 220)
  {
    value = 1;
    analogWrite(AB1, dutycycle);
    analogWrite(AB2, 255);
    delay(50);
  }
  else {
    value = -1;
    analogWrite(AB2, dutycycle);
    analogWrite(AB1, 255);
    delay(50);
  }
}


void loop() {
  // put your main code here, to run repeatedly:


  pid1(220);
  analogWrite(AB1, 255);
  Serial.println(num);
  delay(5000);
  pid2(0);
  analogWrite(AB2, 255);
  Serial.println("anti");
  Serial.println(num);
  delay(5000);

  
  
  

}
