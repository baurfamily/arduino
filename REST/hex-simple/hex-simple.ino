// imports WIFI ssid/password (not included in repo)
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <FastLED.h>

#include <ArduinoJson.h>

#define LED_COUNT 576
#define DATA_PIN 13

#include "env.h"

WebServer server(80);
CRGB leds[LED_COUNT];

typedef struct pattern {
  uint8_t hue;
  uint16_t boundry;
  uint8_t  increment;
  uint8_t  boundry_diff;
  uint16_t inc_speed;
} pattern;

pattern current = { .hue = 0, .boundry = 48, .increment = 1, .boundry_diff = 1, .inc_speed = 1000 };

void setup(void) {
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
  // with local DNS hostmane *.local
  if (MDNS.begin(HOSTNAME)) {
    Serial.println("MDNS responder started");
  }
 
  // Set server routing
  restServerRouting();
  
  // Start server
  server.begin();
  Serial.println("HTTP server started");

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, LED_COUNT);
  FastLED.show();
}

void loop(void) {
  server.handleClient();

  current.hue += current.increment;

  int chunks = LED_COUNT / current.boundry;
  for (int i=0; i<current.boundry; i++) {
    for (int j=0; j<chunks; j++) {
      leds[i + j*current.boundry] = CHSV(current.hue + j*current.boundry_diff, 255, 255);
    }
  }

//  Serial.println("-------------------");
//  Serial.print("show: ");
//  Serial.print(current.hue);
//  Serial.print(" / ");
//  Serial.print(current.boundry);
//  Serial.print(" / ");
//  Serial.print(current.increment);
//  Serial.print(" / ");
//  Serial.print(current.boundry_diff);
//  Serial.print(" / ");
//  Serial.print(current.inc_speed);
//  Serial.println("");
  
  FastLED.show();
  delay(current.inc_speed);
}

 
