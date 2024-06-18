// ESP32 #2: Send DHT11 sensor data to ESP8266 #3 via Serial Comm
#include <WiFi.h>
#include <esp_now.h>

// Struktur untuk menerima data
typedef struct struct_message {
  float temp;
  float hum;
} struct_message;

struct_message dhtData;

// Callback saat data diterima
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
  memcpy(&dhtData, incomingData, sizeof(dhtData));
  // Menampilkan data yang dikirim melalui ESPNOW
  Serial.println("Success receive DHT11 sensor data from ESP8266 #1");
  Serial.print("Temperature: ");
  Serial.print(dhtData.temp);
  Serial.print(" °C, Humidity: ");
  Serial.print(dhtData.hum);
  Serial.println("%");

  // Kirim datanya kembali melalui sambungan Serial1
  Serial1.print("Temperature: ");
  Serial1.print(dhtData.temp);
  Serial1.print(" °C, Humidity: ");
  Serial1.print(dhtData.hum);
  Serial1.println("%");

  // Flush serial buffer to ensure all data is sent
  Serial1.flush();
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, 16, 17); // RX = 16, TX = 17

  // Inisialisasi WiFi dan ESPNOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callback saat data diterima
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {
  // Tidak ada kode yang diperlukan di loop
}
