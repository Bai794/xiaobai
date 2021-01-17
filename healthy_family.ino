#include <ESP8266WiFi.h>
//定义最多多少个client可以连接本server(一般不要超过4个)
#define MAX_SRV_CLIENTS 1
//以下三个定义为调试定义
#define DebugBegin(baud_rate)    Serial.begin(baud_rate)
#define DebugPrintln(message)    Serial.println(message)
#define DebugPrint(message)    Serial.print(message)
char ssid[]= "ke";
char password[] = "kemengxinkemengxin233";
String temp="";
//创建server 端口号是23
WiFiServer server(25000);
//管理clients
WiFiClient serverClients[MAX_SRV_CLIENTS];

void setup() {
  DebugBegin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  DebugPrint("\nConnecting to ");
  DebugPrintln(ssid);
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) {
    delay(500);
  }
  if (i == 21) {
    DebugPrint("Could not connect to");
    DebugPrintln(ssid);
    while (1) {
      delay(500);
    }
  }
  //启动server
  server.begin();
  //关闭小包合并包功能，不会延时发送数据
  server.setNoDelay(true);
  DebugPrint("Ready! Use 'telnet ");
  //DebugPrint("&");
  DebugPrint(WiFi.localIP());
  //DebugPrint("$"); 
  DebugPrintln(" 25000' to connect");
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  delay(5000);
  Serial.write('&');
  DebugPrint(WiFi.localIP());
  Serial.write('$');
}

void loop() {
  uint8_t i;
  unsigned char number_client;
  struct station_info *stat_info;
  
  //检测是否有新的client请求进来
  if (server.hasClient()) {
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      //释放旧无效或者断开的client
      if (!serverClients[i] || !serverClients[i].connected()) {
        if (serverClients[i]) {
          serverClients[i].stop();
        }
        //分配最新的client
        serverClients[i] = server.available();

        DebugPrint("New client: ");
        DebugPrint(i + 1);

        break;
      }
    }
    //当达到最大连接数 无法释放无效的client，需要拒绝连接
    if (i == MAX_SRV_CLIENTS) {
      WiFiClient serverClient = server.available();
      serverClient.stop();
      DebugPrintln("Connection rejected ");
    }
  }
  //检测client发过来的数据
  for (i = 0; i < MAX_SRV_CLIENTS; i++) {
    if (serverClients[i] && serverClients[i].connected()) {
      if (serverClients[i].available()) {
        //get data from the telnet client and push it to the UART
        while (serverClients[i].available()) {
          //发送到串口调试器
          Serial.write(serverClients[i].read());
        }
      }
    }
  }

  if (Serial.available()) {
    //把串口调试器发过来的数据 发送给client
    size_t len = Serial.available();
    uint8_t sbuf[len];
    Serial.readBytes(sbuf, len);
//     temp = Serial.readString();
    //push UART data to all connected telnet clients
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      if (serverClients[i] && serverClients[i].connected()) {
        serverClients[i].write(sbuf, len);
        delay(5);
      }
    }
  }
}
