// ESP8266 #3: Receive data from ESP32 #2 via Serial Comm
void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    String receivedData = Serial.readStringUntil('\n');
    Serial.println("Success received data from ESP32 #2");
    Serial.println(receivedData);
  }
}
