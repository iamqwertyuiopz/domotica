#include <SPI.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(22, 23, 24, 25, 26, 27);  //Pines donde va conectada la pantalla (RS, E, D4, D5, D6, D7)
 float temperature;
  float humidity;

int vent=2;
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
Servo puertaPrincipal; //Nombre de servo
Servo garaje;

#include <Keypad.h>; // Con esta linea llamamos a la librería Keypad para que arduino reconozca el teclado
char contrasena[]="0999"; // En esta linea creamos la constante de cadena llamada contraseña (el numero 2222 puede ser cambiado)
char codigo[4]; // Creamos otra constante, en esta se guardaran los caracteres que presionemos en nuestro teclado matricial 4x4
int cont=0; // Esta es una variable que nos servirá más adelante

const byte ROWS = 4; //Este código se estable para definir que nuestro teclado tiene 4 filas 
const byte COLS = 4; //Este código se estable para definir que nuestro teclado tiene 4 columnas
  
char hexaKeys[ROWS][COLS] = //En el siguiente matriz se agregan en orden las teclas que posee nuestro teclado
{
 {'1','2','3','A'},
 {'4','5','6','B'},
 {'7','8','9','C'},
 {'*','0','#','D'}
};
  
byte rowPins[ROWS] = {42, 43, 44, 45}; // Estos son los pines de conexión de Arduino de las columnas
byte colPins[COLS] = {46, 47, 48, 49}; //Estos son los pines de conexión de Arduino de las filas
  
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); //En este punto el teclado se inicia y realiza el mapeo de sus teclas con respecto a la matriz colocada arriba
void setup() 
{
  pinMode(13, OUTPUT); //El pin 13 sera nuestra salida digital (así aprovecharemos el led que pose)
 Serial.begin(9600); //Se inicia el puerto serial
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  puertaPrincipal.attach(3); //servo pin
  puertaPrincipal.write(0); //servo posicion inicial
/*  garaje.attach(12);
  garaje.write(100);*/
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("ok");
  pinMode(vent,OUTPUT);
    pinMode(13,OUTPUT);
    lcd.begin(16,2);
    
}
static bool measure_environment( float *temperature, float *humidity ){
  static unsigned long measurement_timestamp = millis( );


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
  char customKey = customKeypad.getKey(); //esta linea indica que el carácter presionado se guardara en la variable customKey
 if (customKey != NO_KEY) //Con este siclo preguntamos si se presiono una tecla, si se presiono entonce el ciclo continua
 {
 codigo[cont]=customKey; //se guardaran los caracteres presionados en codigo[cont]
 Serial.print(codigo[cont]); //Se imprime en nuestro monitor serial lo que este guardado en codigo[cont]
 cont=cont+1; //incrementamos la variable cont (esto con el fin de tener los 4 dígitos que requerimos)
 if(cont==4) //Ahora comienza otro siclo if, en el cual se pregunta si ya hay 4 datos guardados y si es así entonce el siclo continua
  
 {
  
 if(codigo[0]==contrasena[0]&&codigo[1]==contrasena[1]&&codigo[2]==contrasena[2]&&codigo[3]==contrasena[3])
 {
  
 digitalWrite(13,!digitalRead(13)); //si lo anterior es verdad entonces se encenderá el led, si se ingresa nuevamente la contraseña entonces se apagara
 Serial.println("Password correcta"); //si la contraseña es correcta entonces se imprime ""Password correcta")
 /* garaje.write(0);
  delay(1000);
  garaje.write(90);
  delay(5000);
  garaje.write(-0);
  delay(500);
  garaje.write(90);*/
 }
 if(codigo!=contrasena) //empieza un nuevo ciclo, esta vez indicamos si el código es diferente que la contraseña siga con el siclo
 {
  
 Serial.println("Password incorrecta"); // se imprime que que la Password es incorrecta
 }
  
 cont=0; //resetear a 0 la variable cont
 }
 }

  if( measure_environment( &temperature, &humidity ) == true )
  {
 /* lcd.setCursor(0, 0);
  lcd.write("Temp: ");
  lcd.setCursor(7, 0);
  lcd.write(temperature);
  lcd.setCursor(10,0);
  lcd.write("º");
  lcd.setCursor(0, 1);
  lcd.write("Hum: ");
  lcd.setCursor(6, 1);
  lcd.write(humidity);
  lcd.setCursor(9,1);
  lcd.write("%"); */


    if (temperature > 34){
      digitalWrite(vent,HIGH);
    }
    else {
      digitalWrite(vent,LOW);
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
  if (content.substring(1) == "7B A1 32 A3" || content.substring(1) == "FC B5 D3 D3") //ID del tag con acceso
  {
    lcd.clear();
    lcd.setCursor(0, 0);
  lcd.write("Acceso Correcto");
    Serial.println("Acceso Correcto");
    Serial.println();
    delay(500);
    digitalWrite(LED_G, HIGH);
    delay(300);
    puertaPrincipal.write(90);
    delay(5000);
    puertaPrincipal.write(0);
    digitalWrite(LED_G, LOW);
    lcd.clear();
  }
 
 else   {
  lcd.setCursor(0, 0);
  lcd.write("Acceso Denegado");
    Serial.println(" Acceso Denegado");
    digitalWrite(LED_R, HIGH);
    alarma();
    digitalWrite(LED_R, LOW);
    lcd.clear();
  }
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.write("Temp: ");
  lcd.setCursor(7, 0);
  lcd.write(temperature);
  lcd.setCursor(10,0);
  lcd.write("º");
  lcd.setCursor(0, 1);
  lcd.write("Hum: ");
  lcd.setCursor(6, 1);
  lcd.write(humidity);
  lcd.setCursor(9,1);
  lcd.write("%");
  
  }
void alarma(){
  digitalWrite(13,HIGH);
      delay(500);
      digitalWrite(13,LOW);
      delay(500);
      digitalWrite(13,HIGH);
      delay(500);
      digitalWrite(13,LOW);
      delay(500);
}
