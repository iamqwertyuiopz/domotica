#include <SoftwareSerial.h>

SoftwareSerial BT(2, 3); // RX, TX
int luzBalcon = 5;
int luzGaraje = 8;
int luzSala = 13;
int luzCuarto = 9; 
int luzCocina = 11;
int luzFuera = 10;
const int ldrPin = A0;
byte sensorpir = 6;
int actSensor = 5;
char valor ;  //Variable para indicar que llega una orden

void setup() {
  // put your setup code here, to run once:
  pinMode(sensorpir,INPUT);
  BT.begin(9600);
  pinMode(luzSala, OUTPUT);
  pinMode(actSensor, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(luzCuarto, OUTPUT);
  pinMode(luzCocina, OUTPUT);
  pinMode(luzFuera, OUTPUT);
  pinMode(ldrPin, INPUT);
  Serial.begin(9600);
  digitalWrite(actSensor, HIGH);
  pinMode(luzGaraje, OUTPUT);
  pinMode(luzBalcon, OUTPUT);

}

void loop() {
  if (BT.available())  //Si el puerto serie (Bluetooth) está disponible
  {
    valor = BT.read();  //Lee el dato entrante via Bluetooth
  }
    if (valor == 'A')   
    {
      digitalWrite(luzFuera, HIGH);  
    }
    if (valor == 'B')  
    {
      digitalWrite(luzFuera, LOW);  
    }
    if (valor == 'C')   
    {
      digitalWrite(luzSala, HIGH);  
    }
    if (valor == 'D') 
    {
      digitalWrite(luzSala, LOW); 
    }
    if (valor == 'E')
    {
      digitalWrite(luzCocina, HIGH); 
    }
    if (valor == 'F') 
    {
      digitalWrite(luzCocina, LOW); 
    }
    if (valor == 'G')  
    {
      digitalWrite(luzCuarto, HIGH); 
    }
    if (valor == 'H')   
    {
      digitalWrite(luzCuarto, LOW);   
    }
    if (valor == 'K')   
    {
      digitalWrite(luzGaraje, HIGH);  
    }
    if (valor == 'L') 
    {
      digitalWrite(luzGaraje, LOW); 
    }
    if (valor == 'I')   
    {
      digitalWrite(luzBalcon, HIGH);  
    }
    if (valor == 'J') 
    {
      digitalWrite(luzBalcon, LOW); 
    }


    if (valor == 'X' ){
      if(digitalRead(sensorpir) == HIGH){
        alarma();
      
      delay(1000);
      digitalWrite(actSensor,LOW);
      }
      }
    if (valor == 'Z'){
      digitalWrite(7,LOW);
      digitalWrite(actSensor,HIGH);
    }
    
}

void alarma(){
  digitalWrite(7,HIGH);
      delay(500);
      digitalWrite(7,LOW);
      delay(500);
      digitalWrite(7,HIGH);
      delay(500);
      digitalWrite(7,LOW);
      delay(500);
}
