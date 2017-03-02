boolean sendingVol = false;
int velocity = 0;

void setup() {
  serialSetup();
  led_setup();

  led_on('r');
  btn_setup();
  wifi_setup();
  i2c_setup();
  led_off();
}

void serialSetup(){
  Serial.begin(115200);
  Serial.println();
  Serial.println("Serial setup successful");
}

void loop() {
  btn_update();
  i2c_update();
  if(btn_cmdBtnPressed()){
    sendCmd();
  }
  
  if(btn_volBtnPressed()){
    if(!sendingVol){
      velocity = 0;
      led_on('g');
      sendingVol = true;
    }
    sendVol();
  }else{
    if(sendingVol){
      led_off();
      sendingVol = false;
    }
  }
  delay(10);
}

// Sends a command based on the orientation of the device
void sendCmd(){
  led_on('b');
  i2c_update(); // Update one last time to make sure we have updated data
  wifi_sendRequest("cmd", cmdMap(i2c_getOrientation()));
  delay(300); // Some delay to not send more than one command
  led_off();
}

// Sends volume if velocity is high enough
void sendVol(){
  i2c_update();
  int dv = i2c_getAccZ() - 1000;
  velocity += dv;
  
  if(velocity < -1000){
    wifi_sendRequest("vol", "-"); // Volume down
  }
  else if(velocity > 1000){
    wifi_sendRequest("vol", "+"); // Volume up
  }
  delay(30);
}

// Map orientation to command
String cmdMap(char o){
  switch(o){
    case('u'):
      return "mic";
    case('f'):
      return "prev";
    case('b'):
      return "next";
    case('r'):
      return "none";
    case('l'):
      return "playpause";
    case('d'):
      return "none";
    default:
      return "none";
  }
}



