#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <WiFiClientSecure.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4); 

//Firebase e WiFi
#define FIREBASE_HOST "https://mumid-prototype-23da6-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "bqZy1UegK5hvvwkfYG7TL3k4TZFluxE673H8qrzD"
#define WIFI_SSID "Theios"
#define WIFI_PASSWORD "lucas2001"
FirebaseData firedata;
WiFiClientSecure client;


float val_temp = 0;
float val_estoque = 0;

void setup()
{

  Serial.begin(9600);

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

  LCD();
  
}
void LCD(){
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(5,0);
  lcd.print("MUMID 2021");
  lcd.setCursor(3,2);
  lcd.print("IFSC Criciuma");
  delay(4000);
  lcd.init();
  
}
void loop()
{
  
 
 lertemp_e_estoque();
 printa_valores();
 delay(1000);
  
}
void lertemp_e_estoque(){
  
  //firebase
  if (Firebase.getFloat(firedata, "/Dados/temperatura")) {    // On successful Write operation, function returns 1  
    
    val_temp = firedata.floatData();

    Serial.println("Temperatura recebida do firebase = ");
    Serial.print(val_temp);
    Serial.print(" °C");
    Serial.println("\n");
  
  }else {        
    Serial.println(firedata.errorReason());
  }
  
  if (Firebase.getFloat(firedata, "/Dados/estoque")) {    // On successful Write operation, function returns 1  
    
    val_estoque = firedata.floatData();
    val_estoque = (val_estoque/1024)*100;
    
    Serial.println("Estoque recebido do firebase = ");
    Serial.print(val_estoque);
    Serial.print(" %");
    Serial.println("\n");
  
  }else {        
    Serial.println(firedata.errorReason());
  }
}
void printa_valores(){

  lcd.setCursor(0,0);
  lcd.print("Temperatura: ");
  lcd.print(val_temp);
  lcd.print(" C");
  
  lcd.setCursor(0,2);
  lcd.print("Estoque: ");
  lcd.print(val_estoque);
  lcd.print(" %");

}
