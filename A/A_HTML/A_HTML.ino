#include <WiFi.h>
#include <Wire.h>
#include <MPU6050_tockn.h>
#include <ESP32Servo.h>
#include <ssd1306.h>
#include <ssd1306_console.h>
Ssd1306Console OLED1;
MPU6050 zt(Wire);
Servo m1_a;
Servo m1_b;
Servo m2_a;
Servo m2_b;
int m1 = 2;
int m2 = 4;
int m3 = 26;
int m4 = 27;
int aj_PIN = 0;
char a[20];

WiFiServer server(80);
WiFiClient myclient;
String potVal="";
String potVal2="";
String potVal3="";
String readLine="";
void setup() {
  Serial.begin(115200);
  WiFi.softAP("EPS32多功能演示仪A主板","58565858");
  Serial.println(WiFi.softAPIP());
  server.begin();
  Wire.begin();
  ssd1306_128x64_i2c_init();
  ssd1306_fillScreen(0x00);
  ssd1306_setFixedFont(ssd1306xled_font6x8);
  zt.begin();
  zt.calcGyroOffsets(true);
  pinMode(aj_PIN,INPUT_PULLUP);
  m1_a.attach(m1);
  m1_b.attach(m2);
  m2_a.attach(m3);
  m2_b.attach(m4);
}

void loop() {
  zt.update();
  int zlfx = (zt.getAngleX())+100;
  int m1_Y = abs(zt.getAngleY());
  bool aj = digitalRead(aj_PIN);
  Serial.print("angleX : ");
  Serial.print(zt.getAngleX());
  Serial.print("\tangleY : ");
  Serial.print(zt.getAngleY());
  dtostrf(m1_Y, 1, 2, a);
  ssd1306_printFixedN(0,0,"angle |MYH",STYLE_NORMAL,FONT_SIZE_2X);
  ssd1306_printFixedN(0,32,a,STYLE_NORMAL,FONT_SIZE_4X);
  int m1_Y_2 = (m1_Y)+3;
  m1_a.write(m1_Y_2);
  m1_b.write(zlfx);
  if(aj_PIN==LOW){
    m2_a.write(90);
    m2_b.write(90);
  }else{
    m2_a.write(0);
    m2_b.write(0);
  }
  myclient = server.available();
  if(myclient){
    Serial.print("add new client ip:");
    Serial.println(myclient.remoteIP());
    while(myclient.connected()){
      if(myclient.available()){
        char c = myclient.read();
        Serial.print(c);
        if(c == '\n'){
          if(readLine.length()==0){
            getCommand();
            pageDisplay();
            break;
          }else{
            readLine = "";
          }
        }else if(c !='\r'){
          readLine += c;
        }
      }
    }
    myclient.stop();
    Serial.println("client disconnect.");
  }
  delay(50);
}
void getCommand(){
  potVal = abs(zt.getAngleY());
  potVal2 = abs(zt.getAngleX()-1);
}
void pageDisplay(){
  myclient.println("HTTP/1.1 200 OK");
  myclient.println("Content-type:text/html");
  myclient.println();
  myclient.println("<!DOCTYPE html><html><head><meta charset=\"utf-8\" http-equiv=\"refresh\" content =\"0.5\"><title>ESP32多功能演示仪A主板后台数据页面</title><style>html{text-align: center;background-color: bisque;}</style></head><body><h1>ESP32多功能演示仪A主板调试页面</h1><br><br><h2><b><p>MPU6050-Y轴：<font color=\"red\" size=\"6\">"+potVal+"</font>度</p></b></h2><h2><b><p>MG90S-X轴：<font color=\"red\" size=\"6\">"+potVal2+"</font>度</p></b></h2></body></html>");
}
