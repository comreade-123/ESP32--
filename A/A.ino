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

void setup() {
  Serial.begin(115200);
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
  int zlfx = (zt.getAngleX())+93;
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
  delay(50);
}
