// Proyecto: Termometro Digital
// Autor: Alberto Gonzalez
// Version: 0.2


#include <math.h>

const int Rc = 10000; //valor de la resistencia
const int Vcc = 5;
const int SensorPIN = A0;

float A = 1.11492089e-3;
float B = 2.372075385e-4;
float C = 6.954079529e-8;

float K = 2.5; //factor de disipacion en mW/C

void setup()
{
  Serial.begin(9600);
}

void loop() 
{
  float raw = analogRead(SensorPIN);
  float V =  raw / 1024 * Vcc;

  float R = (Rc * V ) / (Vcc - V);
  

  float logR  = log(R);
  float R_th = 1.0 / (A + B * logR + C * logR * logR * logR );

  float kelvin = R_th - V*V/(K * R)*1000;
//  float celsius = kelvin - 273.15;


  Serial.print("T = ");
//  Serial.print(celsius);
  Serial.print(converKelvinCelsius(kelvin));
  Serial.print("C\n");
  delay(2500);
}

// Funcion que transforma los valores leidos a kelvin, usando la formula Stein Hart
float steinHart(float raw){
  float V =  raw / 1024 * Vcc;
  float R = (Rc * V ) / (Vcc - V);
  float logR  = log(R);
  float R_th = 1.0 / (A + B * logR + C * logR * logR * logR );
  float kelvin = R_th - V*V/(K * R)*1000;
  return kelvin;
}

// Funcion que convierte los kelvin a grados
float converKelvinCelsius(float dato){
  float celsius = dato - 273.15;
  return celsius;
}
