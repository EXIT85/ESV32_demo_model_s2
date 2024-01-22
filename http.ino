#include "edit_html.h"
enum { NONE=0, INDEX, XML };

void handleMenuHtml(void){
   Server.send(200, "text/html", AUTOCONNECT_LINK(BAR_32)); 
}

void handleFileFormat(void){
  formatFileSystem();
  Server.send(200, "text/html", "Format Complete"); 
}

void handleSendpitch(void){
  
  Server.send(200, "text/json", "{\"status\":\"Pitch Received\"}"); 
}

void handleDeleteSsids(void){
  Serial.println("handleDeleteSsids:");
  deleteAllCredentials();
   Server.send(200, "text/json", "{\"status\":\"Credentials Deleted\"}");
}

void handleFSFile(void){
  handleFileRead(Server.uri());
}

void setupHttpServer(){
  //need to add all pages here as the NotFound will send request to captiveportal if not connected to an ap
  Server.on("/", HTTP_GET, handleFSFile);
  
  addFilesToWebServer();

  Serial.println("setupHttpServer:");
  //sendpitch
  Server.on("/sendpitch", HTTP_GET, handleSendpitch);
  //SERVER INIT
  Server.on("/autoconnectMenu", HTTP_GET, handleMenuHtml);
  Server.on("/deleteSsids", HTTP_GET, handleDeleteSsids);
  //format filesystem
  Server.on("/format", HTTP_GET, handleFileFormat);

   // Filesystem status
  Server.on("/status", HTTP_GET, handleStatus);

  //list directory
  Server.on("/list", HTTP_GET, handleFileList);
  //load editor
  Server.on("/edit", HTTP_GET, []() {
      Server.send(200, "text/html", edit_html);
  });
  //create file
  Server.on("/edit", HTTP_PUT, handleFileCreate);
  //delete file
  Server.on("/edit", HTTP_DELETE, handleFileDelete);
  //first callback is called after the request has ended with all parsed arguments
  //second callback handles file uploads at that location
  Server.on("/edit", HTTP_POST, []() {
    Server.send(200, "text/plain", "");
  }, handleFileUpload);
  
  //called when the url is not defined here
  //use it to load content from FILESYSTEM
  Server.onNotFound([]() {
    if (!handleFileRead(Server.uri())) {
      Server.send(404, "text/plain", "FileNotFound");
    }
  });

 
  
}
