// LED
int rgb[] = {D5, D0, D6};

void led_setup(){
  pinMode(rgb[0], OUTPUT);
  pinMode(rgb[1], OUTPUT);
  pinMode(rgb[2], OUTPUT);

  led_off(); // Turn off all the LEDs initially
  Serial.println("LED setup successful");
}

void led_off(){
  for(int i = 0; i < 3; i++){
    digitalWrite(rgb[i], LOW);
  }
}

void led_on(int i){
  digitalWrite(rgb[i], HIGH);
}

void led_on(char c){
  switch(c){
    case 'r':
      led_on(0);
      break;
    case 'g':
      led_on(1);
      break;
    case 'b':
      led_on(2);
      break;
  }
}

void led_cycleLeds(){
  for(int i = 0; i < 3; i++){
    led_off();
    led_on(i);
    delay(1000);
  }
}
