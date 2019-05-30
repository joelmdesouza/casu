//************************************************************//
//          CASU - Controle e Automacao Submarina             //
//Eduardo Luis dos Santos                                     //
//Erico de Castro Rodrigues                                   //
//Joel Magalhaes de Souza                                     //
//Lucas Emanuel do Rego                                       //
//Wesley Okumura Shenci                                       //
//************************************************************//
//Incluao da biblioteca de controle do servo motor
#include <Servo.h>

//DHT
#include "DHT.h"

//DS
#include <OneWire.h>
#include <DallasTemperature.h>

#define DHTPIN A2     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
//FIM DHT

//DS
#define ONE_WIRE_BUS 10

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);
DeviceAddress sensor1;
//FIM DS

//Variavel usada para verificar inico de comando
int STX = 255;

//Variavel para armazenar cada byte no momento da leitura
int lerByte = 0;

//Variavel indicando a porta do motor DC 
int MOTOR_PORT=9;

//Variaveis referentes aos servo motores
Servo SERVO_1;
int SERVO_1_PORT=5;

Servo SERVO_2;
int SERVO_2_PORT=11;

Servo SERVO_EMBOLO;
int SERVO_EMBOLO_PORT=12;

void setup(){
  dht.begin();
  
  //Configura motor DC
  pinMode(MOTOR_PORT,OUTPUT);
  //Configura servo motor 1
  SERVO_1.attach(SERVO_1_PORT);
  SERVO_1.write(90);
  //Configura servo motor 2
  SERVO_2.attach(SERVO_2_PORT);
  SERVO_2.write(90);
  //
  SERVO_EMBOLO.attach(SERVO_EMBOLO_PORT);
  SERVO_EMBOLO.write(25);
  //Configura comunicacao serial
  Serial.begin(9600);
  sensors.begin();
  //Serial.print(sensors.getDeviceCount(), DEC);
  sensors.getDeviceCount();
  //Serial.println(" sensores.");
  if (!sensors.getAddress(sensor1, 0)) 
     //Serial.println("Sensores nao encontrados !"); 
  // Mostra o endereco do sensor encontrado no barramento
  //Serial.print("Endereco sensor: ");
  mostra_endereco_sensor(sensor1);
  //Serial.println();
}

void mostra_endereco_sensor(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // Adiciona zeros se necessário
    if (deviceAddress[i] < 16) {
      //Serial.print("0");
    }
    //Serial.print(deviceAddress[i], HEX);
    deviceAddress[i];
  }
}

void loop(){
  //SERVO_EMBOLO.write(25);
  //delay(4000);
  //SERVO_EMBOLO.write(165);
  //delay(4000);
  //Criei um protocolo de comunicacao com 3 bytes, sera descrito sequencialmente abaixo
  //Antes de ler os dados da serial verifica se tem pelo menos 3 bytes
  if (Serial.available() >= 3) {
      lerByte = Serial.read();
      //O primeiro byte e o STX, valor que indica inicio de pacote de comunicacao
      if(lerByte==STX){
          lerByte = Serial.read();
          //O segundo byte indica a porta  
          if(lerByte==MOTOR_PORT){
              lerByte = Serial.read();
              //O terceiro byte indica o valor a ser enviado a respectiva porta
              digitalWrite(MOTOR_PORT,lerByte);    
          }else if(lerByte==SERVO_1_PORT){
              lerByte = Serial.read();
              SERVO_1.write(lerByte);    
          }else if(lerByte==SERVO_2_PORT){
              lerByte = Serial.read();
              SERVO_2.write(lerByte);      
          }else if(lerByte==SERVO_EMBOLO_PORT){
              lerByte = Serial.read();
              SERVO_EMBOLO.write(lerByte);      
          }  
      }       
  }
  
  float humidade = dht.readHumidity();
  float temperaturaDHT = dht.readTemperature();
  
  // Le a informacao do sensor
  sensors.requestTemperatures();
  float tempDS = sensors.getTempC(sensor1);
  
  Serial.print("#;");
  Serial.print(temperaturaDHT);
  Serial.print(";");
  Serial.print(humidade);
  Serial.print(";");
  Serial.print(tempDS);
  Serial.println(";$");
}
