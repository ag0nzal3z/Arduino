/* Programa que lee la temperatura y la humedad
Dependiendo de la temperatura enciende un led
Luego muestra la temperatura y humedad por el lcd
La humedad ideal para el hogar es de 50% con una temperatura de 20 grados
*/

// Proyecto: Termometro
// Autor: Alberto Gonzalez
// Version: 0.6


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
    // Inicializamos el puerto serie (Modo debug)
    Serial.begin( 9600);
    // Inicializamos el lcd
    lcd.begin(16, 2);
    // Definimos los pines de los led como salida
    pinMode(ledPinRojo , OUTPUT);
    pinMode(ledPinVerde , OUTPUT);
    pinMode(ledPinAzul , OUTPUT);

}

/*
 * Poll for a measurement, keeping the state machine alive.  Returns
 * true if a measurement is available.
 */
static bool measure_environment( float *temperature, float *humidity )
{
    static unsigned long measurement_timestamp = millis( );
  /* Measure once every four seconds. */
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
    digitalWrite(ledPinRojo , LOW);
    digitalWrite(ledPinVerde , LOW);
    digitalWrite(ledPinAzul , LOW);
    // Limpiar la pantalla lcd
    lcd.clear();
    // Lista con los mensajes
    String mensajes[] = {"T = ", "H = ", "Hace frio", "Hace calor", "Temperatura de confort", "El ambiente es seco", "El ambiente es bastante humedo", "La humedad en los valores correctos"};

    
    /* Measure temperature and humidity.  If the functions returns
     true, then a measurement is available. */
    if( measure_environment( &temperature, &humidity ) == true )
    {
    Serial.print( "T = " );
    Serial.print( temperature, 1 );
    Serial.print( " deg. C, H = " );
    Serial.print( humidity, 1 );
    Serial.println( "%" );

    if(temperature <= umbral_frio){
        Serial.println(mensajes[2]);
        digitalWrite(ledPinAzul , HIGH);
    }

    if(temperature >= umbral_calor){
        Serial.println(mensajes[3]);
        digitalWrite(ledPinRojo , HIGH);
    }

    if(temperature >= umbral_frio && temperature <= umbral_calor){
        Serial.println(mensajes[4]);
        digitalWrite(ledPinVerde , HIGH);
    }

    if(humidity < umbral_seco){
        Serial.println(mensajes[5]);
    }

    if(humidity > umbral_humedo){
        Serial.println(mensajes[6]);
    }

    if(humidity >= umbral_seco && humidity <= umbral_humedo){
        Serial.println(7);
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
    lcd.print(" C");

    lcd.setCursor(0,1);
    
    //Fila de abajo mensaje
    lcd.print("H = ");
    lcd.print(humidity);
    lcd.print(" %");

    delay(tiempoEspera);
    }
}
