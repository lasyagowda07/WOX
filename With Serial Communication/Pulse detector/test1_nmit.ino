#include "WiFi.h"
#include "ESPAsyncWebSrv.h"
#include <Wire.h>

int sensorPin = 4;
int tilt= 5;
int LED=13;
int val;

const char* ssid = "Hackeye";
const char* password = "12345678";

 AsyncWebServer server(80);

String readmapPulse() {
  return String(readmapPulse());
}
 String readval() {
  return String(readval());
}

void setup() {
   Serial.begin(115200);
   Serial.println();
   Serial.print("Setting AP (Access Point)…");
   WiFi.softAP(ssid, password);
   IPAddress IP = WiFi.softAPIP();
   Serial.print("AP IP address: ");
   Serial.println(IP);
   
   server.on("/heartbeat", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readmapPulse().c_str());
   });

    server.on("/tilt", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readval().c_str());
  });

   pinMode(tilt, INPUT);
   //pinMode(sensorPin, INPUT);
   pinMode(LED, OUTPUT);

   server.begin();
}


void loop ()
{ 
   val = digitalRead(tilt);
if(val == HIGH)
{ 
  digitalWrite(LED, HIGH);
  int pulse;
  int sum = 0;
  for (int i = 0; i < 20; i++)
    sum += analogRead(sensorPin);
  pulse = sum / 20.00;
  int mapPulse= map(pulse, 0, 4095, 0,220);
  Serial.println(mapPulse);
  delay(100);
}
else
{
digitalWrite(LED,LOW);
}
}
