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

#define MAXDO   14
#define MAXCS   13
#define MAXCLK  12

Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

float temp = 0;

//sensor estoque
#define pinoestoque A0
int distancia = 0;

//Firebase
#define FIREBASE_HOST "https://mumid-prototype-23da6-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "bqZy1UegK5hvvwkfYG7TL3k4TZFluxE673H8qrzD"
#define WIFI_SSID "Juliana 2.4G"
#define WIFI_PASSWORD "lucas2001"
FirebaseData firedata;

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
  
  //inciando conexão firebas
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  delay(1000);
  
  client.setInsecure();
}

void loop() {
  
 //leitura de medidas
 temp = thermocouple.readCelsius();
 distancia = analogRead(A0);
 
 enviatemp_firebase(temp,distancia);
 //enviatemp_googlesheets(temp,analog_estoque);
 
 delay(10000);

}

void enviatemp_firebase(float temp_f,float estoque_f){

  //firebase
  if (Firebase.setFloat(firedata, "/Dados/temperatura", temp_f)) {    // On successful Write operation, function returns 1  

    Serial.println("Temperatura enviada ao firebase =");
    Serial.println(temp_f);
    Serial.println("\n");
  
  }else {        
    Serial.println(firedata.errorReason());
  }
  if (Firebase.setFloat(firedata, "/Dados/estoque", estoque_f)) {    // On successful Write operation, function returns 1  

    Serial.println("Distancia enviada ao firebase = ");
    Serial.println(estoque_f);
    Serial.println("\n");
  
  }else {        
    Serial.println(firedata.errorReason());
  }
}

/*
void enviatemp_googlesheets(float temp_g,float estoque_g){


  Serial.print("connecting to ");
  Serial.println(host);
  
  //conectando ao host do Google
  if (!client.connect(host, httpsPort)) {
    Serial.println("Conexão com host perdida!");
    return;
  }

  //processando dados e enviando
  String string_temperatura =  String(temp_g); 
  String string_estoque =  String(estoque_g); 
  String url = "/macros/s/" + GAS_ID + "/exec?temperatura=" + string_temperatura + "&estoque=" + string_estoque;
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "User-Agent: BuildFailureDetectorESP8266\r\n" + "Connection: close\r\n\r\n");

  Serial.println("request sent");

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
}
*/
