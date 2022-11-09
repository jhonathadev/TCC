#include "heltec.h"
#include "WiFi.h"

#define BAND 915E6  //Escolha a frequência

String packSize = "--";
String packet;
String humi, temp;
String wifi_name = "manteiga";
String wifi_password = "87654321";
int x = 1;

/* Protótipo da função */
void LoRaDataPrint();
void cbk(int packetSize);


void WIFISetUp(void) {
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.disconnect(true);
  delay(100);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(true);
  WiFi.begin(wifi_name, wifi_password);//fill in "Your WiFi SSID","Your Password"
  delay(100);

  byte count = 0;
  while(WiFi.status() != WL_CONNECTED && count < 10) {
    count ++;
    delay(500);
    Heltec.display->clear();
    Heltec.display->drawString(0, 0, "Connecting...");
    Heltec.display->display();
  }
  //Heltec.display->clear();
  if(WiFi.status() == WL_CONNECTED) {
    //Heltec.display->drawString(35, 38, "WIFI SETUP");
    Heltec.display->drawString(0, 9, "OK");
    Heltec.display->display();
    delay(1000);
    Heltec.display->clear();
  }
  else
  {
    //Heltec.display->clear();
    Heltec.display->drawString(0, 9, "Failed");
    Heltec.display->display();
    delay(1000);
    Heltec.display->clear();
  }
}




/*
  Nome da função: LoRaDataPrint
  objetivo: imprime a temperatura e tamanho do pacote recebido no display.
*/
void LoRaDataPrint(){
  //Heltec.display->setFont(ArialMT_Plain_10);
  //Heltec.display->drawString(0 , 1 , "Recebendo "+ packSize + " bytes");
  Heltec.display->setFont(ArialMT_Plain_10);
  humi = packet.substring(0,5);
  temp = packet.substring(6,11);
  Heltec.display->clear();
  Heltec.display->drawString(0, 10, "Temperatura: " + temp + " °C");
  Heltec.display->drawString(0, 20, "Humidade: " + humi + " %");
  Heltec.display->display();
   
  //Heltec.display->drawString(33, 38, packet);
  //Heltec.display->drawString(78, 38, "°C");
}

/*
  Nome da função: cbk
  recebe como parâmetos um inteiros (packetSize)
  objetivo: recebe a temperatura via LoRa e armazena na variável packet.
*/
void cbk(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC);
  for (int i = 0; i < packetSize; i++) {
    packet += (char) LoRa.read(); //Atribui um caractere por vez a váriavel packet 
  }
}

/******************* função principal (setup) *********************/
void setup()
{
  Heltec.begin(true /*DisplayEnable Enable*/, true /*LoRa Enable*/, true /*Serial Enable*/, true /*LoRa use PABOOST*/, BAND /*LoRa RF working band*/);
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->clear();
  Heltec.display->drawString(10, 5, "Iniciado com Sucesso!");
  Heltec.display->drawString(10, 30, "Aguardando os dados...");
  Heltec.display->display();
  LoRa.receive(); // Habilita o rádio LoRa para receber dados
  Heltec.display->clear();

}

/******************* função em loop (loop) *********************/
void loop()
{
  int packetSize = LoRa.parsePacket();

  if (packetSize) { //Verifica se há dados chegando via LoRa
    cbk(packetSize);
    LoRaDataPrint();
  }   

  delay(2000);
  Heltec.display->clear();

}