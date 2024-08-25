
void initNTP(int timeZone){
  timeClient.begin();
  timeClient.setTimeOffset(timeZone*3600);
}

String getFileName(){
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  String currentTime=String(timeClient.getHours())+"_"+String(timeClient.getMinutes())+"_"+String(timeClient.getSeconds());
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  int currentDay = timeClient.getDay();
  int currentMonth = ptm->tm_mon+1;
  Serial.print("Month: ");
  Serial.println(currentMonth);
  int currentYear = ptm->tm_year+1900;
  Serial.print("Year: ");
  Serial.println(currentYear);
  String currentDate = "/"+String(currentYear) + "-" + String(currentMonth) + "-" + String(currentDay);
  String simpleName=currentDate+" "+currentTime;
  return simpleName;
}