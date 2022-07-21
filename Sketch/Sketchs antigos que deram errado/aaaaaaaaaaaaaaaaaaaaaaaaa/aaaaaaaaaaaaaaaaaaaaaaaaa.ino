/*
 * MUMID prototype 2021 
 * 
 * Placa 2 - interface/ponto/cadastro
 * 
 * by: estudante Thales 3° ano IFSC campus Criciúma
 * 
 */


#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

//WiFi

#define WIFI_SSID "Juliana 2.4G"
#define WIFI_PASSWORD "lucas2001"

//firebase

#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

#define API_KEY "AIzaSyDTZ_oWrqZ0dypOJc4z1P-4KnuSiG3CFd0" // chave API
#define DATABASE_URL "https://mumid-prototype-23da6-default-rtdb.firebaseio.com/" //URL database

//login firebase
#define USER_EMAIL "thales.v2001@aluno.ifsc.edu.br"
#define USER_PASSWORD "lucas2001"

FirebaseData fbdo; //objeto para biblioteca

FirebaseAuth auth;
FirebaseConfig config;

//tempo de envio do pacote
unsigned long sendDataPrevMillis = 0;
unsigned long count = 0;

void setup()
{

  Serial.begin(115200);

//inicia conexão wifi
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

//configura dados firebase

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  
  config.api_key = API_KEY;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  config.database_url = DATABASE_URL;

  config.token_status_callback = tokenStatusCallback;

//inicia firebase

  Firebase.begin(&config, &auth);

  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);
}
void loop()
{

  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
    {
    sendDataPrevMillis = millis();

    Serial.printf("Get int... %s\n", Firebase.RTDB.getInt(&fbdo, F("/status/1forno1")) ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
    
    FirebaseJson json;

    if (count == 0)
    {
      json.set("value/round/" + String(count), F("cool!"));
      json.set(F("value/ts/.sv"), F("timestamp"));
      Serial.printf("Set json... %s\n", Firebase.RTDB.set(&fbdo, F("/test/json"), &json) ? "ok" : fbdo.errorReason().c_str());
    }
    else
    {
      json.add(String(count), F("smart!"));
      Serial.printf("Update node... %s\n", Firebase.RTDB.updateNode(&fbdo, F("/test/json/value/round"), &json) ? "ok" : fbdo.errorReason().c_str());
    }

    Serial.println();

    count++;
    }
}
