
// Define routing
void restServerRouting() {
  server.on("/", HTTP_GET, []() {
      server.send(200, F("text/html"),
          F("Welcome to the REST Web Server. See <a href='/help'>help</a> for more info."));
  });
  server.on(F("/help"), HTTP_GET, showHelp);
  server.on(F("/leds"), HTTP_POST, setLights);
  server.on(F("/pattern"), HTTP_POST, setPattern);
  server.on(F("/brightness"), HTTP_POST, setBrightness);
  
  // Set not found response
  server.onNotFound(showHelp);
}
