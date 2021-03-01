/*
 *  Simple hello world Json REST response
  *  by Mischianti Renzo <https://www.mischianti.org>
 *
 *  https://www.mischianti.org/
 *
 */
 
#include "Arduino.h"

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>

#include <ArduinoJson.h>

// imports WIFI ssid/password (not included in repo)
#include "env.h"
 
WebServer server(80);

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
  if (MDNS.begin("hex-lights")) {
    Serial.println("MDNS responder started");
  }
 
  // Set server routing
  restServerRouting();
  
  // Set not found response
  server.onNotFound(handleNotFound);
  
  // Start server
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}

// Define routing
void restServerRouting() {
    server.on("/", HTTP_GET, []() {
        server.send(200, F("text/html"),
            F("Welcome to the REST Web Server"));
    });
    server.on(F("/helloWorld"), HTTP_GET, getHelloWord);
    server.on(F("/leds"), HTTP_POST, setLights);
}

void setLights() {
  Serial.println("got set");
  Serial.println(server.arg(0));

  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, server.arg(0));
  
  //Check for errors in parsing
  if (error) {
    Serial.println("Parsing failed for:");
    Serial.println(server.arg(0));
    String message = error.f_str();
    String response =  "{\"error\": \"Failed to parse request\", \"message\": \"" + message + "\"}";
    server.send(400, "text/json", response );
    return;
  }

  // currently expects { "foo": "some text to echo back" }
  const char * foo = doc["foo"];
  int bar = doc["bar"]; // left over from example
  
  server.send(200, "text/json", F(foo));
}
 
// Serving Hello world
void getHelloWord() {
  Serial.println("hello!");
    server.send(200, "text/json", "{\"name\": \"Hello world\"}");
}
 
// Manage not found URL
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
 
 
