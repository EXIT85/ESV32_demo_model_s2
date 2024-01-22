#define FILESYSTEM SPIFFS
// You only need to format the filesystem once
#define FORMAT_FILESYSTEM false
#if FILESYSTEM == FFat
#include <FFat.h>
#endif
#if FILESYSTEM == SPIFFS
#include <SPIFFS.h>
#endif

File fsUploadFile;
static bool fsOK;
//format bytes
String formatBytes(size_t bytes) {
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  } else {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}

String getContentType(String filename) {
  if (Server.hasArg("download")) {
    return "application/octet-stream";
  } else if (filename.endsWith(".htm")) {
    return "text/html";
  } else if (filename.endsWith(".html")) {
    return "text/html";
  } else if (filename.endsWith(".css")) {
    return "text/css";
  } else if (filename.endsWith(".js")) {
    return "application/javascript";
  } else if (filename.endsWith(".png")) {
    return "image/png";
  } else if (filename.endsWith(".gif")) {
    return "image/gif";
  } else if (filename.endsWith(".jpg")) {
    return "image/jpeg";
  } else if (filename.endsWith(".ico")) {
    return "image/x-icon";
  } else if (filename.endsWith(".xml")) {
    return "text/xml";
  } else if (filename.endsWith(".pdf")) {
    return "application/x-pdf";
  } else if (filename.endsWith(".zip")) {
    return "application/x-zip";
  } else if (filename.endsWith(".gz")) {
    return "application/x-gzip";
  }
  return "text/plain";
}

bool exists(String path){
  // bool yes = false;
  // File file = FILESYSTEM.open(path, "r");
  // if(!file.isDirectory()){
  //   yes = true;
  // }
  // file.close();
  return FILESYSTEM.exists(path);
}

bool handleFileRead(String path) {
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) {
    path += "index.htm";
  }
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (exists(pathWithGz) || exists(path)) {
    if (exists(pathWithGz)) {
      path += ".gz";
      Serial.println("handleFileRead: using gz file" + path);
    }
    File file = FILESYSTEM.open(path, "r");
    size_t fileSize = file.size();
    Serial.printf("FS File: %s, size: %s\n", path.c_str(), formatBytes(fileSize).c_str());
    Server.streamFile(file, contentType, 200);
    
    file.close();
    return true;
  }else{
    Serial.println("handleFileRead: file not found" + path);
  }
  return false;
}

void handleStatus() {
  Serial.println("handleStatus");
  String json;
  json.reserve(128);

  json = "{\"type\":\"";
  json += "SPIFFS";
  json += "\", \"isOk\":";
  if (fsOK) {
    
    json += F("\"true\", \"totalBytes\":\"");
    json += String(FILESYSTEM.totalBytes());
    json += F("\", \"usedBytes\":\"");
    json += String(FILESYSTEM.usedBytes());
    json += "\"";
  } else {
    json += "\"false\"";
  }
  json += F(",\"unsupportedFiles\":\"");
  //json += unsupportedFiles;
  json += "\"}";

  Server.send(200, "application/json", json);
}

void handleFileUpload() {
  if (Server.uri() != "/edit") {
    return;
  }
  HTTPUpload& upload = Server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) {
      filename = "/" + filename;
    }
    Serial.print("handleFileUpload Name: "); Serial.println(filename);
    fsUploadFile = FILESYSTEM.open(filename, "w");
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    Serial.print("handleFileUpload Data: "); Serial.println(upload.currentSize);
    if (fsUploadFile) {
      fsUploadFile.write(upload.buf, upload.currentSize);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {
      fsUploadFile.close();
      Serial.println("handleFileUpload File Close");
    }
    Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
  }
}

void handleFileDelete() {
  if (Server.args() == 0) {
    return Server.send(500, "text/plain", "BAD ARGS");
  }
  String path = Server.arg(0);
  Serial.println("handleFileDelete: " + path);
  if (path == "/") {
    return Server.send(500, "text/plain", "BAD PATH");
  }
  if (!exists(path)) {
    return Server.send(404, "text/plain", "FileNotFound");
  }
  FILESYSTEM.remove(path);
  Server.send(200, "text/plain", "");
  path = String();
}


void handleFileCreate() {
  if (Server.args() == 0) {
    return Server.send(500, "text/plain", "BAD ARGS");
  }
  String path = Server.arg(0);
  Serial.println("handleFileCreate: " + path);
  if (path == "/") {
    return Server.send(500, "text/plain", "BAD PATH");
  }
  if (exists(path)) {
    return Server.send(500, "text/plain", "FILE EXISTS");
  }
  File file = FILESYSTEM.open(path, "w");
  if (file) {
    file.close();
  } else {
    return Server.send(500, "text/plain", "CREATE FAILED");
  }
  Server.send(200, "text/plain", "");
  path = String();
}

void handleFileList() {
  if (!Server.hasArg("dir")) {
    Server.send(500, "text/plain", "BAD ARGS");
    return;
  }
  String path = Server.arg("dir");
  Serial.println("handleFileList: " + path);
  File root = FILESYSTEM.open(path);
  path = String();

  String output = "[";
  if(root.isDirectory()){
    Serial.println("handleFileList: root is a directory");
    File file = root.openNextFile();
    while(file){
        if (output != "[") {
          output += ',';
        }
        output += "{\"type\":\"";
        output += (file.isDirectory()) ? "dir" : "file";
        output += "\",\"name\":\"";
        output += String(file.name());
        output += "\",\"size\":\"";
        output += String(file.size());
        output += "\"}";
        file = root.openNextFile();
    }
  }else{
    Serial.println("handleFileList: Error root is not a directory");
  }
  output += "]";
  Server.send(200, "text/json", output);
}

String readFileAsString(String path){
  
  File file = FILESYSTEM.open(path);
  if(!file || file.isDirectory()){
      Serial.println("- failed to open file for reading");
      return "";
  }

  String strData = "";
  while(file.available()){
      strData = strData + (char) file.read();
  }
  file.close();
  return strData;
}

void formatFileSystem(){
    Serial.println("Formating...");
    FILESYSTEM.format();
    Serial.println("Format Complete");
}

void setupFileSystem(){
  Serial.println("setupFileSystem...");
  if (FORMAT_FILESYSTEM) {
    formatFileSystem();
  }
  fsOK = FILESYSTEM.begin();
  {
      File root = FILESYSTEM.open("/");
      File file = root.openNextFile();
      while(file){
          String fileName = file.name();
          size_t fileSize = file.size();
          Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
          file = root.openNextFile();
      }
      Serial.printf("\n");
  }
}

void addFilesToWebServer(){
  Serial.println("addFilesToWebServer...");
  if (fsOK) {
    
      File root = FILESYSTEM.open("/");
      File file = root.openNextFile();
      while(file){
          String fileName = file.name();
          size_t fileSize = file.size();          
          Server.on("/" + fileName, HTTP_GET, handleFSFile);
          Serial.println("Added Server /" + fileName);
          if(fileName.endsWith(".gz")){
            String newFileName = fileName.substring(0,fileName.length()-3);
            Server.on("/" + newFileName, HTTP_GET, handleFSFile);
            Serial.println("Added Server /" + newFileName);
          }
          file = root.openNextFile();
      }
      Serial.printf("\n");
  }
}
