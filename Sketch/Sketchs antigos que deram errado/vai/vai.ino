/*
 * MUMID prototype 2021 
 * 
 * Placa 2 - interface/ponto/cadastro
 * 
 * by: estudante Thales 3° ano IFSC campus Criciúma
 * 
 */

#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//LCD

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4); 


//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

//WiFi
#define WIFI_SSID "Juliana 2.4G"
#define WIFI_PASSWORD "lucas2001"

//chave API
#define API_KEY "AIzaSyDTZ_oWrqZ0dypOJc4z1P-4KnuSiG3CFd0"

//URL
#define DATABASE_URL "https://mumid-prototype-23da6-default-rtdb.firebaseio.com/" 

//objeto firebase
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
String statuscadastro = "";
int estado = 0;
float floatValue;
bool signupOK = false;
int intacucar = 0;

void setup() {
  Serial.begin(115200);

  //iniciando conexão WiFi
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  //configurando API
   config.api_key = API_KEY;

  //configurando URL
  config.database_url = DATABASE_URL;
  
  //logando
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);


  LCD();

}
void LCD(){
  
  lcd.init();

  lcd.setCursor(5,0);
  lcd.backlight();
  lcd.print("MUMID 2022");
  lcd.setCursor(3,2);
  lcd.print("IFSC Criciuma");
  delay(4000);
  lcd.init();

  lcd.clear();
}

void loop() {

  

  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
   
    sendDataPrevMillis = millis();

   if (Firebase.RTDB.getString(&fbdo, "/pontes/app")) {
        if (fbdo.dataType() == "string") {
          
          statuscadastro = fbdo.stringData();
        
          Serial.println("Status app: ");
          Serial.print(statuscadastro);


           if(statuscadastro == "1"){
    
              Serial.println("cadastrar");

              lcd.clear();
  
              lcd.setCursor(0,1);
              lcd.print("APROXIME O CARTAO");
              lcd.setCursor(0,2);
              lcd.print("NO SENSOR");
  
              delay(4000);  
              lcd.clear();

           }else{}
        } 
      }else {
              Serial.println(fbdo.errorReason());
      }

      if(statuscadastro == "0"){
          if (Firebase.RTDB.getFloat(&fbdo, "/status/1forno1")) {
                      if (fbdo.dataType() == "float") {
                      floatValue = fbdo.floatData();
                      Serial.println(floatValue);

                      lcd.setCursor(0,0);
                      lcd.print("Forno 1: ");
                      lcd.print(floatValue);
                      lcd.print(" C");
        
                      }
                     }else {
                            Serial.println(fbdo.errorReason());
                      }
    
          if (Firebase.RTDB.getInt(&fbdo, "/status/acucar")) {
            int acucar = 0;
            
                  if (fbdo.dataType() == "int") {
                       intacucar = fbdo.intData();
                       Serial.println(intacucar);

                        acucar = intacucar/10.24;
                        
                       lcd.setCursor(0,2);
                       lcd.print("Acucar: ");
                       lcd.print(acucar);
                       lcd.print(" %");
        
                  }
          }else {
              Serial.println(fbdo.errorReason());
          }
    
       }else{}
  }
        
delay(100);
}
