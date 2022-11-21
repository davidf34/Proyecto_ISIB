/*
IMPORTANTE: Se debe agregar la libreria para el DHT y el ThingerESP32
Autor: Ing. David Felipe Sandoval Granados
FUNCION PARA ON/OFF DEL DISPENSADOR
1. Funcion para que el pulsador envie datos a la nube -> Cada que se abre el dispensador y mueve el motor
2. Funcion que detecte cuando la fotoresistencia percibe luz e indicar que ya no hay comida
3. Funcion que lea la humedad
4. Funcion para el sensor de ultrasonido (?)
*/



// Incluimos librería
#include <DHT.h>
#include <ThingerESP32.h>
#include "secrets.h"
#include <Wire.h>


#define LED_ESP32 2     //led de prueba para la placa ESP32
#define LED_ON 25       //led que indica el encendido del dispensador
#define LED_IND 3       //led que indica la apertura del dispensador manualmente
#define PULSADOR 19     //pulsador
#define FOTORES 21      //fotorresistencia
#define DHTTYPE DHT11
#define DHTPIN 22    //sensor de humedad
#define MOTOR 4
#define TRIG 16
#define ECHO 17

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);
int buttonState = 0;
bool buttonOut = false;
bool fotoRes = false;
float temperatura,humedad;

DHT dht(DHTPIN, DHTTYPE);


void setup() {
  thing.add_wifi(SSID,SSID_PASSWORD);
  dht.begin();
  
  pinMode(LED_ESP32,OUTPUT);
  pinMode(LED_ON,OUTPUT);
  pinMode(LED_IND,OUTPUT);
  pinMode(PULSADOR,INPUT);
 
 
  thing["led"] << digitalPin(LED_ESP32); 

  thing["button"] >> [](pson& out){
      out = buttonState;
  };
  thing["buttonLED"] >> [](pson& out){
      out = buttonOut;
  };

    //lectura del sensor de HUMEDAD
  /* thing["dht"]>>[](pson & out){
    out["temperatura"] = temperatura = dht.readTemperature();
    out["humedad"] = humedad = dht.readHumidity();
  } */


}

void loop() {

  digitalWrite(LED_ON, HIGH);
  buttonState = digitalRead(PULSADOR);
  fotoRes = digitalRead(FOTORES);


  if (buttonState == 0) {    // El pulsador es presionado
    buttonOut = false;
    digitalWrite(LED_IND, LOW); // Apaga el LED 
    
  }
  else {
    buttonOut = true;
    digitalWrite(LED_IND, HIGH); // Enciende el LED 
    digitalWrite(MOTOR, HIGH); //Gira el motor para servir comida
    delay(3500);
    digitalWrite(MOTOR, LOW);
    //ver si va acá o en setup
    thing["dht"]>>[](pson & out){
        out["temperatura"] = temperatura = dht.readTemperature();
        out["humedad"] = humedad = dht.readHumidity();
    };
    
  }
  //Revisar
  if(fotoRes == true){
    thing["fotorresistencia"] >> [](pson& out){
        out = fotoRes;
    };
  }

  thing["SONIC"] >> [] (pson& out){
      double duration, distance;
      digitalWrite(TRIG, LOW);  // Get Start
      delayMicroseconds(2); // stable the line 
      digitalWrite(TRIG, HIGH); // sending 10 us pulse
      delayMicroseconds(10); // delay 
      digitalWrite(TRIG, LOW); // after sending pulse wating to receive signals 
      duration = pulseIn(ECHO, HIGH); // calculating time 
      distance = (duration/2) / 29.1; // single path 
      out = distance;
  };
  

  thing.handle();
}
