#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DHT11.h>
#include <ESP8266WebServer.h>

//IP details
const char* ssid = "ESP_8266";   // Custom SSID
const char* pass = "1234567890"; // Password (min. 8 chars)
ESP8266WebServer server(80);  // port decleration

// pin decleration

int bulb = 15; // Bulb on D8
int fan = 13; // Fan on D7
int ldr = 14;  // LDR on D5
int ultr_trig = 12; //Ultrasonic trigger on D6
int ultr_echo = 5; //Ultrasonic echo on D1
DHT11 dht11(2); // DHT on D4

// variable decleration

bool bulbstatval = 0;
bool fanstatval = 0;

String modestatval = "auto";
int temperature = 0;
float duration = 0;

// setup parameters

float distance_thresh = 1000;
int temp_thresh = 27;
int ppl_count = 0;

// Distance ultrasonic

float ultrsnc(){
  digitalWrite(ultr_trig, LOW);
  delayMicroseconds(20);
  digitalWrite(ultr_trig, HIGH);
  delayMicroseconds(100);
  duration = pulseIn(ultr_echo, HIGH);
  return duration;
}

// fucntion declerations for html

String htmlpg(int bulbstat, int fanstat, String modestat){
  String html = "<!DOCTYPE html> <html>\n";
  html +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  html +="<title>LED Control</title>\n";
  html +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; color: #5C8D89}\n";
  html +="body{background-color:F9F8EB ; margin-top: 50px;} h1 {color: black;margin: 50px auto 30px;} h3 {color: #5C8D89;margin-bottom: 50px;}\n";
  html +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: #5C8D89;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  html +=".button-on {background-color: #A7D7C5;}\n";
  html +=".button-on:active {background-color: #5C8D89;}\n";
  html +=".button-off {background-color: #A7D7C5;}\n";
  html +=".button-off:active {background-color: #5C8D89;}\n";
  html +="p {font-size: 14px;color: #5C8D89;margin-bottom: 10px;}\n";
  html +="h1 {color : #5C8D89;} \n";
  html +="footer {color: #5C8D89;} \n";
  html +="</style>\n";
  html +="</head>\n";
  html +="<body>\n";
  html +="<h1>Home Automation</h1>\n";

  if(modestat == "auto"){
    html +="<p>Mode : AUTO</p> <a class=\"button button-off\" href=\"/modemanual\">OFF</a>\n";
  }
  else{
    html +="<p>Mode : MANUAL</p><a class=\"button button-on\" href=\"/modeauto\">ON</a>\n";
  }

  if(bulbstat){
    html +="<p>BULB Status: ON</p> <a class=\"button button-off\" href=\"/bulboff\">OFF</a>\n";
  }
  else{
    html +="<p>BULB Status: OFF</p><a class=\"button button-on\" href=\"/bulbon\">ON</a>\n";
  }

  if(fanstat){
    html +="<p>FAN Status: ON</p><a class=\"button button-off\" href=\"/fanoff\">OFF</a>\n";
  }
  else{
    html +="<p>FAN Status: OFF</p><a class=\"button button-on\" href=\"/fanon\">ON</a>\n";
  }
  
  html += "<footer> \n"; 
  html += "</footer> \n";
  html += "</body> \n";
  html += "</html> \n";

  return html;
  }

// basic setup 

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("\n\n\nSetting up...");

  pinMode(bulb, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(ldr, INPUT);
  pinMode(ultr_trig, OUTPUT);
  pinMode(ultr_echo, INPUT);

  // Start Access Point with default settings
  WiFi.softAP(ssid, pass);
  
  Serial.print("Access Point started: ");
  Serial.println(ssid);
  Serial.print("Default IP address: ");
  Serial.println(WiFi.softAPIP());  // Default is usually 192.168.4.1

  // Setup web server routes
  server.on("/", handleRoot);
  server.on("/modeauto", handlemodeauto);
  server.on("/modemanual", handlemodemanual);
  server.on("/bulbon", handlebulb_on);
  server.on("/bulboff", handlebulb_off);
  server.on("/fanon", handlefan_on);
  server.on("/fanoff", handlefan_off);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("Server started in AP mode");
}

// main program

void loop() {
  
  server.handleClient();
  temperature = dht11.readTemperature();
  Serial.print("Bulb mode: ");
  Serial.println(bulbstatval);
  Serial.print("Fan mode: ");
  Serial.println(fanstatval);
  Serial.print("Mode of operation: ");
  Serial.println(modestatval);
  Serial.print("Temperature");
  Serial.println(temperature);
  Serial.print("Distance");
  Serial.println(duration);

  Serial.println("\n");

  if(bulbstatval == 0){
    digitalWrite(bulb,LOW);
  }
  else{
    digitalWrite(bulb,HIGH);
  }

  if(fanstatval == 0){
    digitalWrite(fan,LOW);
  }
  else{
    digitalWrite(fan,HIGH);
  }

  
  if(modestatval == "auto"){
    ultrsnc();
    delay(500);
    if (duration < distance_thresh){
      ppl_count+=1;
    }

    if (ppl_count>=1){ 
      if (digitalRead(ldr) == 1){
       bulbstatval = 1;
       Serial.println("Ambient light low");
      }
      else{
        bulbstatval = 0;
        Serial.println("Ambient light sufficient");
      }
      if (temperature > temp_thresh){
        fanstatval = 1;
        Serial.println("Ambient temp hot");
      }
      else {
        fanstatval = 0;
        Serial.println("Ambient temp cool");
      }
        
    }
  }
}

// webserver functions

void handleRoot(){
  server.send(200, "text/html", htmlpg(bulbstatval,fanstatval,modestatval));
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found");          
}

void handlefan_off(){
  digitalWrite(fan, LOW);
  fanstatval = 0;
  server.sendHeader("Location", "/");
  Serial.println("Turning fan off");
  server.send(200 , "text/html", htmlpg(bulbstatval,0,modestatval));
}

void handlefan_on(){
  digitalWrite(fan, HIGH);
  fanstatval = 1;
  server.sendHeader("Location", "/");
  Serial.println("Turning fan on");
  server.send(200 , "text/html", htmlpg(bulbstatval,1,modestatval));
}

void handlebulb_on(){
  digitalWrite(bulb, HIGH);
  bulbstatval = 1;
  server.sendHeader("Location", "/");
  Serial.println("Turning bulb on");
  server.send(200 , "text/html", htmlpg(1,fanstatval,modestatval));
}

void handlebulb_off(){
  digitalWrite(bulb, LOW);
  bulbstatval = 0;
  server.sendHeader("Location", "/");
  Serial.println("Turning bulb off");
  server.send(200 , "text/html", htmlpg(0,fanstatval,modestatval));
}

void handlemodeauto(){
  modestatval = "auto";
  Serial.println("Changing mode auto");
  server.sendHeader("Location", "/");
  server.send(200, "text/html", htmlpg(bulbstatval,fanstatval,"auto")); 
}

void handlemodemanual(){
  modestatval = "manual";
  ppl_count = 0;
  Serial.println("Changing mode manual");
  server.sendHeader("Location", "/");
  server.send(200, "text/html", htmlpg(bulbstatval,fanstatval,"manual"));
}