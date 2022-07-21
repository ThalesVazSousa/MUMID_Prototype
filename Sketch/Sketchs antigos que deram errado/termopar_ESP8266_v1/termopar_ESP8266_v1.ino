#include <ESP8266WiFi.h>

//MAX31855 
#include <SPI.h>
#include "Adafruit_MAX31855.h"

#define MAXDO   14
#define MAXCS   13
#define MAXCLK  12

Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

void setup() {
  
  //iniciar monitor serial
  Serial.begin(115200);

}

void loop() {

  Serial.println(thermocouple.readCelsius());
  delay(5000);

} 
