#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <FastLED.h>

#include <ArduinoJson.h>

#define LED_COUNT 576
#define DATA_PIN 13
#define PWM_PIN 12

#include "config.h"

// imports WIFI ssid/password (not included in repo)
#include "env.h"

WebServer server(80);
CRGB leds[LED_COUNT];
AppConfig config = AppConfig();

pattern current;
uint8_t brightness = 0;

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

void setup(void) {
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(PWM_PIN, ledChannel);
  ledcWrite(ledChannel, 0);
  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 
  // Activate mDNS this is used to be able to connect to the server
  // with local DNS hostname *.local
  if (MDNS.begin(HOSTNAME)) {
    Serial.println("MDNS responder started");
  }
 
  // Set server routing
  restServerRouting();
  
  // Start server
  server.begin();
  Serial.println("HTTP server started");

  config.begin();
  current = config.getPattern();

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, LED_COUNT);
  FastLED.show();

  printPatternToSerial(current);
}

void loop(void) {
  server.handleClient();

  brightness = (brightness+1) % 255;
  ledcWrite(ledChannel, brightness);

  current.hue += current.increment;

  int chunks = LED_COUNT / current.boundry;
  for (int i=0; i<current.boundry; i++) {
    for (int j=0; j<chunks; j++) {
      leds[i + j*current.boundry] = CHSV(
        current.hue + j*current.boundry_diff,
        255,
        current.value
       );
    }
  }
  
  FastLED.show();
  config.configSave();
  delay(current.inc_speed);
}

 
