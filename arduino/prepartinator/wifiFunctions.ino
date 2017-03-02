#include<ESP8266WiFi.h>

#define WIFI_SSID "belkin.2ec"
#define WIFI_PASS "4ebb76d9"

const char* host = "192.168.2.20";
const int port = 1337;

WiFiClient client;

void wifi_setup(){
  connectToAP();
  Serial.println("WiFi setup successful");
  connectToServer();
}

static void connectToAP(){
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("connecting to AP");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected! IP: ");
  Serial.println(WiFi.localIP());
}

static int connectToServer(){
  Serial.println("Connecting to Server");
  if(client.connect(host, port)){
    Serial.println("Connected!");
    return 0;
  }
  return -1;
}

void wifi_sendRequest(String type, String value){

  String msg = encodeRequest(type, value);
  client.print(msg);
  Serial.println(msg);
}

String encodeRequest(String type, String value){
  return "{\"request\": \"" + type + "\", \"value\": \"" + value + "\"}";
}



