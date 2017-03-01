#include<ESP8266WiFi.h>

#define WIFI_SSID "PiSpot"
#define WIFI_PASS "passord123"

const char* host = "192.168.42.1";
const int port = 8000;

WiFiClient client;

void wifi_setup(){
  connectToAP();
  Serial.println("WiFi setup successfull");
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

  String msg = "{\"request\": \"" + type + "\", \"value\": \"" + value + "\"}";
  /*
  client.print(msg);
  client.print("\r\n");
  */
  Serial.println(msg);
}



