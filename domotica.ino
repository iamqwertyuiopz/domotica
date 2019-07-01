#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
 
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

}
void loop() 
{
 
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
