/*
 * MUMID prototype 2021 
 * 
 * Placa 1 - envio de dados sensores para plataforma Firebase e Google sheets
 * 
 * by: estudante Thales 3° ano IFSC campus Criciúma
 * 
 */

#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <WiFiClientSecure.h>

//sensor de temp (MAX31855) 
#include <SPI.h>
#include "Adafruit_MAX31855.h"

#define MAXDO   12
#define MAXCS   15
#define MAXCLK  13

Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

float temp = 0;

//sensor estoque
#define pinoestoque A0
int distancia = 0;

//Firebase

#define FIREBASE_HOST "https://mumid-prototype-23da6-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "bqZy1UegK5hvvwkfYG7TL3k4TZFluxE673H8qrzD"
FirebaseData firedata;

//WiFi

#define WIFI_SSID "Minha rede"
#define WIFI_PASSWORD "360360180"

//endereço do script Google Sheets
const char* host = "script.google.com";
const int httpsPort = 443;

WiFiClientSecure client;

String GAS_ID = "1gfnjP7Wut2MBG5AcEEcGG_dnW5Z76F2naBuqNei710k"; //--> spreadsheet script ID

void setup() {
  
  Serial.begin(9600);

  //setando pinos
  
    pinMode(A0, INPUT);
  
  //conectando com wifi
  
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Conectando em: ");
    Serial.print(WIFI_SSID);
      while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
    }
    Serial.println();
    Serial.print("Conectado no IP: ");
    Serial.println(WiFi.localIP());
  
  //inciando conexão firebase
  
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
    delay(1000);
  
  client.setInsecure();
}

void loop() {
  
 //leitura de medidas
 temp = thermocouple.readCelsius();
 distancia = analogRead(A0);
 
 envia_firebase(temp,distancia);
 
 delay(1000);

}

void envia_firebase(float temp_f,float estoque_f){

  //firebase
  if (Firebase.setFloat(firedata, "/status/1forno1", temp_f)) {    // On successful Write operation, function returns 1  

    Serial.println("Temperatura enviada ao firebase =");
    Serial.println(temp_f);
    Serial.println("\n");
  
  }else {        
    Serial.println(firedata.errorReason());
  }
  if (Firebase.setFloat(firedata, "/status/acucar", estoque_f)) {    // On successful Write operation, function returns 1  

    Serial.println("Distancia enviada ao firebase = ");
    Serial.println(estoque_f);
    Serial.println("\n");
  
  }else {        
    Serial.println(firedata.errorReason());
  }
}
