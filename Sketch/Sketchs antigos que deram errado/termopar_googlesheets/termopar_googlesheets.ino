/*
 * MUMID prototype 2021 
 * 
 * Placa 2 - interface/ponto/cadastro
 * 
 * by: estudante Thales 3° ano IFSC campus Criciúma
 * 
 */
#include <Wire.h> // comunicação com a interface I2C
#include <LiquidCrystal_I2C.h> // comunicação display
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <FirebaseESP8266.h>

//WiFi

#define WIFI_SSID "Juliana 2.4G"
#define WIFI_PASSWORD "lucas2001"

//Firebase

#define FIREBASE_HOST "https://mumid-prototype-23da6-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "OZj8bV503VgdHD24jxRaHQGW1WvzDOyV6CqnLd0r"
FirebaseData firedata;

//LCD

LiquidCrystal_I2C lcd(0x27,20,4); //lcd 20x4

WiFiClientSecure client;


//variaveis globais

float forno1;
int acucar;


void setup()
{
  
Serial.begin(9600);

  //iniciando display
  
    lcd.init(); 
    lcd.backlight();
    
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

void loop()
{
  
    recebe_firebase(forno1,acucar);

  //boas vindas lcd
  
    lcd.setCursor(0,0);
    lcd.print("Seja bem-vindo !!!");
    lcd.setCursor(0,2);
    lcd.print("iniciando MUMID");
    delay(3000); //intervalo de 1s

    lcd.clear();
}

void recebe_firebase(float temp_f,int estoque_f){

  if (Firebase.getint(firedata, "/status/1forno1", temp_f)) {  

    lcd.setCursor(0,0);
    lcd.print("Forno 1: ");
    lcd.print(temp_f);
    lcd.print(" %");
        
    Serial.println("Temperatura forno 1 recebida do firebase =");
    Serial.println(temp_f);
    Serial.println("\n");
  
  }else {        
    Serial.println(firedata.errorReason());
}
}
