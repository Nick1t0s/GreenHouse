void saveData(String data){
  String filenm=getFileName();
  isPhoto = takeSavePhoto(filenm+".jpg");
  isWritedFile = writeFile(SD_MMC,converter(filenm+".txt"),converter(data));
}