
// Serving Hello world
void getHelloWord() {
  Serial.println("hello!");
    server.send(200, "text/json", "{\"name\": \"Hello world\"}");
}
