// imports WIFI ssid/password (not included in repo)
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <FastLED.h>

#include <ArduinoJson.h>

#define HEX_SIDE_COUNT 6
#define HEX_LEDS_PER_SIDE 8
#define HEX_LEDS_COUNT HEX_SIDE_COUNT * HEX_LEDS_PER_SIDE

#define HEX_COUNT 12
#define TOTAL_LEDS HEX_COUNT * HEX_LEDS_COUNT

#include "env.h"
#include "hex.h"

WebServer server(80);
Hex hexes[HEX_COUNT];
CRGB leds[HEX_COUNT][HEX_SIDE_COUNT][HEX_LEDS_PER_SIDE];

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

  for(int i=0; i<HEX_COUNT; i++) {
    hexes[i] = Hex( leds[i] );
  }

  FastLED.addLeds<NEOPIXEL, 4>(leds, TOTAL_LEDS);
  FastLED.show();
}

void loop(void) {
  server.handleClient();
  FastLED.show();
}

 
