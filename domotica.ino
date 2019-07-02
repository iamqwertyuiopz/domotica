#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11

static const int DHT_SENSOR_PIN = 6;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

#define SS_PIN 53
#define RST_PIN 10
#define LED_G 7 // LED verde pin
#define LED_R 8 // LED rojo
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Crea instancia MFRC522.
Servo puertaGaraje; //Nombre de servo

void setup() 
{
  Serial.begin(9600);   // Inicia comunicacion serial
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  puertaGaraje.attach(3); //servo pin
  puertaGaraje.write(0); //servo posicion inicial
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  Serial.println("Pase la tarjeta");
  Serial.println();
  pinMode(22,OUTPUT);

}
static bool measure_environment( float *temperature, float *humidity ){
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

void loop() 
{
 float temperature;
  float humidity;

  /* Measure temperature and humidity.  If the functions returns
     true, then a measurement is available. */
  if( measure_environment( &temperature, &humidity ) == true )
  {
    Serial.print( "T = " );
    Serial.print( temperature, 1 );
    Serial.print( " deg. C, H = " );
    Serial.print( humidity, 1 );
    Serial.println( "%" );

    if (temperature > 26){
      digitalWrite(22,HIGH);
    }
    else if( temperature<=26){
      digitalWrite(22,LOW);
    }
  }
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Muestra UID en monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensaje : ");
  content.toUpperCase();
  if (content.substring(1) == "26 EC 36 AC") //ID del tag con acceso
  {
    Serial.println("Acceso Correcto");
    Serial.println();
    delay(500);
    digitalWrite(LED_G, HIGH);
    delay(300);
    puertaGaraje.write(90);
    delay(5000);
    puertaGaraje.write(0);
    digitalWrite(LED_G, LOW);
  }
 
 else   {
    Serial.println(" Acceso Denegado");
    digitalWrite(LED_R, HIGH);
    delay(1000);
    digitalWrite(LED_R, LOW);
  }
} 
