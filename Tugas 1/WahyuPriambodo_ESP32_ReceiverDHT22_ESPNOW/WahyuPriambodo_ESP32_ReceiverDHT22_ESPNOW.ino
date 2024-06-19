#include <ESP8266WiFi.h>
extern "C" {
  #include <espnow.h>
}

typedef struct struct_message {
  float temperature;
  float humidity;
} struct_message;

struct_message incomingData;

const int ledPin = D2;  // Define the pin where the LED is connected
const float temperatureThreshold = 30.0;  // Temperature threshold in Celsius
const float humidityThreshold = 70.0;  // Humidity threshold in percentage

void setup() {
  Serial.begin(115200);

  // Set the LED pin as an output
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  // Make sure the LED is off initially

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register receive callback
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(onDataRecv);
}

void loop() {
  // Loop does nothing. Data is received via callback
}

void onDataRecv(uint8_t *mac, uint8_t *data, uint8_t len) {
  memcpy(&incomingData, data, sizeof(incomingData));

  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Temperature: ");
  Serial.println(incomingData.temperature);
  Serial.print("Humidity: ");
  Serial.println(incomingData.humidity);

  // Check if the received data exceeds the thresholds
  if (incomingData.temperature > temperatureThreshold || incomingData.humidity > humidityThreshold) {
    digitalWrite(ledPin, HIGH);  // Turn on the LED
    delay(100);  // Wait for 100 ms
    digitalWrite(ledPin, LOW);  // Turn off the LED
  }
}