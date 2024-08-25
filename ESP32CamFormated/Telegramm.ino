FastBot bot(BOT_TOKEN);

void initBot(){
  bot.attach(newMsg);
}

void checkBot(){
  bot.tick();
}

void newMsg(FB_msg& msg){
  Serial.print(msg.chatID);     // ID чата 
  Serial.print(", ");
  Serial.print(msg.username);   // логин
  Serial.print(", ");
  Serial.println(msg.text);     // текст
  switch (msg.text) {
  case "//start":
    aStart(msg);
  case 1:
    break;
  case 2:
    break;
  default:
    break;
  }
}

void aStart(FB_msg& msg){
  bot.sendMessage("Приветствую вас в боте для управления теплицей\nВы можете управлять теплицей при помощи кнопок и комманд", msg.chatID);
  bot.showMenu("Получить последние данные \t Запросить новые данные \t Запросить фото \n Получить справку");
}