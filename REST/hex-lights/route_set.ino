
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
