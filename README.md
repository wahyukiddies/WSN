# Tugas Praktisi Mengajar #2

## Alat dan Bahan

- Sensor DHT11
- NodeMCU ESP8266 LoLin V3 (x2)
- NodeMCU ESP32-WROOM-32

## Konektivitas

- ESP8266 #1 terhubung ke ESP32 #2 melalui protokol **ESPNOW**.
- ESP32 #2 terhubung ke ESP8266 #3 melalui **Serial Communication**.
- ESP8266 #3 terhubung ke MQTT Broker (HiveMQ) di topic **DHT11_Sensor/data**.

## Penjelasan

Mempraktikkan 3 protokol komunikasi yang berbeda di 3 buah ESP.
Jadi, ESP8266 #1 mengirimkan data sensor DHT11 ke ESP32 #2 melalui **ESPNOW**,
selanjutnya ESP32 #2 mengirimkan data yang berhasil diperoleh ke ESP8266 #3 (lainnya) melalui **Serial Communication**, dan 
setelah ESP8266 #3 menerima data tersebut, baru diubah ke dalam format JSON untuk dikirimkan ke Subscriber melalui public MQTT Broker yang tersedia secara free, dalam hal ini saya menggunakan **broker.hivemq.com**.
Untuk pengujian apakah data JSON tersebut sampai atau tidak ke Subscriber, saya menggunakan tool free yang bernama MQTT Explorer.


## Hasil
Alhamdulillah, semuanya berhasil terhubung sampai berhasil mengirimkan data berupa JSON ke MQTT broker untuk diteruskan ke Subscriber.

# Credits

- thomasnordquist (https://github.com/thomasnordquist/MQTT-Explorer)

