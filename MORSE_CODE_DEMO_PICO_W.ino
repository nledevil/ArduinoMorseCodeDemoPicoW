// Import Variables from variables.h
#include "variables.h"
// Function to match mime types
#include "mimetypes.h"
// Web Socket code
#include "websocket.h"
// CW Code
#include "cw.h"
// OTA Code
#include "ota.h"
// Other Libraries
#include "LittleFS.h" // LittleFS is declared
#include <WebSocketsServer.h>
#include <WiFi.h>
#include <WiFiClient.h>
// #include <WiFiMulti.h>
#include <ArduinoOTA.h>
#include <WebServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

// Experimenting with WifiMulti
// WiFiMulti wifiMulti;
// Web Server on Port 80
WebServer server(80);       // create a web server on port 80

// Access Point Local IP
IPAddress apIP;

// Temp File used to store received a received file
File fsUploadFile;

/*__________________________________________________________SETUP__________________________________________________________*/

void setup() {
  // Init CW Key
  pinMode(CW_KEY, INPUT_PULLUP);
  // Built-In Output LED for debugging
  pinMode(LED_BUILTIN, OUTPUT);
  // Tone Pin
  #ifdef CW_TONE
    pinMode(CW_TONE, OUTPUT);
  #endif

  // We store MORSE CODE in json, so we need to deserialize it
  DeserializationError error = deserializeJson(doc, json);

  // Make sure it deserializes without error
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  // Start the Serial Output
  Serial.begin(9600);        // Start the Serial communication to send messages to the computer
  delay(10);
  // Start the Wifi AP
  startWiFi();
  // Start the OTA Web Server Files Updater
  startOTA();
  // Initialize the LittleFS
  LittleFS.begin();
  // Start the WebSocket Server
  startWebSocket();
  // Start the Web Server
  startServer();
}

/*__________________________________________________________LOOP__________________________________________________________*/
void loop() {
  // On First Boot, display the version, AP Name and AP IP
  // Also list the contents of the LittleFS (Should have all the web files)
  if (bootupStart) {
    delay(3000);
    Serial.println("\r\nVersion: ");
    Serial.print(version);
    Serial.println("\r\n");
    Serial.println("ID: ");
    Serial.print(id);
    Serial.println("\r\n");
    Serial.print("Access Point \"");
    Serial.print(ssid);
    Serial.print("\" started on ");
    Serial.print(apIP);
    Serial.println("\r\n");
    Serial.println("\r\n");
    listLittleFS();
    bootupStart = false;
  }
  // Make sure to check for new WebSocket Events
  webSocket.loop();
  // Run the Web Server
  server.handleClient();
  // Check for OTA Events
  ArduinoOTA.handle();
  // Morse Code Functions
  // Check if key is pressed or not and light led/play tone
  handleKey();
  // Check the buffer/timing for a new letter > 200ms = new character
  checkNewLetter();
  // Check for new Words > 1000ms(1s)
  checkNewWord();
}


/*__________________________________________________________SETUP_FUNCTIONS__________________________________________________________*/

void startWiFi() {
  // Start the Access Point with the ssid/password
  WiFi.softAP(ssid, password);
  // Get the Access Point IP
  apIP = WiFi.softAPIP();             // Start the access point
  // wifiMulti.addAP("Pretty fly for a WiFi", "51c1v35O8P");   // add Wi-Fi networks you want to connect to

  // Serial.println("Connecting");
  // while (wifiMulti.run() != WL_CONNECTED && WiFi.softAPgetStationNum() < 1) {  // Wait for the Wi-Fi to connect
  //   delay(250);
  //   Serial.print('.');
  // }
  // Serial.println("\r\n");
  // if(WiFi.softAPgetStationNum() == 0) {      // If the ESP is connected to an AP
  //   Serial.print("Connected to ");
  //   Serial.println(WiFi.SSID());             // Tell us what network we're connected to
  //   Serial.print("IP address:\t");
  //   Serial.print(WiFi.localIP());            // Send the IP address of the ESP8266 to the computer
  // } else {                                   // If a station is connected to the ESP SoftAP
  //   Serial.print("Station connected to AP");
  // }
  // Serial.println("\r\n");
}

void listLittleFS() {
  Serial.println("LittleFS Started. Contents:");
  {
    Dir dir = LittleFS.openDir("/");
    while (dir.next()) {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("\tFS File: %s, size: %s\r\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    Serial.printf("\n");
  }
}

void startServer() {
  // This handler checks for edit.html, which is for OTA Updates
  server.on("/edit.html",  HTTP_POST, []() {
    server.send(200, "text/plain", ""); 
  }, handleFileUpload);
  // All Other requests go to handleNotFound Handler, which will determine if it's truely not found
  server.onNotFound(handleNotFound);
  // Actually Start the Server
  server.begin();
  Serial.println("HTTP server started.");
}

/*__________________________________________________________SERVER_HANDLERS__________________________________________________________*/

void handleNotFound() {
  // Check if file exists, else return 404 not found
  if(!handleFileRead(server.uri())) {
    server.send(404, "text/plain", "404: File Not Found");
  }
}

bool handleFileRead(String path) {
  Serial.println("handleFileRead: " + path);
  // No folders for you, redirect to index.html
  if (path.endsWith("/")) path += "index.html";
  // Get the Mime Type
  String contentType = getContentType(path);             // Get the MIME type
  // Check if we are getting compressed version or not (gzip)
  String pathWithGz = path + ".gz";
  if (LittleFS.exists(pathWithGz) || LittleFS.exists(path)) {
    // If compressed version, use that
    if (LittleFS.exists(pathWithGz))
      path += ".gz";
    // Open the file for reading
    File file = LittleFS.open(path, "r");
    // Send the file to the client using the detected mime type
    size_t sent = server.streamFile(file, contentType);
    // Close the file
    file.close();
    Serial.println(String("\tSent file: ") + path);
    return true;
  }
  // If file not found, return 404 by returning false here;
  Serial.println(String("\tFile Not Found: ") + path);
  return false;
}

void handleFileUpload() {
  // Initialize an upload
  HTTPUpload& upload = server.upload();
  // Temp path
  String path;
  // Check if upload started
  if(upload.status == UPLOAD_FILE_START) {
    // set path to filename
    path = upload.filename;
    // Add a leading /
    if(!path.startsWith("/")) path = "/" + path;
    // If uploading a non-compressed version, make sure to remove an already existing compressed version if it exists
    if(!path.endsWith(".gz")) {
      String pathWithGz = path + ".gz";
      if(LittleFS.exists(pathWithGz))
         LittleFS.remove(pathWithGz);
    }
    Serial.print("handleFileUpload Name: "); Serial.println(path);
    // Open the file for writing
    fsUploadFile = LittleFS.open(path, "w");
    path = String();
    // If status is file write, then write the received bytes to the file
  } else if(upload.status == UPLOAD_FILE_WRITE) {
    if(fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
    // If status is File End, then close the fsUploadFile and send server response
  } else if(upload.status == UPLOAD_FILE_END){
    if(fsUploadFile) { 
      fsUploadFile.close();
      Serial.print("handleFileUpload Size: ");
      Serial.println(upload.totalSize);
      server.sendHeader("Location","/success.html");
      server.send(303);
    } else {
      // Error
      server.send(500, "text/plain", "500: couldn't create file");
    }
  }
}

/*__________________________________________________________HELPER_FUNCTIONS__________________________________________________________*/

// Generic Helper Function to convert bytes to KB/MB, no need for anything higher than MB
String formatBytes(size_t bytes) {
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  }
  return String();
}
