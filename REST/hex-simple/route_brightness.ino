
void setBrightness() {
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

  /* {
   *    "brightness": the brightness to set
   *  }
   */

  brightness = doc["brightness"].as<uint8_t>();

  Serial.println("setting...");
  config.setBrightness(brightness);

  Serial.println(brightness);
    
  server.send(200, "text/json", "{\"status\": \"okay\"}");
} 
