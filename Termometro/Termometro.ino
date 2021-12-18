/* Programa que lee la temperatura y la humedad
Dependiendo de la temperatura enciende un led
Luego muestra la temperatura y humedad por el lcd
La humedad ideal para el hogar es de 50% con una temperatura de 20 grados
*/

// Proyecto: Termometro
// Autor: Alberto Gonzalez
// Version: 0.7


// Librerias del temperatura y humedad
#include <dht_nonblocking.h>
// Libreria lcd
#include <LiquidCrystal.h>

// Inicializamos la libreria lcd
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Agregamos los pin necesarios para el sensor de temperatura y humedad
#define DHT_SENSOR_TYPE DHT_TYPE_11
static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

// Agregamos los led
const int ledPinRojo = 6;
const int ledPinVerde = 5;
const int ledPinAzul = 3;

// Tiempo de espera entre bucles una vez encendido el led que corresponde
const long int tiempoEspera = 30000;

void setup(){
    // Inicializamos el lcd
    lcd.begin(16, 2);
    // Definimos los pines de los led como salida
    pinMode(ledPinRojo , OUTPUT);
    pinMode(ledPinVerde , OUTPUT);
    pinMode(ledPinAzul , OUTPUT);

}

// Funcion que realiza la medicion de temperatura y humedad 
static bool measure_environment( float *temperature, float *humidity )
{
    static unsigned long measurement_timestamp = millis( );
  // Toma mediciones cada 4 segundos
    if( millis( ) - measurement_timestamp > 3000ul )
    {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
        measurement_timestamp = millis( );
        return( true );
    }
    }
    return( false );
}

void loop(){
    // Variables
    const float umbral_frio = 20.0;
    const float umbral_calor = 27.0;
    const float umbral_seco = 40.0;
    const float umbral_humedo = 69.9; 
    float temperature;
    float humidity;
    String msgt;
    String msgh;
    digitalWrite(ledPinRojo , LOW);
    digitalWrite(ledPinVerde , LOW);
    digitalWrite(ledPinAzul , LOW);
    // Limpiar la pantalla lcd
    lcd.clear();
    // Lista con los mensajes
    String mensajes[] = {"T = ", "H = ", "Frio", "Calor", "Confort", "Seco", "Humedo", "Normal"};

    // Comprueba si hay mediciones disponibles
    if( measure_environment( &temperature, &humidity ) == true )
    {
    Serial.print( "T = " );
    Serial.print( temperature, 1 );
    Serial.print( " deg. C, H = " );
    Serial.print( humidity, 1 );
    Serial.println( "%" );

    if(temperature <= umbral_frio){
        msgt = mensajes[2];
        digitalWrite(ledPinAzul , HIGH);
    }

    if(temperature >= umbral_calor){
        msgt = mensajes[3];
        digitalWrite(ledPinRojo , HIGH);
    }

    if(temperature >= umbral_frio && temperature <= umbral_calor){
        msgt = mensajes[4];
        digitalWrite(ledPinVerde , HIGH);
    }

    if(humidity < umbral_seco){
        msgh = mensajes[5];
    }

    if(humidity > umbral_humedo){
        msgh = mensajes[6];
    }

    if(humidity >= umbral_seco && humidity <= umbral_humedo){
        msgh = mensajes[7];
    }

    // Conversion de los valores de float a int, para mostrarlos por el lcd
    int temp = int(temperature);
    int humi = int(humidity);
    int temperature = temp;
    int humidity = humi;
    
    // Informacion que mostrara el lcd 
    //Fila de arriba mensaje
    lcd.print("T = ");
    lcd.print(temperature);
    lcd.print("C ");
    lcd.print(msgt);

    lcd.setCursor(0,1);
    
    //Fila de abajo mensaje
    lcd.print("H = ");
    lcd.print(humidity);
    lcd.print("% ");
    lcd.print(msgh);

    delay(tiempoEspera);
    }
}
