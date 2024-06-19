#include <esp_now.h>
#include <WiFi.h>
#include "DHTesp.h"

DHTesp dht;
const int DHT_PIN = 4;
const int LED_PIN = 2; // GPIO 2 (D2) untuk LED

// MAC ADDRESS ESP8266 (RECEIVER): C8:C9:A3:33:9F:2F
uint8_t receiverMacAddress[] = {0xC8, 0xC9, 0xA3, 0x33, 0x9F, 0x2F};

typedef struct struct_message {
  float temperature;
  float humidity;
} struct_message;

struct_message myData;

unsigned long previousMillis = 0;
const long interval = 1000; // Interval untuk kedipan LED (ms)
bool ledState = LOW; // Keadaan awal LED

void setup() {
  Serial.begin(115200);

  // Initialize DHT sensor
  dht.setup(DHT_PIN, DHTesp::DHT22);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverMacAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  // Setup LED pin
  pinMode(LED_PIN, OUTPUT);

  // Send data
  esp_now_register_send_cb(onDataSent);
}

void loop() {
  unsigned long currentMillis = millis();

  // Blink LED
  if (currentMillis - previousMillis >= interval / 2) {
    previousMillis = currentMillis;

    // Invert LED state
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }

  myData.temperature = dht.getTemperature();
  myData.humidity = dht.getHumidity();

  if (isnan(myData.temperature) || isnan(myData.humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(receiverMacAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }

  delay(3000);
}

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
