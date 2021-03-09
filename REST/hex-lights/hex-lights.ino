
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <FastLED.h>

#include <ArduinoJson.h>

// imports WIFI ssid/password (not included in repo)
#include "env.h"

#include "globals.h"

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

  for(int i=0; i<HEX_COUNT * LEDS_PER_HEX; i++)
  {
    leds[i] = CRGB::White;  
  }

  FastLED.addLeds<NEOPIXEL, 4>(leds, HEX_COUNT * LEDS_PER_HEX);
  FastLED.show();
}

void loop(void) {
  server.handleClient();
  FastLED.show();
}

 
