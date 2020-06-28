#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <TM1637Display.h>
#define CLK 2 //D4
#define DIO 0 //D3
const int trigPin = D1;
const int echoPin = D2;
unsigned int angka;
long duration;
int distance;
TM1637Display display(CLK, DIO);
ESP8266WebServer server(80);
void setup() {
  display.setBrightness(0x0f);  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  WiFi.softAP("NAYLATOOLS", "naylatools");
  
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Alamat IP: ");
  Serial.println(myIP);
  server.on("/", html);
  server.on("/data", data);
  server.begin();
  Serial.println("HTTP server started");
}
void html(){
  String html = "";
  html += "<!DOCTYPE html>";
  html += "<html lang='en'>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Meteran</title>";
  html += "</head>";
  html += "<body>";
  html += "<h1 id='jarak'></h1>";
  html += "<script>";
  html += "setInterval(";
  html += "function() {";
  html += "var xhttp = new XMLHttpRequest();";
  html += "xhttp.onreadystatechange = function() {";
  html += "if (this.readyState == 4 && this.status == 200) {";
  html += "document.getElementById('jarak').innerHTML = this.responseText;";
  html += "}";
  html += "};";
  html += "xhttp.open('GET', '/data', true);";
  html += "xhttp.send();";
  html += "}, 100";
  html += ");";
  html += "</script>";
  html += "</body>";
  html += "</html>";
  server.send(200, "text/html", html);
}

void data(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  Serial.print("Jarak: ");
  Serial.println(distance);
  server.send(200, "text/html", String(distance));
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  Serial.println(distance);
  display.showNumberDec(distance); 
  server.handleClient();
  delay(100);
}
