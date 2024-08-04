// Пример отправки и приёма структуры через Serial
// ПРИЁМНИК
// Ардуины соединены так:
// приёмник D10 -> отправитель D11

// структура для приёма
// должна соответствовать отпраляемой
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#define WIFI_SSID "ASUS21G"
#define WIFI_PASS "Asus164w"
#define BOT_TOKEN "6915453470:AAHwlUU2vuqXJR91FyCA2iZI9u3qrUMZgWY"
#include <FastBot.h>
FastBot bot(BOT_TOKEN);
#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
WiFiClient client;
#include<ESP8266WiFi.h>
char* host="192.168.1.42";
uint16_t port = 12345;

struct Str {
  float inT;
  float inH;
  float outT;
  float outH;
  int light;
  int soilHum;
  int waterLevel;
  int isWent;
  int isHot;
  int isLight;
  int isWater;
  int isAuto;
};

float inT=0;
float inH=0;
float outT=0;
float outH=0;
int light=0;
int soilHum=0;
int waterLevel=0;
int isWent=0;
int isHot=0;
int isLight=0;
int isWater=0;
int isAuto=0;


String resultToSend;


// создаём саму структуру
Str buf;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void setup() {
  pinMode(12,OUTPUT);
  digitalWrite(12,HIGH);
  delay(4000);
  digitalWrite(12,LOW);
  Serial.begin(9600);
  oled.init();        // инициализация
  oled.clear();       // очистка
  oled.setScale(2);   // масштаб текста (1..4)
  oled.home();        // курсор в 0,0
  connectWiFi();
  timeClient.begin();
  timeClient.setTimeOffset(10800);
  timeClient.update();
  oled.clear();
  oled.home();
  oled.setScale(1);
  String formattedTime = timeClient.getFormattedTime();
  oled.println(formattedTime);
  delay(2000);
  bot.attach(newMsg);
//  bot.attach(newMsg);
  Serial.swap();
}

void loop() {
  // читаем родным методом readBytes()
  // указываем ему буфер-структуру, но приводим тип к byte*
  // размер можно указать через sizeof()
  if (Serial.readBytes((byte*)&buf, sizeof(buf))) {
/*    Serial.swap();
    Serial.print("inT: ");
    Serial.println(buf.inT);
    Serial.print("inH: ");
    Serial.println(buf.inT);
    Serial.print("outT:");
    Serial.println(buf.outT);
    Serial.print("outH: ");
    Serial.println(buf.outH);
    Serial.print("light: ");
    Serial.println(buf.light);
    Serial.print("soilHum: ");
    Serial.println(buf.soilHum);
    Serial.print("waterLevel: ");
    Serial.println(buf.waterLevel);
    Serial.print("isWent");
    Serial.println(buf.isWent);
    Serial.print("isHot");
    Serial.println(buf.isHot);
    Serial.print("isLight");
    Serial.println(buf.isLight);
    Serial.print("isWater");
    Serial.println(buf.isWater);
    Serial.print("isAuto");
    Serial.println(buf.isAuto);
    delay(100);*/
    inT=buf.inT;
    inH=buf.inH;
    outT=buf.outT;
    outH=buf.outH;
    light=buf.light;
    soilHum=buf.soilHum;
    waterLevel=buf.waterLevel;
    isWent=buf.isWent;
    isHot=buf.isHot;
    isLight=buf.isLight;
    isWater=buf.isWater;
    isAuto=buf.isAuto;
//    Serial.swap();
    timeClient.update();
    oled.clear();
    oled.home();
//    oled.setScale(2);
//    oled.println("Lastest");
//    oled.println("data ");
    oled.setScale(1);
    oled.print("inT: ");
    oled.println(buf.inT);
    oled.print("inH: ");
    oled.println(buf.inH);
    oled.print("SH: ");
    oled.println(buf.soilHum);
    oled.print("isHot: ");
    oled.println(buf.isHot);
    String formattedTime = timeClient.getFormattedTime();
    oled.println(formattedTime);
    time_t epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime ((time_t *)&epochTime); 
    int monthDay = ptm->tm_mday;
    int currentMonth = ptm->tm_mon+1;
    int currentYear = ptm->tm_year+1900;
    String currentDate = String(monthDay)+"-"+String(currentMonth)+"-"+String(currentYear);
    resultToSend=currentDate+" "+timeClient.getFormattedTime()+"#"+String(buf.inT)+"#"+String(buf.inH)+"#"+String(buf.outT)+"#"+String(buf.outH)+"#"+String(buf.light)+"#"+String(buf.soilHum)+"#"+String(buf.waterLevel);
    if(!client.connect(host,port)){
      oled.println("Нет подключения");
      oled.println("к серверу");
    }
    if(client.connected()){
      client.print(resultToSend);
      delay(500);
      client.stop();
    }
    
  }
  bot.tick();
}

void connectWiFi() {
  int counter=0;
  oled.println("Connecting");
  delay(2000);
  oled.clear();
  oled.home();
  Serial.println();
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    oled.print(".");
    delay(500);
    counter+=1;
    if (counter==3){
      counter=0;
      oled.clear();
      oled.home();
    }
    if (millis() > 15000) ESP.restart();
  }
  Serial.println("Connected");
  oled.clear();
  oled.home();
  oled.println("Connceted");
  oled.println("Waiting data");
}
void newMsg(FB_msg& msg) {
  // выводим ID чата, имя юзера и текст сообщения
  bot.sendMessage(msg.text, msg.chatID);
  if (msg.text.substring(0,3)=="/on"){
    if (isAuto==0){
      bot.sendMessage("ok",msg.chatID);
      if (msg.text.substring(3)=="Went"){
        Serial.print("onWent");
        bot.sendMessage("Вентиляция включена",msg.chatID);
      }
      else if (msg.text.substring(3)=="Hot"){
        Serial.print("onHot");
        bot.sendMessage("Подогрев включен",msg.chatID);
      }
      else if (msg.text.substring(3)=="Light"){
        Serial.print("onLight");
        bot.sendMessage("Освещение включено",msg.chatID);
      }
      else if (msg.text.substring(3)=="Water"){
        Serial.print("onWater");
        bot.sendMessage("Полив включен",msg.chatID);
      }
      else if (msg.text.substring(3)=="Auto"){
        Serial.print("onAuto");
        bot.sendMessage("Автоуправление включено\nРучное управление невозможно",msg.chatID);
      }
      else{
        bot.sendMessage("Устройсто отсутствует",msg.chatID);
      }
    }
    else{
      bot.sendMessage("Невозможно изменить состояние устройств, т.к. включено автоуправление",msg.chatID);
    }
  }
  else if (msg.text.substring(0,4)=="off"){
    if (isAuto==0){
      bot.sendMessage("okk",msg.chatID);
      if (msg.text.substring(4)=="Went"){
        Serial.print("offWent");
        bot.sendMessage("Вентиляция выключена",msg.chatID);
      }
      else if (msg.text.substring(4)=="Hot"){
        Serial.print("offHot");
        bot.sendMessage("Подогрев выключен",msg.chatID);
      }
      else if (msg.text.substring(4)=="Light"){
        Serial.print("offLight");
        bot.sendMessage("Освещение выключено");
      }
      else if (msg.text.substring(4)=="Water"){
        Serial.print("offWater");
        bot.sendMessage("Полив отключен",msg.chatID);
      }
      else if (msg.text.substring(4)=="Auto"){
        Serial.print("offAuto");
        bot.sendMessage("Автоуправление отключено\nРучное управление возможно",msg.chatID);
      }
      else{
        bot.sendMessage("Устройсто отсутствует",msg.chatID);
      }
    }
    else{
      bot.sendMessage("Невозможно изменить состояние устройств, т.к. включено автоуправление",msg.chatID);
    }
  }
    
  else if (msg.text.substring(0,3)=="get"){
    if(msg.text.substring(4)=="inT"){
      bot.sendMessage(String(inT),msg.chatID);
    }
    else if(msg.text.substring(4)=="inH"){
      bot.sendMessage(String(inH),msg.chatID);
    }
    else if(msg.text.substring(4)=="outT"){
      bot.sendMessage(String(outT),msg.chatID);
    }
    else if(msg.text.substring(4)=="outH"){
      bot.sendMessage(String(outH),msg.chatID);
    }
    else if(msg.text.substring(4)=="light"){
      bot.sendMessage(String(light),msg.chatID);
    }
    else if(msg.text.substring(4)=="soilHum"){
      bot.sendMessage(String(soilHum),msg.chatID);
    }
    else if(msg.text.substring(4)=="waterLevel"){
      bot.sendMessage(String(waterLevel),msg.chatID);
    }
    else if(msg.text.substring(4)=="isWent"){
      bot.sendMessage(String(isWent),msg.chatID);
    }
    else if(msg.text.substring(4)=="isHot"){
      bot.sendMessage(String(isHot),msg.chatID);
    }
    else if(msg.text.substring(4)=="isLight"){
      bot.sendMessage(String(isLight),msg.chatID);
    }
    else if(msg.text.substring(4)=="isWater"){
      bot.sendMessage(String(isWater),msg.chatID);
    }
    else{
      bot.sendMessage("Устройтво отключено",msg.chatID);
    }
  }
  else{
    bot.sendMessage("Команда незарегестрированна",msg.chatID);
  }
}