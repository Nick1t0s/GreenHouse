#define minTemp 20
#define maxTemp 30
#define minSH 100
#define minLight 200
#define minWaterLevel 250
uint32_t timer = 0;
uint32_t timerReg = 0;   // переменная таймера
#define T_PERIOD 15000  // период переключения
#define R_PERIOD 500

int dataPin = 4;    // к выводу 14 регистра SD
int clockPin = 6;  // к выводу 11 регистра (SH_CP)
int latchPin = 5;  // к выводу 12 регистра (ST_CP)

byte devic;

// Пример отправки и приёма структуры через Serial
// ОТПРАВИТЕЛЬ
// Ардуины соединены так:
// отправитель D11 -> приёмник D10
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX

#include "DHT.h"
#define DHTINPIN 7
#define DHTOUTPIN 8
DHT dhtin(DHTINPIN, DHT11);
DHT dhtout(DHTOUTPIN, DHT11);
/*
  Данный код позволяет принять данные, идущие из порта, в строку (String) без "обрывов"
*/
String strData = "";
boolean recievedFlag;



struct Str {
  float inT;
  float inH;
  float outT;
  float outH;
  long light;
  long soilHum;
  long waterLevel;
  long isWent;
  long isHot;
  long isLight;
  long isWater;
  long isAuto;
};
float inT=0;
float inH=0;
float outT=0;
float outH=0;
long light=0;
long soilHum=0;
long waterLevel=0;
long isWent=0;
long isHot=0;
long isLight=0;
long isWater=0;
long isAuto=0;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	digitalWrite(latchPin, LOW);
  dhtin.begin();
  dhtout.begin();
}

void loop() {
  float inT=dhtin.readTemperature();
  float inH=dhtin.readHumidity();
  float outT=0;
  float outH=0;
  long light=analogRead(A0);
  long soilHum=analogRead(A1);
  long waterLevel=analogRead(A2);
//  long isWent=0;
//  long isHot=0;
//  long isLight=0;
//  long isWater=0;
//  long isAuto=0;
  // буфер на отправку
  Str buf;

  // заполняем
  buf.inT = inT;
  buf.inH = inH;
  buf.outT = outT;
  buf.outH = outH;
  buf.light = light;
  buf.soilHum = soilHum;
  buf.waterLevel = waterLevel;
  buf.isWent = isWent;
  buf.isHot = isHot;
  buf.isLight = isLight;
  buf.isWater = isWater;
  buf.isAuto = isAuto;


  // отправляем родным write()
  // указываем ему буфер-структуру, но приводим тип к byte*
  // размер можно указать через sizeof()
  if (millis() - timer >= T_PERIOD) { // таймер на millis()
    timer = millis(); // сброс
    mySerial.write((byte*)&buf, sizeof(buf));
    Serial.println("sendDone");
    // действие с counter - наш i-ый светодиод например
  }
  while (mySerial.available() > 0) {         // ПОКА есть что то на вход    
    strData += (char)mySerial.read();        // забиваем строку принятыми данными
    recievedFlag = true;                   // поднять флаг что получили данные
    delay(2);                              // ЗАДЕРЖКА. Без неё работает некорректно!
    Serial.println("dsgffg");
    
  }
  if (recievedFlag) {                      // если данные получены
//    Serial.println(strData);          
    Serial.println("ds43gffgfdsgshdhgfhhjgfdf");                // опустить флаг
    Serial.print(strData);
    if(strData=="onWent"){
      isWent=1;
      Serial.println("onWent");
    }
    else if(strData=="onHot"){
      isHot=1;
      Serial.println("onHot");
    }
    else if(strData=="onLight"){
      isLight=1;
      Serial.println("onLight");
    }
    else if(strData=="onWater"){
      isWater=1;
      Serial.println("onWater");
    }
    else if(strData=="onAuto"){
      isAuto=1;
      Serial.println("onAuto");
    }
    else if(strData=="offWent"){
      isWent=0;
      Serial.println("offWent");
    }
    else if(strData=="offHot"){
      isHot=0;
      Serial.println("offHot");
    }
    else if(strData=="offLight"){
      isLight=0;
      Serial.println("offLight");
    }
    else if(strData=="offWater"){
      isWater=0;
      Serial.println("offWater");
    }
    else if(strData=="offAuto"){
      isAuto=0;
      Serial.println("offAuto");
    }
    strData = "";                          // очистить
    recievedFlag = false;  
  }

  if (isAuto==1){
    if (inT>maxTemp){
      isWent=1;
    } 
    else{
      isWent=0;
    }
    if (inT<minTemp){
      isHot=1;
    }
    else{
      isHot=0;
    }
    if (soilHum>minSH){
      isWater=1;
    }
    else{
      isWater=0;
    }
    if (light>minLight){
      isLight=1;
    }
    else{
      isLight=0;
    }
  }
  devic=0;

  if(isWent==1){
    bitWrite(devic, 7, HIGH); // При bitPos=0 получим B00000001, при bitPos=1 - B00000010, при bitPos=2 - B00000100 и т.д.
  }
  else{
    bitWrite(devic, 7, LOW); // При bitPos=0 получим B00000001, при bitPos=1 - B00000010, при bitPos=2 - B00000100 и т.д.
  }

  if(isHot==1){
    bitWrite(devic, 3, HIGH); // При bitPos=0 получим B00000001, при bitPos=1 - B00000010, при bitPos=2 - B00000100 и т.д.
  }
  else{
    bitWrite(devic, 3, LOW); // При bitPos=0 получим B00000001, при bitPos=1 - B00000010, при bitPos=2 - B00000100 и т.д.
  }

  if(isLight==1){
    bitWrite(devic, 5, HIGH); // При bitPos=0 получим B00000001, при bitPos=1 - B00000010, при bitPos=2 - B00000100 и т.д.
  }
  else{
    bitWrite(devic, 5, LOW); // При bitPos=0 получим B00000001, при bitPos=1 - B00000010, при bitPos=2 - B00000100 и т.д.
  }

  if(isWater==1){
    bitWrite(devic, 1, HIGH); // При bitPos=0 получим B00000001, при bitPos=1 - B00000010, при bitPos=2 - B00000100 и т.д.
  }
  else{
    bitWrite(devic, 1, LOW); // При bitPos=0 получим B00000001, при bitPos=1 - B00000010, при bitPos=2 - B00000100 и т.д.
  }
//  bitWrite(devic, 4, LOW); // При bitPos=0 получим B00000001, при bitPos=1 - B00000010, при bitPos=2 - B00000100 и т.д.
//  bitWrite(devic, 3, LOW); // При bitPos=0 получим B00000001, при bitPos=1 - B00000010, при bitPos=2 - B00000100 и т.д.
//  bitWrite(devic, 2, LOW); // При bitPos=0 получим B00000001, при bitPos=1 - B00000010, при bitPos=2 - B00000100 и т.д.
//  bitWrite(devic, 0, LOW); // При bitPos=0 получим B00000001, при bitPos=1 - B00000010, при bitPos=2 - B00000100 и т.д.
//  Serial.println(devic,BIN);
  Serial.println(isWent);
  Serial.println(isHot);
  Serial.println(isLight);
  Serial.println(isWater);
  if (millis() - timerReg >= R_PERIOD) {
    timerReg=millis();
    delay(100);
    digitalWrite(latchPin, LOW); // начинаем передачу данных
	  // устанавливаем нужный байт
	  shiftOut(dataPin, clockPin, LSBFIRST, devic);
	  digitalWrite(latchPin, HIGH); // прекращаем передачу данных
//    Serial.println("fdsgsfdgfdg");
  }
}