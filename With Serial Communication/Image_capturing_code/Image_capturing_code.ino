#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "esp_camera.h"

const char* ssid = "Hackeye";
const char* password = "12345678";

const char* serverNamePulse = "http://192.168.4.1/heartbeat";
const char* serverNametilt = "http://192.168.4.1/value";


#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    4
#define SIOD_GPIO_NUM    18
#define SIOC_GPIO_NUM    23

#define Y9_GPIO_NUM      36
#define Y8_GPIO_NUM      37
#define Y7_GPIO_NUM      38
#define Y6_GPIO_NUM      39
#define Y5_GPIO_NUM      35
#define Y4_GPIO_NUM      14
#define Y3_GPIO_NUM      13
#define Y2_GPIO_NUM      34
#define VSYNC_GPIO_NUM   5
#define HREF_GPIO_NUM    27
#define PCLK_GPIO_NUM    25

// Set the camera module pins
#define I2C_SDA      26
#define I2C_SCL      27
#define CAMERA_PIN   32

// Camera resolution
#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480

String pulse;
String val;
int value;

unsigned long previousMillis = 0;
const long interval = 5000;

void setup() {
  Serial.begin(115200);
  
          WiFi.begin(ssid, password);
          Serial.println("Connecting");
          while(WiFi.status() != WL_CONNECTED) { 
          delay(500);
          Serial.print(".");
           }
            Serial.println("");
            Serial.print("Connected to WiFi network with IP Address: ");
            Serial.println(WiFi.localIP());
           
  // Initialize I2C communication
  Wire.begin(I2C_SDA, I2C_SCL);

  // Initialize the camera
   camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  // Init with almost the same defaults as Arduino Sketch
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  // Init the camera module
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Set the camera resolution
  sensor_t *s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_VGA);
}



void loop() {
     unsigned long currentMillis = millis(); 
    if(currentMillis - previousMillis >= interval) {
     // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED ){ 
      pulse= httpGETRequest(serverNamePulse);
      val = httpGETRequest(serverNametilt);
      value= val.toInt();
      Serial.println("HeatBeat: "+ pulse);
       previousMillis = currentMillis;
      }
      else {
      Serial.println("WiFi Disconnected");
        }
      }
    pulse = httpGETRequest(serverNamePulse);
    Serial.println("Pulse is: " +pulse); 

    if(value == HIGH) {
  // Capture a frame from the camera
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }



  // Send the image over serial communication to the computer
  size_t fb_len = fb->len;
  Serial.write((const uint8_t *)fb->buf, fb_len);

  // Return the frame buffer to the camera
  esp_camera_fb_return(fb);

  delay(5000); // Wait for 5 seconds before capturing the next image
}
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
