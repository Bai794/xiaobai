#define leftA_PIN 5
#define leftB_PIN 6
#define righA_PIN 9
#define righB_PIN 10
int time = 1200;
float Kp = 12, Ki = 0.05, Kd = 0.05;                 //pid弯道参数参数
float error = 0, P = 0, I = 0, D = 0, PID_value = 0; //pid直道参数
float decide = 0;                                    //元素判断
float previous_error = 0, previous_I = 0;            //误差值
int sensor[5] = {0, 0, 0, 0, 0};                     //
int num = 0;

static int initial_motor_speed = 60; //初始速度

void read_sensor_values(void); //读取初值
void calc_pid(void);           //计算pid
void motor_control(void);      //电机控制

void motor_pinint(); //引脚初始化
void _stop();        //停车
void trun_90(int dir, int time);

//             循迹模块设置               //
#define leftA_track_PIN 2
#define leftB_track_PIN 3
#define middle_track_PIN 4
#define righA_track_PIN 7
#define righB_track_PIN 8

void setup()
{
  Serial.begin(9600); //串口
  track_pinint();     //循迹引脚初始化
  motor_pinint();     //电机引脚初始化
}
void loop()
{
  read_sensor_values(); 
  calc_pid();
  motor_control();
  // switch (num)
  // {
  // case 2:
  //   // trun_90(0, time);
  //   num = 0;
  //   break;

  // }
  // sensor[0] = digitalRead(2);
  // sensor[1] = digitalRead(3);
  // sensor[2] = digitalRead(4);
  // sensor[3] = digitalRead(5);
  // for(int i=0;i<4;i++)
  // {
  // Serial.print(sensor[i]);
  // Serial.print(" ");
  // }
  // // Serial.println();
  // motorsWrite(50, 50);
}
void trun_90(int dir, int time)
{
  analogWrite(leftA_PIN, 60);
  analogWrite(leftB_PIN, 0);
  analogWrite(righA_PIN, 60);
  analogWrite(righB_PIN, 0);
  delay(600);
  if (dir == 0)
  {
    
    analogWrite(leftA_PIN, 150);
    analogWrite(leftB_PIN, 0);
    analogWrite(righA_PIN, 0);
    analogWrite(righB_PIN, -150);
  }
  else
  {
    analogWrite(leftA_PIN, 0);
    analogWrite(leftB_PIN, -150);
    analogWrite(righA_PIN, 150);
    analogWrite(righB_PIN, 0);
  }
  delay(time);
  _stop();
}
void track_pinint()
{
  pinMode(leftA_track_PIN, INPUT); //设置引脚为输入引脚
  pinMode(leftB_track_PIN, INPUT);
  pinMode(middle_track_PIN, INPUT);
  pinMode(righA_track_PIN, INPUT);
  pinMode(righB_track_PIN, INPUT);
}

void motor_pinint()
{
  pinMode(leftA_PIN, OUTPUT);
  pinMode(leftB_PIN, OUTPUT);
  pinMode(righA_PIN, OUTPUT);
  pinMode(righB_PIN, OUTPUT);
}

void _stop()
{
  analogWrite(leftA_PIN, 0);
  analogWrite(leftB_PIN, 0); //左轮静止不动
  analogWrite(righA_PIN, 0);
  analogWrite(righB_PIN, 0); //右轮静止不动
}
//速度范围(-255,255)
void motorsWrite(int speedL, int speedR)
{
  // Serial.println(speedL);
  if (speedL > 0)
  {
    analogWrite(leftA_PIN, speedL);
    analogWrite(leftB_PIN, 0);
  }
  else
  {
    analogWrite(leftA_PIN, 0);
    analogWrite(leftB_PIN, -speedL);
  }

  if (speedR > 0)
  {
    analogWrite(righA_PIN, speedR);
    analogWrite(righB_PIN, 0);
  }
  else
  {
    analogWrite(righA_PIN, 0);
    analogWrite(righB_PIN, -speedR);
  }
}
//速度设定范围(-100，100)
void motorsWritePct(int speedLpct, int speedRpct)
{
  //speedLpct, speedRpct ranges from -100 to 100
  motorsWrite(speedLpct * 2.55, speedRpct * 2.55);
}
void motorsStop()
{
  analogWrite(leftA_PIN, 0);
  analogWrite(leftB_PIN, 0); //左轮静止不动
  analogWrite(righA_PIN, 0);
  analogWrite(righB_PIN, 0); //右轮静止不动
}

void read_sensor_values()
{
  sensor[0] = digitalRead(leftA_track_PIN);
  sensor[1] = digitalRead(leftB_track_PIN);
  sensor[2] = digitalRead(middle_track_PIN);
  sensor[3] = digitalRead(righA_track_PIN);
  //   sensor[4] = digitalRead(righB_track_PIN);

  if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1))
  {
    decide = 1; //十字路口 1 1 1 1   直行
    error = 0;
    num++;
  }
  else if ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0))
  {
    decide = 0; //十字路口 0 1 1 0  直行
    error = 0;
  }
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 0))
  {
    decide = 1; //90度路口 0 0 1 0    右转90度
    error = 1;
  }
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 1))
  {
    decide = 1; //90度路口 0 0 1 1   右转90度
    error = 1;
  }
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1))
  {
    decide = 2; //90度路口 0 0 0 1    右转90度
    error = 2;
  }
  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0))
  {
    decide = 3; //90度路口 1 1 0 0    左转90度
    error = -1;
  }
  else if ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0))
  {
    decide = 3; //90度路口 0 1 0 0    左转90度
    error = -1;
  }
  else if ((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0))
  {
    error = -2;
    decide = 3; //90度路口 1000    左转90度
  }
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0))
  {

  }
 
}
  void calc_pid()
  {
    P = error;
    I = I + error;
    D = error - previous_error;

    PID_value = (Kp * P) + (Ki * I) + (Kd * D);

    previous_error = error;
  }
  void motor_control()
  {
    int left_motor_speed = initial_motor_speed + PID_value + 8;
    int right_motor_speed = initial_motor_speed - PID_value;

    if (left_motor_speed < -255)
    {
      left_motor_speed = -255;
    }
    if (left_motor_speed > 255)
    {
      left_motor_speed = 255;
    }
    if (right_motor_speed < -255)
    {
      right_motor_speed = -255;
    }
    if (right_motor_speed > 255)
    {
      right_motor_speed = 255;
    }
    motorsWrite(left_motor_speed, right_motor_speed);

    // Serial.print("move_A: ");
    // Serial.print(left_motor_speed, DEC);
    //  Serial.println();
    // // Serial.print(" move_B: ");
    // Serial.print(right_motor_speed, DEC);

    Serial.print(" error: ");
    Serial.print(error, DEC);
    Serial.println();
  }
