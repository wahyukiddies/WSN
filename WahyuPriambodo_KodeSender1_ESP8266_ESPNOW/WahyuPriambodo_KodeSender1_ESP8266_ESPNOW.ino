// ESP8266 #1: Send DHT11 sensor data to ESP32 #2 via ESPNOW
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "DHT.h"

#define DPIN D4       // Set Pin GPIO4 untuk nerima inputan sensor DHT11
#define DTYPE DHT11   // Set tipe sensor DHT11

DHT dht(DPIN, DTYPE); // Instansiasi obyek `dht` dengan argumen `DPIN` dan `DTYPE`

// Alamat MAC penerima (ESP32)
// ESP32 Board MAC Address: cc:db:a7:16:cf:a4
uint8_t broadcastAddress[] = {0xCC, 0xDB, 0xA7, 0x16, 0xCF, 0xA4};  // Ganti dengan alamat MAC ESP32

// Struktur untuk mengirim data
typedef struct struct_message {
  float temp;
  float hum;
} struct_message;

struct_message myData;

// Callback saat data terkirim
void onSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Send DHT11 sensor data to ESP32 #2: ");
  Serial.println(sendStatus == 0 ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);

  // Inisialisasi DHT sensor
  dht.begin();

  // Inisialisasi WiFi dan ESPNOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callback saat data terkirim
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(onSent);

  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  float tc = dht.readTemperature();  // Baca suhu dalam Celcius
  float hu = dht.readHumidity();     // Baca kelembaban

  // Tampilkan di Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(tc);
  Serial.print(" °C, Humidity: ");
  Serial.print(hu);
  Serial.println("%");

  // Siapkan data yang akan dikirim
  myData.temp = tc;
  myData.hum = hu;

  // Kirim data
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  delay(2000); // Kasih delay selama 2s
}
