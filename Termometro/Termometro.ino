// Programa que lee la temperatura y la humedad
// Dependiendo de la temperatura enciende un led
// Luego la muestra la temperatura por la lcd


/*
La humedad ideal para el hogar es de 50% con una temperatura de 20 grados
 */


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



void setup(){
    // Inicializamos el puerto serie (Modo debug)
    Serial.begin( 9600);
    // Inicializamos el lcd
    lcd.begin(16, 2);
    // Informacion que sacara el lcd
    lcd.print("Hello, World!");
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
    float umbral_frio = 20.0;
    float umbral_calor = 27.0;
    float umbral_seco = 40.0;
    float umbral_humedo = 69.9; 
    float temperature;
    float humidity;
    digitalWrite(ledPinRojo , LOW);
    digitalWrite(ledPinVerde , LOW);
    digitalWrite(ledPinAzul , LOW);


    
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
        Serial.println("Hace frio");
        digitalWrite(ledPinAzul , HIGH);
        delay(1000);
    }

    if(temperature >= umbral_calor){
        Serial.println("Hace calor");
        digitalWrite(ledPinRojo , HIGH);
        delay(1000);
    }

    if(temperature >= umbral_frio && temperature <= umbral_calor){
        Serial.println("Temperatura de confort");
        digitalWrite(ledPinVerde , HIGH);
        delay(1000);
    }

    if(humidity < umbral_seco){
        Serial.println("El ambiente es seco");
    }

    if(humidity > umbral_humedo){
        Serial.println("El ambiente es bastante humedo");
    }

    if(humidity >= umbral_seco && humidity <= umbral_humedo){
        Serial.println("La humedad en el ambiente esta entre los valores correctos");
    }
    
    }

    lcd.setCursor(0, 1);
    lcd.print(millis() / 1000);


    //delay(10000);
}
