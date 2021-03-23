
void setLights() {
  Serial.println("got sent");
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

  /*
   *  {
   *    "hue": the starting hue to use
   *    "boundry": the space between sections of the display (48 is one hex)
   *    "increment": how much the hue changes on each step
   *    "boundry_diff": how much to change the hue between each section
   *    "inc_speed": how long to wait (in ms) between each step
   *  }
   * 
   */

  current.hue = doc["hue"].as<uint8_t>();
  current.boundry = doc["boundry"].as<int16_t>();
  current.increment = doc["increment"].as<int8_t>();
  current.boundry_diff = doc["boundry_diff"].as<int8_t>();
  current.inc_speed = doc["inc_speed"].as<int16_t>();
    
  server.send(200, "text/json", "{\"status\": \"okay\"}");
}
