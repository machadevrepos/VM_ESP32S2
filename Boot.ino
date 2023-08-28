void boot()
{
  Serial1.begin(9600);                                                          //Serail communication between ESP32 and Onion Omega 2S

  lcd.begin();                                                                  //Threshold setting LCD begin
  lcd.backlight();                                                              //Threshold setting LCD backlight on

  if (!EEPROM.begin(EEPROM_SIZE))                                               //Initializing the EEPROM
  {
    Serial.println("failed to initialise EEPROM");
  }  

	FastLED.setBrightness(255);                                                   //Setting RGB LEDs brightness to maximum (8bit value)
  FastLED.addLeds<WS2812, RGB_S_L_PIN>(RGB_S_L_pixels, RGB_S_L_NUMPIXELS);      //Configuring left compartment RGB Straight Strip with FastLED library
  FastLED.addLeds<WS2812, RGB_S_R_PIN>(RGB_S_R_pixels, RGB_S_R_NUMPIXELS);      //Configuring right compartment RGB Straight Strip with FastLED library
  FastLED.addLeds<WS2812, RGB_L_PIN>(RGB_L_pixels, RGB_L_NUMPIXELS);            //Configuring left compartment RGB Round Strip with FastLED library
  FastLED.addLeds<WS2812, RGB_R_PIN>(RGB_R_pixels, RGB_R_NUMPIXELS);            //Configuring right compartment RGB Round Strip with FastLED library

  pinMode(PWM1,OUTPUT);                                                         //Declaring pin as output
  pinMode(PWM2,OUTPUT);                                                         //Declaring pin as output
  pinMode(BIN1,OUTPUT);                                                         //Declaring pin as output

  pinMode(PWM3,OUTPUT);                                                         //Declaring pin as output
  pinMode(PWM4,OUTPUT);                                                         //Declaring pin as output
  pinMode(DIN1,OUTPUT);                                                         //Declaring pin as output

  pinMode(Button_START_RIGHT,INPUT_PULLDOWN);                                   //Declaring pin as input with software pulldown
  attachInterrupt(Button_START_RIGHT, isr_start_right, RISING);                 //Attach interrupt with rising edge
  pinMode(Button_START_LEFT,INPUT_PULLDOWN);                                    //Declaring pin as input with software pulldown 
  attachInterrupt(Button_START_LEFT, isr_start_left, RISING);                   //Attach interrupt with rising edge
  pinMode(Button_MINUS,INPUT);                                                  //Declaring pin as input
  attachInterrupt(Button_MINUS, isr_minus, RISING);                             //Attach interrupt with rising edge
  pinMode(Button_PLUS,INPUT);                                                   //Declaring pin as input
  attachInterrupt(Button_PLUS, isr_plus, RISING);                               //Attach interrupt with rising edge

  pinMode(ENCODER_SPIRAL_LEFT,INPUT_PULLUP);                                    //Declaring pin as input with software pullup
  attachInterrupt(ENCODER_SPIRAL_LEFT, isr_encoder_spiral_left, FALLING);       //Attach interrupt with falling edge
  pinMode(ENCODER_SWAPPER_LEFT,INPUT_PULLUP);                                   //Declaring pin as input with software pullup
  attachInterrupt(ENCODER_SWAPPER_LEFT, isr_encoder_swapper_left, FALLING);     //Attach interrupt with falling edge
  pinMode(ENCODER_SPIRAL_RIGHT,INPUT_PULLUP);                                   //Declaring pin as input with software pullup
  attachInterrupt(ENCODER_SPIRAL_RIGHT, isr_encoder_spiral_right, FALLING);     //Attach interrupt with falling edge
  pinMode(ENCODER_SWAPPER_RIGHT,INPUT_PULLUP);                                  //Declaring pin as input with software pullup
  attachInterrupt(ENCODER_SWAPPER_RIGHT, isr_encoder_swapper_right, FALLING);   //Attach interrupt with falling edge

  pinMode(IR_L,INPUT);                                                          //Declaring pin as input
  pinMode(IR_R,INPUT);                                                          //Declaring pin as input
  pinMode(BUZZER,OUTPUT);                                                       //Declaring pin as output
  digitalWrite(BUZZER,LOW);                                                     //Buzzer off

  digitalWrite(BIN1,HIGH);                                                      //Left compartment swapper motor rotate clockwise

  digitalWrite(DIN1,HIGH);                                                      //Right compartment swapper motor rotate clockwise

  digitalWrite(PWM1, 0);                                                         //Left compartment spiral rod motor stop
  analogWrite(PWM2, 0);                                                         //Left compartment swapper motor stop

  analogWrite(PWM3, 0);                                                         //Right compartment spiral rod motor stop
  analogWrite(PWM4, 0);                                                         //Right compartment swapper motor stop

  weight_threshold = EEPROM.read(1);                                            //Retrive threshold value from EEPROM

  rgb_r(CRGB::Green);                                                           //Right compartment RGB strip color set as Green
  rgb_s_r(CRGB::Green);                                                         //Right compartment RGB switch light color set as Green
  rgb_l(CRGB::Green);                                                           //Left compartment RGB strip color set as Green
  rgb_s_l(CRGB::Green);                                                         //Left compartment RGB switch light color set as Green

  Serial.println("Initializing the scale");
  scale_left.begin(LOADCELL1_DOUT_PIN, LOADCELL1_SCK_PIN);                      //Left load cell initialization
  scale_right.begin(LOADCELL2_DOUT_PIN, LOADCELL2_SCK_PIN);                     //Right load cell initialization
  scale_left.tare();                                                            //Reset the scale to 0
  scale_right.tare();                                                           //Reset the scale to 0
  Serial.println("Readings:");

  lcd.setCursor(0,0);                                                           
  lcd.print("WELCOME TO");
  lcd.setCursor(0,1);
  lcd.print("NUTRI-STOP");                                                      //Welcome note on LCD screen
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SERVING SIZE:");
  lcd.setCursor(0,1);
  lcd.print(String(weight_threshold) + "  ");                                   //Displaying weight threshold on LCD screen
  Serial.println("SERVING SIZE:" + String(weight_threshold));
  lcd.setCursor(5,1);
  lcd.print("GRAMS");

  analogWrite(PWM1, spiral_motor_speed);

  // while(vm_status != 8)
  // {
  //   serial_read();
  //   rgb_r_flash(CRGB::Green);                                                     //Right compartment RGB strip flash in Green
  //   rgb_s_r_flash(CRGB::Green);                                                   //Right compartment RGB switch light flash in Green 
  //   rgb_l_flash(CRGB::Green);                                                     //Right compartment RGB strip flash in Green
  //   rgb_s_l_flash(CRGB::Green);                                                   //Right compartment RGB switch light flash in Green
  // }
  // // if(vm_status == 8) serial_write(i_ssid);
  // delay(100);
  // if(vm_status == 8) serial_write("realme 8");
  // Serial.println("Sent Internet SSID to Onion Omega 2S");
  // while(vm_status != 9)
  // {
  //   serial_read();
  //   rgb_r_flash(CRGB::Green);                                                     //Right compartment RGB strip flash in Green
  //   rgb_s_r_flash(CRGB::Green);                                                   //Right compartment RGB switch light flash in Green 
  //   rgb_l_flash(CRGB::Green);                                                     //Right compartment RGB strip flash in Green
  //   rgb_s_l_flash(CRGB::Green);                                                   //Right compartment RGB switch light flash in Green
  // }
  // // if(vm_status == 9) serial_write(i_pass);
  // delay(100);
  // if(vm_status == 9) serial_write("zxcvbnmm");
  // Serial.println("Sent Internet Password to Onion Omega 2S");

  rgb_r(CRGB::Green);                                                             //Right compartment RGB strip color set as Green
  rgb_s_r(CRGB::Green);                                                           //Right compartment RGB switch light color set as Green
  rgb_l(CRGB::Green);                                                             //Right compartment RGB strip color set as Green
  rgb_s_l(CRGB::Green);                                                           //Right compartment RGB switch light color set as Green

  Button_START_RIGHT_pressed = false;
  Button_START_LEFT_pressed = false;
}