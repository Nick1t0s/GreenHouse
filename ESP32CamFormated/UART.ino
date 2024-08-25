SoftwareSerial mySerial(12345, 16); // RX, TX

String strData;
boolean recievedFlag;

void getData(){
  while (Serial.available() > 0) {         // ПОКА есть что то на вход    
    strData += (char)Serial.read();        // забиваем строку принятыми данными
    recievedFlag = true;                   // поднять флаг что получили данные
    delay(2);                              // ЗАДЕРЖКА. Без неё работает некорректно!
  }
  if (recievedFlag) {                      // если данные получены
    Serial.println(strData);               // вывести
    saveData(strData);
    if (ftp.isConnected()){
    listDir(SD_MMC,"/",0);
    }
    strData = "";                          // очистить
    recievedFlag = false;                  // опустить флаг
  }
}