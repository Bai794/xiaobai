#include <ESP8266WiFi.h>
#include <ArduinoJson.h>                 //使用JSON-v5版的库

const char* AP_SSID     = "bai";         // XXXXXX -- 使用时请修改为当前你的 wifi ssid
const char* AP_PSK = "12345678bai";        // XXXXXX -- 使用时请修改为当前你的 wifi 密码

WiFiClient client;                       //创建一个网络对象

/*  请求的Json数据格式如下：
   {
      "results": [
          {
              "location": {
                  "id": "WX4FBXXFKE4F",
                  "name": "北京",
                  "country": "CN",
                  "path": "北京,北京,中国",
                  "timezone": "Asia/Shanghai",
                  "timezone_offset": "+08:00"
              },
              "now": {
                  "text": "多云",
                  "code": "4",
                  "temperature": "23"
              },
              "last_update": "2019-10-13T09:51:00+08:00"
          }
      ]
  }
*/

void wifi_start_connect()              //连接WIFI
{
  WiFi.mode(WIFI_STA);                 //设置esp8266 工作模式
  Serial.println("Connecting to ");    //写几句提示
  Serial.println(AP_SSID);
  WiFi.begin(AP_SSID, AP_PSK);         //连接wifi
  WiFi.setAutoConnect(true);
  while (WiFi.status() != WL_CONNECTED) //这个函数是wifi连接状态，返回wifi链接状态
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected IP is");
  Serial.println(WiFi.localIP());
}

void parseUserData(String content)  // Json数据解析并串口打印.可参考https://www.bilibili.com/video/av65322772
{
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + 210;
  DynamicJsonBuffer jsonBuffer(capacity);

  JsonObject& root = jsonBuffer.parseObject(content);

  JsonObject& results_0 = root["results"][0];

  JsonObject& results_0_location = results_0["location"];
  const char* results_0_location_id = results_0_location["id"];
  const char* results_0_location_name = results_0_location["name"];
  const char* results_0_location_country = results_0_location["country"];
  const char* results_0_location_path = results_0_location["path"];
  const char* results_0_location_timezone = results_0_location["timezone"];
  const char* results_0_location_timezone_offset = results_0_location["timezone_offset"];

  JsonObject& results_0_now = results_0["now"];
  const char* results_0_now_text = results_0_now["text"];
  const char* results_0_now_code = results_0_now["code"];
  const char* results_0_now_temperature = results_0_now["temperature"];
  const char* results_0_last_update = results_0["last_update"];

  Serial.println(results_0_location_name);                       //通过串口打印出需要的信息
  Serial.println(results_0_now_text);
  Serial.println(results_0_now_code);
  Serial.println(results_0_now_temperature);
  Serial.println(results_0_last_update);
  Serial.print("\r\n");
}

void setup()
{
  Serial.begin(115200);
  wifi_start_connect();
  client.setTimeout(5000);//设置服务器连接超时时间
}

void loop()
{
  if (client.connect("api.seniverse.com", 80) == 1)           //连接服务器并判断是否连接成功，若成功就发送GET 请求数据下发
  { //换成你自己在心知天气申请的私钥//改成你所在城市的拼音
    client.print("GET /v3/weather/now.json?key=“SUi3UDcZSHNKL28nF”&location=chongqing&language=zh-Hans&unit=c HTTP/1.1\r\n"); //心知天气的URL格式
    client.print("Host:api.seniverse.com\r\n");
    client.print("Accept-Language:zh-cn\r\n");
    client.print("Connection:close\r\n\r\n"); //向心知天气的服务器发送请求。


    String status_code = client.readStringUntil('\r');        //读取GET数据，服务器返回的状态码，若成功则返回状态码200
    Serial.println(status_code);

    if (client.find("\r\n\r\n") == 1)                         //跳过返回的数据头，直接读取后面的JSON数据，
    {
      String json_from_server = client.readStringUntil('\n'); //读取返回的JSON数据
      Serial.println(json_from_server);
      parseUserData(json_from_server);                      //将读取的JSON数据，传送到JSON解析函数中进行显示。
    }
  }
  else
  {
    Serial.println("connection failed this time");
    delay(5000);                                            //请求失败等5秒
  }

  client.stop();                                            //关闭HTTP客户端，采用HTTP短链接，数据请求完毕后要客户端要主动断开https://blog.csdn.net/huangjin0507/article/details/52396580
  delay(5000);
}
