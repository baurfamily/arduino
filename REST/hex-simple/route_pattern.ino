
#define CHAIN { .hue=0, .boundry=1, .increment=1, .boundry_diff=1, .inc_speed=100 }
#define RAINBOW { .hue=0, .boundry=48, .increment=1, .boundry_diff=1, .inc_speed=100 }
#define TWO_COLOR { .hue=0, .boundry=48, .increment=1, .boundry_diff=128, .inc_speed=20 }
#define FOUR_COLOR { .hue=0, .boundry=48, .increment=1, .boundry_diff=64, .inc_speed=20 }
#define DISCO { .hue=0, .boundry=48, .increment=100, .boundry_diff=100, .inc_speed=500 }

#define RED { .hue=0, .boundry=576, .increment=0, .boundry_diff=0, .inc_speed=1000 }
#define ORANGE { .hue=32, .boundry=576, .increment=0, .boundry_diff=0, .inc_speed=1000 }
#define YELLOW { .hue=64, .boundry=576, .increment=0, .boundry_diff=0, .inc_speed=1000 }
#define GREEN { .hue=96, .boundry=576, .increment=0, .boundry_diff=0, .inc_speed=1000 }
#define AQUA { .hue=128, .boundry=576, .increment=0, .boundry_diff=0, .inc_speed=1000 }
#define BLUE { .hue=160, .boundry=576, .increment=0, .boundry_diff=0, .inc_speed=1000 }
#define PURPLE { .hue=192, .boundry=576, .increment=0, .boundry_diff=0, .inc_speed=1000 }
#define PINK { .hue=224, .boundry=576, .increment=0, .boundry_diff=0, .inc_speed=1000 }

void setPattern() {
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

  /*
   *  {
   *    "pattern": the name of the pattern to use, see names below
   *  }
   * 
   */

  pattern newPattern;
  const char* patternName = doc["pattern"];
  bool found = false;

  if (strcmp(patternName, "chain")==0) { newPattern = CHAIN; found = true; }
  if (strcmp(patternName, "rainbow")==0) { newPattern= RAINBOW; found = true; }
  if (strcmp(patternName, "two_color")==0) { newPattern = TWO_COLOR; found = true; }
  if (strcmp(patternName, "four_color")==0) { newPattern = FOUR_COLOR; found = true; }
  if (strcmp(patternName, "disco")==0) { newPattern = DISCO; found = true; }
  if (strcmp(patternName, "red")==0) { newPattern = RED; found = true; }
  if (strcmp(patternName, "orange")==0) { newPattern = ORANGE; found = true; }
  if (strcmp(patternName, "yellow")==0) { newPattern = YELLOW; found = true; }
  if (strcmp(patternName, "green")==0) { newPattern = GREEN; found = true; }
  if (strcmp(patternName, "aqua")==0) { newPattern = AQUA; found = true; }
  if (strcmp(patternName, "blue")==0) { newPattern = BLUE; found = true; }
  if (strcmp(patternName, "purple")==0) { newPattern = PURPLE; found = true; }
  if (strcmp(patternName, "pink")==0) { newPattern = PINK; found = true; }

  if (found) {
    Serial.println("-------------------");
    Serial.print("show: ");
    Serial.print(newPattern.hue);
    Serial.print(" / ");
    Serial.print(newPattern.boundry);
    Serial.print(" / ");
    Serial.print(newPattern.increment);
    Serial.print(" / ");
    Serial.print(newPattern.boundry_diff);
    Serial.print(" / ");
    Serial.print(newPattern.inc_speed);
    Serial.println("");
    
    current.hue = newPattern.hue;
    current.boundry = newPattern.boundry;
    current.increment = newPattern.increment;
    current.boundry_diff = newPattern.boundry_diff;
    current.inc_speed = newPattern.inc_speed;
  
    Serial.println("setting...");
    server.send(200, "text/json", "{\"status\": \"okay\"}");
    
  } else {
    Serial.println("could not find specified pattern");
    server.send(200, "text/json", "{\"status\": \"not found\"}");
  }

}
