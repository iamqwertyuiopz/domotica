/* Los pines 9, 10, 11, 12, 13; serán ocupados por el módulo RFID el cual al activarse activará "puertaGaraje" (pin 2)

*/
/*
int puertaGaraje = 2;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}*/

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
 
#define SS_PIN 53
#define RST_PIN 10
#define LED_G 7 // LED verde pin
#define LED_R 8 // LED rojo
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Crea instancia MFRC522.
Servo myServo; //Nombre de servo
 
void setup() 
{
  Serial.begin(9600);   // Inicia comunicacion serial
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  myServo.attach(3); //servo pin
  myServo.write(0); //servo posicion inicial
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
    myServo.write(90);
    delay(5000);
    myServo.write(0);
    digitalWrite(LED_G, LOW);
  }
 
 else   {
    Serial.println(" Acceso Denegado");
    digitalWrite(LED_R, HIGH);
    delay(1000);
    digitalWrite(LED_R, LOW);
  }
} 
