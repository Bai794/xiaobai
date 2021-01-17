#define BLINKER_WIFI
#define BLINKER_MIOT_OUTLET   //小爱同学

#include <Blinker.h>
#include <Arduino.h>
typedef unsigned char uint8_t;
char auth[] = "697378fa8f56";  //设备key
char ssid[] = "bai";  
char pswd[] = "123456bai"; 
BlinkerButton Button1("btn-abc");    
BlinkerButton Button2("btn-abcd"); 
BlinkerSlider Slider1("ran-un1");
BlinkerSlider Slider2("duoji2");
BlinkerSlider Slider3("duoji3");
BlinkerSlider Slider4("duoji4");
BlinkerSlider Slider5("duoji5");
BlinkerSlider Slider6("duoji6");
bool oState = false;
int counter = 0,i=0;
int pwm=100;
unsigned char jiaoduval[8]={0};

void miotPowerState(const String & state)
{
  //Serial.println("1");
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {     //小爱同学控制开命令 此处修改为点动模式，适合按钮操作，
        digitalWrite(D0, LOW);
        BlinkerMIOT.powerState("on");
        Serial.println("#");
        //BlinkerMIOT.print();
        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {   
 
        digitalWrite(D0, HIGH);
        BlinkerMIOT.powerState("off");
          Serial.println("*");
       // BlinkerMIOT.print();
        oState = false;
    }
}

void miotQuery(int32_t queryCode)      //小爱同学控制
{
    //BLINKER_LOG("MIOT Query codes: ", queryCode);
   //Serial.println("3");
   switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
        Serial.println("start");
            //BLINKER_LOG("MIOT Query All");
            //BlinkerMIOT.powerState(oState ? "on" : "off");
            //BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
           // BLINKER_LOG("MIOT Query Power State");
           // BlinkerMIOT.powerState(oState ? "on" : "off");
            //BlinkerMIOT.print();
            break;
        default :
           // BlinkerMIOT.powerState(oState ? "on" : "off");
           // BlinkerMIOT.print();
            break;
    }
}

void dataRead(const String & data)      // 如果未绑定的组件被触发，则会执行其中内容
{
    BLINKER_LOG("Blinker readString: ", data);

    Blinker.vibrate();
    
   uint32_t BlinkerTime = millis();
    
   Blinker.print("millis", BlinkerTime);
}

void button1_callback(const String & state)     //点灯app内控制按键触发
{
    //BLINKER_LOG("get button state: ", state);
      Serial.write("#");
    digitalWrite(D0,LOW);
    //Serial.println("hello");
}
void button2_callback(const String & state)     //点灯app内控制按键触发
{
    //BLINKER_LOG("get button state: ", state);
    digitalWrite(D0, HIGH);
    //Serial.print("helloxiaobai");
    Serial.write("*");
    Serial.println("\n");
}
void slider1_callback(int32_t value)
{
  jiaoduval[1]=value;
  for( i=0;i<8;i++)
    {
      Serial.write(jiaoduval[i]);
    }
  //Serial.println(value);
}
void slider2_callback(int32_t value)
{
  jiaoduval[2]=value;
  for( i=0;i<8;i++)
    {
      Serial.write(jiaoduval[i]);
    }
 //Serial.println(value);
}
void slider3_callback(int32_t value)
{
   jiaoduval[3]=value;
   for( i=0;i<8;i++)
    {
      Serial.write(jiaoduval[i]);
     
    }
 //Serial.println(value);
}
void slider4_callback(int32_t value)
{
  jiaoduval[4]=value;
   for( i=0;i<8;i++)
    {
     Serial.write(jiaoduval[i]);
    }
 //Serial.println(value);
}
void slider5_callback(int32_t value)
{
   jiaoduval[5]=value;
    for(i=0;i<8;i++)
    {
     Serial.write(jiaoduval[i]);
    
    }
 //Serial.println(value);
}
void slider6_callback(int32_t value)
{
   jiaoduval[6]=value;
    for( i=0;i<8;i++)
    {
      Serial.write(jiaoduval[i]);
    
    }
 //Serial.println(value);
}
void setup()
{
    Serial.begin(115200);
   BLINKER_DEBUG.stream(Serial);
   jiaoduval[0]='$';//zhentou
   jiaoduval[7]='*';//zhenwei
    pinMode(D0, OUTPUT);              //定义io口为输出
    digitalWrite(D0, HIGH);           //定义io默认为高电平

    Blinker.begin(auth, ssid, pswd);
    delay(3000);
    Blinker .attachData(dataRead);
    
    BlinkerMIOT.attachPowerState(miotPowerState);
    BlinkerMIOT.attachQuery(miotQuery);
    Button1.attach(button1_callback);  
    Button2.attach(button2_callback);    
    Slider1.attach(slider1_callback);
    Slider2.attach(slider2_callback);
    Slider3.attach(slider3_callback);
    Slider4.attach(slider4_callback);
    Slider5.attach(slider5_callback);
    Slider6.attach(slider6_callback);
}

void loop()
{ 
    Blinker.run();  
     //delay(1000);
}
