#include <ESP32Servo.h>
Servo jg1;
Servo jg2;
int jg_a = 2;
int jg_b = 4;
int dwqB_PIN = A0;

void setup() {
  jg1.attach(jg_a);
  jg2.attach(jg_b);
}

void loop() {
  int dwqB = analogRead(dwqB_PIN);
  int dwqA3 = map(dwqB,0,4094,90,179);
  int dwqA3_2 = map(dwqB,0,4094,90,0);
  int dwqA3_3 = dwqA3_2-5;
  Serial.println(dwqA3);
  Serial.println();
  jg1.write(dwqA3);
  jg2.write(dwqA3_3);
}
