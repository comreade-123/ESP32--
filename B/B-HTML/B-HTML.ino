#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include <HCSR04.h>
#include <ssd1306.h>
#include <ssd1306_console.h>

const char* ssid = "EPS32多功能演示仪B主板";
const char* password = "58565858";

WebServer server(80);

Ssd1306Console OLED1;
#define TRIGPIN 13//TRIG引脚
#define ECHOPIN 12//ECHO引脚
HCSR04 hc(TRIGPIN, ECHOPIN);//创建HCSR04对象
int fmq = 17;//蜂鸣器引脚
int L1 = 27;//LED绿
int L2 = 26;//LED黄
int L3 = 25;//LED红
Servo sj1;
Servo sj2;
int sj_a = 2;
int sj_b = 4;
int dwqA_PIN = A0;
char a[20];

void handle_root() {
    String html = R"rawliteral(
        <!DOCTYPE html>
        <html>
          <head>
            <meta charset="utf-8">
            <style>html{text-align: center;background-color: bisque;}</style>
        </head>
        <body>
        <h1>ESP32 multi-function presenter background debugging interface</h1>
        <h2>B motherboard</h2>
        <br>
        <br>
        <br>
        <form action="/submit" method="post">
            Servo precision tuning(+): <input type="number" name="number">
            <input type="submit" value="Submit">
        </form>
        </body>
        </html>
    )rawliteral";
    server.send(200, "text/html", html);
}

void handle_submit() {
    int number = server.arg("number").toInt();
    String response = "You entered: " + String(number);
    server.send(200, "text/plain", response);
}

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  server.on("/", handle_root);
  server.on("/submit", handle_submit);
  server.begin();
  Serial.println("HTTP server started");
  ssd1306_128x64_i2c_init();
  ssd1306_fillScreen(0x00);
  ssd1306_setFixedFont(ssd1306xled_font6x8);
  pinMode(fmq,OUTPUT);
  pinMode(L1,OUTPUT);
  pinMode(L2,OUTPUT);
  pinMode(L3,OUTPUT);
  sj1.attach(sj_a);
  sj2.attach(sj_b);
  ssd1306_printFixedN(0,0,"distance: ",STYLE_NORMAL,FONT_SIZE_2X);
  ssd1306_printFixedN(0,48,"CM    |MYH",STYLE_NORMAL,FONT_SIZE_2X);
}

void loop() {
  server.handleClient();
  Serial.println(server.arg("number").toInt());
  int dwqA = analogRead(dwqA_PIN);
  int dwqA0 = map(dwqA,0,4094,0,89);
  int dwqA0_2 = dwqA0+(server.arg("number").toInt());
  Serial.println(dwqA0);
  Serial.println();
  sj1.write(dwqA0);
  sj2.write(dwqA0_2);
  float csb = hc.dist();
  Serial.print("当前距离:");
  Serial.println(csb);
  Serial.println("CM");
  
  dtostrf(csb, 1, 2, a);
  //OLED1.print(csb);
  //OLED1.println("CM");
  ssd1306_printFixedN(0,24,a,STYLE_NORMAL,FONT_SIZE_2X);
  
 // OLED1.println();
  if(csb>20){
    digitalWrite(L1,HIGH);
    digitalWrite(L2,LOW);
    digitalWrite(L3,LOW);
    delay(50);
    //大于20cm，绿灯亮
  }else if(csb<=20 && csb>15){
    digitalWrite(L1,LOW);
    digitalWrite(L2,HIGH);
    digitalWrite(L3,LOW);
    digitalWrite(fmq,HIGH);
    delay(300);
    digitalWrite(fmq,LOW);
    delay(300);
    //小于20cm大于15cm，黄灯亮，蜂鸣器警报（慢）
  }else if(csb>5 && csb<=15){
    digitalWrite(L1,LOW);
    digitalWrite(L2,LOW);
    digitalWrite(L3,HIGH);
    digitalWrite(fmq,HIGH);
    delay(100);
    digitalWrite(fmq,LOW);
    delay(100);
    //大于5cm，小于15cm，红灯亮，蜂鸣器警报（快）
  }else if(csb>2 && csb<=5){
    digitalWrite(L1,LOW);
    digitalWrite(L2,LOW);
    digitalWrite(L3,HIGH);
    digitalWrite(fmq,HIGH);
    delay(50);
    digitalWrite(L3,LOW);
    digitalWrite(fmq,LOW);
    delay(50);
    //大于2cm，小于5cm，红灯爆闪，蜂鸣器警报（很快）
  }
}
