boolean sendingVol = false;

void setup() {
  serialSetup();
  led_setup();

  led_on('r');
  btn_setup();
  //wifi_setup();
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

void sendCmd(){
  led_on('b');
  i2c_update(); // Update one last time to make sure we have updated data
  wifi_sendRequest("cmd", cmdMap(i2c_getOrientation()));
  delay(300); // Some delay to not send more than one command
  led_off();
}

void sendVol(){
  wifi_sendRequest("vol", String(1));
  delay(50);
}

// Map orientation to command
String cmdMap(char o){
  switch(o){
    case('u'):
      return "mic";
    case('f'):
      return "none";
    case('b'):
      return "playpause";
    case('r'):
      return "next";
    case('l'):
      return "prev";
    case('d'):
      return "none";
    default:
      return "none";
  }
}



