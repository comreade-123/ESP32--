#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
Servo jg1;
Servo jg2;
int jg_a = 2;
int jg_b = 4;
int dwqB_PIN = A0;

const char* ssid = "EPS32多功能演示仪C主板";
const char* password = "58565858";

WebServer server(80);

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
        <h2>C motherboard</h2>
        <br>
        <br>
        <br>
        <form action="/submit" method="post">
            Servo precision tuning(-
            ): <input type="number" name="number">
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
    jg1.attach(jg_a);
    jg2.attach(jg_b);
}

void loop() {
    server.handleClient();
    int dwqB = analogRead(dwqB_PIN);
    int dwqA3 = map(dwqB,0,2047,90,179);
    int dwqA3_2 = map(dwqB,0,2047,90,0);
    int dwqA3_3 = dwqA3_2-(server.arg("number").toInt());
    Serial.println(dwqA3);
    Serial.println();
    Serial.printf("dwqB:%d",dwqB);
    Serial.println();
    jg1.write(dwqA3);
    jg2.write(dwqA3_3);
    delay(50);
}
