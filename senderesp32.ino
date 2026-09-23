#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>

// LoRa pins
#define ss 5
#define rst 14
#define dio0 2

// DHT22 pins
#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

int counter = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender + DHT22");

  dht.begin();

  LoRa.setPins(ss, rst, dio0);

  while (!LoRa.begin(868E6)) {
    Serial.println(".");
    delay(500);
  }

  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  
  if (isnan(temp) || isnan(hum)) {
    Serial.println("Greska pri citanju DHT22 senzora!");
    delay(2000);
    return;
  }

  Serial.print("Temperatura: ");
  Serial.print(temp);
  Serial.print(" C, Vlaznost: ");
  Serial.print(hum);
  Serial.println(" %");

  
  String poruka = "T:" + String(temp, 2) + ",H:" + String(hum, 2) + ",#" + String(counter);

  Serial.print("Slanje: ");
  Serial.println(poruka);

  LoRa.beginPacket();
  LoRa.print(poruka);
  LoRa.endPacket();

  counter++;
  delay(10000); 
}