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
  #include <ESP8266WiFi.h> //WiFi
#endif

#include <Firebase_ESP_Client.h> //Firebase

#include <SPI.h> // RFID
#include <MFRC522.h> // RFID

#include <Wire.h> //LCD
#include <LiquidCrystal_I2C.h> //LCD

#include "addons/TokenHelper.h" //token
#include "addons/RTDBHelper.h" //RTDB

  //RFID

constexpr uint8_t RST_PIN = D3;    //pino reset
constexpr uint8_t SS_PIN = D4;     // pino ss

MFRC522 rfid(SS_PIN, RST_PIN); //associando a classe rfid
MFRC522::MIFARE_Key key;

String tag; //variavel para id da tag reconhecida

  //LCD

LiquidCrystal_I2C lcd(0x27,20,4); 


  //WiFi
  
#define WIFI_SSID "Juliana 2.4G"
#define WIFI_PASSWORD "lucas2001"

  //Firebase (acesso com conta anonima)
  
#define API_KEY "AIzaSyDTZ_oWrqZ0dypOJc4z1P-4KnuSiG3CFd0" //chave API
#define DATABASE_URL "https://mumid-prototype-23da6-default-rtdb.firebaseio.com/" //URL

FirebaseData fbdo; //objeto firebase dados

FirebaseAuth auth; //objeto firebase autenticação
FirebaseConfig config; //objeto firebase configuração login

unsigned long sendDataPrevMillis = 0;

String statuscadastro = "";

int estado = 0;

bool signupOK = false;

int intacucar = 0;
float forno1;

void setup() {

    //iniciando comunicação serial
  
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

  config.api_key = API_KEY; //configurando API
  config.database_url = DATABASE_URL; //configurando URL
  
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

    if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
   
    sendDataPrevMillis = millis();

    }
    
    //inicia módulo RFID
    
  SPI.begin(); //comunicação SPI
  rfid.PCD_Init(); //inicia MFRC522

  iniciaprog();

}
void iniciaprog(){
  
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

  lcd.clear();

  validacao_tag();
  
  if(statuscadastro == "1"){
    
    lcd.setCursor(0,1);
    lcd.print("APROXIME O CARTAO");
    lcd.setCursor(0,2);
    lcd.print("NO SENSOR");
  
    delay(1000);
  
    cadastra_tag();

    delay(1000);
    
  }else{
    
   imprimefirebase();
  }
        
  delay(10);

}


void validacao_tag(){

    if (Firebase.RTDB.getString(&fbdo, "/pontes/app")) {
      
      if (fbdo.dataType() == "string") {
          
        statuscadastro = fbdo.stringData();
        
        Serial.println("Status app: ");
        Serial.print(statuscadastro);

      } 
    }else{
      Serial.println(fbdo.errorReason());
     }       
}

void cadastra_tag(){

    //leitura tag
    
  do{
    
    if ( ! rfid.PICC_IsNewCardPresent())
    return;
    if (rfid.PICC_ReadCardSerial()) {
      for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    
      Serial.println(tag);

      delay(100);
      
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
  }while(tag == "");
  
    // envia tag para firebase

   Serial.println("enviando tag para firebase");
   
   Serial.println(tag);

    if (Firebase.RTDB.setString(&fbdo, "pontes/esp", tag)){
      Serial.println("Enviado com sucesso !!!");
      Serial.println("Caminho: " + fbdo.dataPath());
      Serial.println("Tipo: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    } 
       
  delay(4000);  

  tag = "";
  
}

void imprimefirebase(){
    
    if (Firebase.RTDB.getFloat(&fbdo, "/status/1forno1")) {
      
      if (fbdo.dataType() == "float") {
        
        forno1 = fbdo.floatData();
        Serial.println();
        Serial.println(forno1);

        lcd.setCursor(0,0);
        lcd.print("Forno 1: ");
        lcd.print(forno1);
        lcd.print(" C");
        
       }
    }else{
      Serial.println(fbdo.errorReason());
    }
    
      if (Firebase.RTDB.getInt(&fbdo, "/status/acucar")) {
            
        if (fbdo.dataType() == "int") {
          
          intacucar = fbdo.intData();
          Serial.println(intacucar);
        
          lcd.setCursor(0,2);
          lcd.print("Acucar: ");
          lcd.print(intacucar/10.24);
          lcd.print(" %");
        
        }
     }else {
        Serial.println(fbdo.errorReason());
      }
      
    delay(1000);
}
