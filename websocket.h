#include <WebSocketsServer.h>

WebSocketsServer webSocket(81);    // create a websocket server on port 81

void sendBroadcast(String msg) {
  String message = "MSG:" + String(*id) + "~" + msg; 
  webSocket.broadcastTXT(message);
}

// Web Socket Event Received
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
  // Check Event Type
  switch (type) {
    // Web Socket Disconnected
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    // Web Socket Connected
    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      }
      break;
    // Text Received
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);
      // processIncomingMessage(num, payload);
      break;
  }
}

void startWebSocket() {
  // Start the WebSocket server
  webSocket.begin();
  // Process incoming events to webSocketEvent
  webSocket.onEvent(webSocketEvent);
  Serial.println("WebSocket server started.");
}