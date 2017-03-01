int cmdBtn = D7;
int volBtn = D8;

int cmdBtnState;
int volBtnState;

void btn_setup(){
  pinMode(cmdBtn, INPUT);
  pinMode(volBtn, INPUT);
  Serial.println("Button setup successful");
}

void btn_update(){
  cmdBtnState = digitalRead(cmdBtn);
  volBtnState = digitalRead(volBtn);
}

boolean btn_cmdBtnPressed(){
  return cmdBtnState  == 1 ? true : false;
}

boolean btn_volBtnPressed(){
  return volBtnState == 1 ? true : false;
}

void printBtnStates(){
  Serial.print("CMD: ");
  Serial.print(cmdBtnState);
  Serial.print("\t");
  Serial.print("VOL: ");
  Serial.println(volBtnState);
}

