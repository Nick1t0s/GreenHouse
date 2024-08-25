void initFTP(){
  ftp.OpenConnection();
  ftp.ChangeWorkDir("/");
}

void closeFTP(){
  ftp.CloseConnection();
}


void changeWorkDir(char* path){
  ftp.ChangeWorkDir(path);
}

void printNewFile(const char* name, const char* text){
  ftp.InitFile("Type A");
  ftp.NewFile(name);
  ftp.Write(text);
  ftp.CloseFile();
}

void writeNewFile(char* name, camera_fb_t* fb){
  ftp.InitFile("Type I");
  ftp.NewFile("octocat.jpg");
  ftp.WriteData(fb->buf,fb->len);
  ftp.CloseFile();
  esp_camera_fb_return(fb);
}

const char* converter(String a){
  const char* b = a.c_str();
  return b;
}

void readAndSendBigBinFile(fs::FS& fs, const char* path, ESP32_FTPClient ftpClient) {
    ftpClient.InitFile("Type I");
    ftpClient.NewFile(path);
    
    String fullPath = "/";
    fullPath.concat(path);
    Serial.printf("Reading file: %s\n", fullPath);

    File file = fs.open(fullPath);
    if (!file) {
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    
    while (file.available()) {
        // Create and fill a buffer
        unsigned char buf[1024];
        int readVal = file.read(buf, sizeof(buf));
        ftpClient.WriteData(buf,sizeof(buf));
    }
    ftpClient.CloseFile();
    file.close();
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
            String fullPath="/"+String(file.name());
            int leng=fullPath.length();
            String rs=fullPath.substring(leng-3,leng+1);
            Serial.println(rs);
            if (rs=="txt"){
              Serial.println("send txt file");
              printNewFile(file.name(),readFile(SD_MMC,fullPath.c_str()).c_str());
            }
            else{
              readAndSendBigBinFile(SD_MMC,fullPath.c_str(),ftp);
            }
            deleteFile(SD_MMC,fullPath.c_str());
        }
        file = root.openNextFile();
    }
}