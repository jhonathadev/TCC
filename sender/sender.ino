#include "heltec.h"
#include "DHT.h"

#define BAND 915E6  //you can set band here directly,e.g. 868E6,915E6
#define DHT11PIN 17
String packet;


void getData();
void sendPacket();

DHT dht(DHT11PIN, DHT11);

float humi;
float temp;

void getData() {
  humi = dht.readHumidity();
  temp = dht.readTemperature();
}

void sendPacket() {
  LoRa.beginPacket();
  LoRa.print(humi);
  LoRa.print("|");
  LoRa.print(temp);

  //LoRa.print(humi);
  //LoRa.print(temp);
  LoRa.endPacket();
}


void setup() {
  Heltec.begin(true /*DisplayEnable Enable*/, true /*LoRa Enable*/, true /*Serial Enable*/, true /*LoRa use PABOOST*/, BAND /*LoRa RF working band*/);
  /* Start the DHT11 Sensor */
  dht.begin();
  Heltec.display -> clear();

}

void loop() {
  getData();
  Heltec.display -> drawString(0, 10,"Temperatura: "+(String)temp+" C");
  Heltec.display -> drawString(0, 20,"Humidade: "+(String)humi+" %");
  Heltec.display -> display();
  delay(1000);
  Heltec.display -> clear();
  sendPacket(); //Envia temperatura
}