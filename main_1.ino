#define THINGER_SERIAL_DEBUG
#include <ThingerESP32.h>
#include "secrets.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ANCHO_PANTALLA 128
#define ALTO_PANTALLA 64

#define OLED_RESET -1
#define DIRECCION_PANTALLA 0x3C

//Salidas 
//1 led para on y off
//led para indicar si se enviaron datos
//Pulsador
//Fotorresistencia
#define LED_ON 1
#define LED_IND 3
#define PULSADOR 19
#define FOTORES 21
#define MOTOR 4

Adafruit_SSD1306 display(ANCHO_PANTALLA,ALTO_PANTALLA,&Wire,OLED_RESET);

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void Comida_manual();
void Cerrar_fotorres();
void Cerrar_manual();
void Indicador();


//Esta funcion deja caer la comida por 3 segundos.
//En esta envia los datos a ThingerIO cuando se deja caer la comida
void Comida_manual(){
  if(digitalRead(PULSADOR) == HIGH){
    thing["ENCENDER_LED"] << digitalPin(LED_ON);
    digitalWrite(MOTOR, HIGH);
    delay(3000);
    digitalWrite(MOTOR, LOW);
  }
}


void Cerrar_fotorres(){
  if(digitalRead(FOTORES) == HIGH){
    digitalWrite(LED_IND,HIGH);
  }
  else{
    digitalWrite(LED_IND,LOW);
  }
}


void setup() {
  thing.add_wifi(SSID,SSID_PASSWORD);
  Serial.begin(9600);
  //Serial.begin(115200);
  
  pinMode(LED_ON,OUTPUT);
  pinMode(LED_IND,OUTPUT);
  pinMode(PULSADOR,INPUT);
  pinMode(FOTORES,OUTPUT);
  pinMode(MOTOR,OUTPUT);

  if(!display.begin(SSD1306_SWITCHCAPVCC, DIRECCION_PANTALLA)){
    Serial.println(F("Fallo en la asignación del SSD1306"));
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Prueba");
  display.display();
  
  Comida_manual();
  Cerrar_fotorres();
}

void loop() {
  thing.handle();

}
