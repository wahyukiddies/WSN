// ESP8266 #3: Kirim data dari ESP32 #2 melalui Serial Comm ke Public MQTT Broker (HiveMQ) dalam format JSON.
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Kredensial WiFi
const char* ssid = "TEBAK PUNYA SIAPA ?";
const char* password = "Bukitduri012";

// Detail MQTT Broker
const char* mqtt_broker = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_pub_topic = "DHT11_Sensor/data";

// Inisialisasi klien WiFi dan MQTT
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);  // Kecepatan komunikasi Serial
  setup_wifi();          // Menghubungkan ke WiFi
  client.setServer(mqtt_broker, mqtt_port);  // Mengatur server MQTT
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    setup_wifi();  // Coba koneksi ulang ke WiFi jika terputus
  }

  if (!client.connected()) {
    reconnect();  // Coba koneksi ulang ke MQTT broker jika terputus
  } else { 
    client.loop();  // Pertahankan koneksi MQTT tetap aktif
  }

  if (Serial.available() > 0) {
    String receivedData = Serial.readStringUntil('\n');  // Baca data dari Serial
    Serial.println("Success received DHT11 sensor data from ESP32 #2");
    Serial.println(receivedData);

    // Parse data yang diterima (format "Temperature: XX.XX °C, Humidity: YY.YY %")
    float temperature, humidity;
    sscanf(receivedData.c_str(), "Temperature: %f °C, Humidity: %f %%", &temperature, &humidity);

    // Buat objek JSON
    StaticJsonDocument<200> doc;
    
    // Gunakan dtostrf untuk mengonversi float menjadi string dengan 2 angka di belakang koma
    char tempStr[6], humStr[6];
    dtostrf(temperature, 4, 2, tempStr);
    dtostrf(humidity, 4, 2, humStr);
    
    doc["temperature"] = tempStr;
    doc["humidity"] = humStr;
    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer);

    // Publikasi data JSON ke MQTT Broker
    boolean isMsgPublished = client.publish(mqtt_pub_topic, jsonBuffer, true);
    if (isMsgPublished) {
      Serial.println("MQTT message published successfully");
    } else {
      Serial.println("MQTT message publishing failed");
    }
  }
}

void setup_wifi() {
  Serial.print("Connecting to: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);  // Mulai koneksi WiFi

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());  // Tampilkan alamat IP yang didapatkan
}

void reconnect() {
  // Loop hingga terkoneksi kembali ke MQTT
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Coba koneksi
    if (client.connect("ESP8266Client")) {
      Serial.println("Successfully connected to MQTT broker!");
    } else {
      Serial.print("Failed to connect to MQTT Broker, try again in 2 seconds: ");
      Serial.println(client.state());
      // Tunggu 2 detik sebelum mencoba ulang
      delay(2000);
    }
  }
}