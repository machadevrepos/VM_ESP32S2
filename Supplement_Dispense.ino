void supplement_dispense()
{
  if(Button_START_RIGHT_pressed)                                                  //Check for right compartment button press
  {
    Button_START_RIGHT_pressed = false;
    rgb_s_r(CRGB::Green);                                                         //Right compartment RGB switch light color set as Green
    //Check for empty container
    if(digitalRead(IR_R) == HIGH)
    {
      //Container is empty
      five_seconds = millis();
      while(millis() - five_seconds <= 5000)
      {
        beep();                                                                   //Buzzer beeps
        delay(5);
        rgb_s_r_move(Red_hue, false);                                             //Right compartment RGB switch light move in circle in Red
        delay(5);
        rgb_r_move(Red_hue);                                                      //Right compartment RGB strip move upward in Red
        delay(5);
      }
      digitalWrite(BUZZER,LOW);                                                   //Buzzer off
      rgb_r(CRGB::Green);                                                         //Right compartment RGB strip color set as Green
      rgb_s_r(CRGB::Green);                                                       //Right compartment RGB switch light color set as Green
    }
    else
    {
      Serial1.write("1");                                                         //Tell Onion Omega 2S to proceed payment with Stripe
      //wait for stripe response
      sixty_seconds = millis();
      while(!Serial1.available())
      {
        //Wait here until we get serial response from Onion Omega 2S
        if(millis() - sixty_seconds >= 30000)
        {
          //Break the loop if no response in 30 seconds
          no_serial_response_break_flag = true;                                   //No response flag high
        }
        else no_serial_response_break_flag = false;
        rgb_s_r_move(Green_hue, true);                                            //Right compartment RGB switch light move in circle in Green and check for serial data too
        if(no_serial_response_break_flag) break;                                  //Break the loop if no response flag high
      }

      if(!no_serial_response_break_flag)
      {
        //If serial response from Onion Omega 2S is available
        serial_read();                                                            //Read serial response

        if(vm_status != 0)                                                        //Check for data updates
        {
          if(vm_status == 1)                                                      //If response is "1" it means Payment_Intent is created and handed over to Stripe Terminal but not yet processed by the customer
          {
            Serial1.write("2");                                                   //Request for new response within 30 seconds.
            sixty_seconds = millis();
            while(!Serial1.available())
            {
              //Wait here until we get serial response from Onion Omega 2S
              if(millis() - sixty_seconds >= 30000)
              {
                //Break the loop if no response in 30 seconds
                five_seconds = millis();
                while(millis() - five_seconds <= 5000)
                {
                  rgb_s_r_flash(CRGB::Red);                                       //Right compartment RGB switch light color set as Red
                  delay(5);
                  rgb_r_flash(CRGB::Red);                                         //Right compartment RGB strip flash in Red
                  delay(5);
                }
                rgb_r(CRGB::Green);                                               //Right compartment RGB strip color set as Green
                rgb_s_r(CRGB::Green);                                             //Right compartment RGB switch light color set as Green
                no_serial_response_break_flag = true;                             //No response flag high
              }
              else 
              {
                no_serial_response_break_flag = false;
                rgb_s_r_move(Green_hue, true);                                    //Right compartment RGB switch light move in circle in Green and check for serial data too
              }
              if(no_serial_response_break_flag) break;                            //Break the loop if no response flag high
            }
            if(!no_serial_response_break_flag)
            {
              //If serial response from Onion Omega 2S is available
              serial_read();                                                      //Read serial response
              if(vm_status == 1)
              {
                //Payment unsuccessful
                five_seconds = millis();
                while(millis() - five_seconds <= 5000)
                {
                  rgb_s_r_flash(CRGB::Red);                                       //Right compartment RGB switch light color set as Red
                  delay(5);
                  rgb_r_flash(CRGB::Red);                                         //Right compartment RGB strip flash in Red
                  delay(5);
                }
                rgb_r(CRGB::Green);                                               //Right compartment RGB strip color set as Green
                rgb_s_r(CRGB::Green);                                             //Right compartment RGB switch light color set as Green
                no_serial_response_break_flag = true;                             //No response flag high
              }
            }
          }
          if(vm_status == 2)                                                      //If response is "2" it means Payment_Intent is created, handed over to Stripe Terminal and successfully processed by the customer
          {
            //Payment is succesfull
            while(current_weight_right() < weight_threshold)
            {
              //Keep pouring the supplement untill threshold is achieved
              analogWrite(PWM3, spiral_motor_speed);                              //Right compartment spiral rod motor rotate
              rgb_s_r_move(Green_hue, false);                                     //Right compartment RGB switch light move in circle in Green
              Serial.println("Right Spiral rod rotating");
            }
            analogWrite(PWM3, 0);                                                 //Right compartment spiral rod motor stop
            Serial.println("Right Spiral rod stopped");
            while(1)
            {
              //Wait here until the customer is ready to collect the supplement
              rgb_r_move(Green_hue);                                              //Right compartment RGB strip move upward in Green
              delay(5);
              rgb_s_r(CRGB::Green);                                               //Right compartment RGB switch light color set as Green
              delay(5);
              if(Button_START_RIGHT_pressed) break;                               //When the customer is ready, a second time button press is required
            }
            rgb_r(CRGB::Green);                                                   //Right compartment RGB strip color set as Green
            rgb_s_r(CRGB::Green);                                                 //Right compartment RGB switch light color set as Green
            Serial.println("Right Swapper opening");
            digitalWrite(DIN1,HIGH);                                              //Right compartment swapper motor rotate clockwise
            rotate_motor_at_angle(PWM4, 180);                                     //Right compartment swapper motorrotate 180 degrees
            digitalWrite(DIN1,LOW);                                               //Right compartment swapper motor rotate anti-clockwise
            delay(1000);
            Serial.println("Right Swapper closing");
            rotate_motor_at_angle(PWM4, 180);                                     //Right compartment swapper motorrotate 180 degrees in opposite direction
            analogWrite(PWM3, 0);                                                 //Right compartment spiral rod motor stop
            analogWrite(PWM4, 0);                                                 //Right compartment swapper motor stop
          }
        }
      }
    }
    no_serial_response_break_flag = false;
    Button_START_RIGHT_pressed = false;                                           //Reset button state
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  if(Button_START_LEFT_pressed)                                                   //Check for left compartment button press
  {
    Button_START_LEFT_pressed = false;
    rgb_s_l(CRGB::Green);                                                         //Left compartment RGB switch light color set as Green
    //Check for empty container
    if(digitalRead(IR_L) == HIGH)
    {
      //Container is empty
      five_seconds = millis();
      while(millis() - five_seconds <= 5000)
      {
        beep();                                                                   //Buzzer beeps
        delay(5);
        rgb_s_l_move(Red_hue, false);                                             //Left compartment RGB switch light move in circle in Red
        delay(5);
        rgb_l_move(Red_hue);                                                      //Left compartment RGB strip move upward in Red
        delay(5);
      }
      digitalWrite(BUZZER,LOW);                                                   //Buzzer off
      rgb_l(CRGB::Green);                                                         //Left compartment RGB strip color set as Green
      rgb_s_l(CRGB::Green);                                                       //Left compartment RGB switch light color set as Green
    }
    else
    {
      Serial1.write("1");                                                         //Tell Onion Omega 2S to proceed payment with Stripe
      //wait for stripe response
      sixty_seconds = millis();
      while(!Serial1.available())
      {
        //Wait here until we get serial response from Onion Omega 2S
        if(millis() - sixty_seconds >= 30000)
        {
          //Break the loop if no response in 30 seconds
          no_serial_response_break_flag = true;                                   //No response flag high
        }
        else no_serial_response_break_flag = false;
        rgb_s_l_move(Green_hue, true);                                            //Left compartment RGB switch light move in circle in Green and check for serial data too
        if(no_serial_response_break_flag) break;                                  //Break the loop if no response flag high
      }

      if(!no_serial_response_break_flag)
      {
        //If serial response from Onion Omega 2S is available
        serial_read();                                                            //Read serial response

        if(vm_status != 0)                                                        //Check for data updates
        {
          if(vm_status == 1)                                                      //If response is "1" it means Payment_Intent is created and handed over to Stripe Terminal but not yet processed by the customer
          {
            Serial1.write("2");                                                   //Request for new response within 30 seconds.
            sixty_seconds = millis();
            while(!Serial1.available())
            {
              //Wait here until we get serial response from Onion Omega 2S
              if(millis() - sixty_seconds >= 30000)
              {
                //Break the loop if no response in 30 seconds
                five_seconds = millis();
                while(millis() - five_seconds <= 5000)
                {
                  rgb_s_l_flash(CRGB::Red);                                       //Left compartment RGB switch light color set as Red
                  delay(5);
                  rgb_l_flash(CRGB::Red);                                         //Left compartment RGB strip flash in Red
                  delay(5);
                }
                rgb_l(CRGB::Green);                                               //Left compartment RGB strip color set as Green
                rgb_s_l(CRGB::Green);                                             //Left compartment RGB switch light color set as Green
                no_serial_response_break_flag = true;                             //No response flag high
              }
              else 
              {
                no_serial_response_break_flag = false;
                rgb_s_l_move(Green_hue, true);                                    //Left compartment RGB switch light move in circle in Green and check for serial data too
              }                
              if(no_serial_response_break_flag) break;                            //Break the loop if no response flag high
            }
            if(!no_serial_response_break_flag)
            {
              //If serial response from Onion Omega 2S is available
              serial_read();                                                      //Read serial response
              if(vm_status == 1)
              {
                //Payment unsuccessful
                five_seconds = millis();
                while(millis() - five_seconds <= 5000)
                {
                  rgb_s_l_flash(CRGB::Red);                                       //Left compartment RGB switch light color set as Red
                  delay(5);
                  rgb_l_flash(CRGB::Red);                                         //Left compartment RGB strip flash in Red
                  delay(5);
                }
                rgb_l(CRGB::Green);                                               //Left compartment RGB strip color set as Green
                rgb_s_l(CRGB::Green);                                             //Left compartment RGB switch light color set as Green
                no_serial_response_break_flag = true;                             //No response flag high
              }
            }
          }
          if(vm_status == 2)                                                      //If response is "2" it means Payment_Intent is created, handed over to Stripe Terminal and successfully processed by the customer
          {
            //Payment is succesfull
            while(current_weight_left() < weight_threshold)
            {
              //Keep pouring the supplement untill threshold is achieved
              analogWrite(PWM1, spiral_motor_speed);                              //Left compartment spiral rod motor rotate
              rgb_s_l_move(Green_hue, false);                                     //Left compartment RGB switch light move in circle in Green
              Serial.println("Left Spiral rod rotating");
            }
            analogWrite(PWM1, 0);                                                 //Left compartment spiral rod motor stop
            Serial.println("Left Spiral rod stopped");
            while(1)
            {
              //Wait here until the customer is ready to collect the supplement
              rgb_l_move(Green_hue);                                              //Left compartment RGB strip move upward in Green
              delay(5);
              rgb_s_l(CRGB::Green);                                               //Left compartment RGB switch light color set as Green
              delay(5);
              if(Button_START_LEFT_pressed) break;                                //When the customer is ready, a second time button press is required
            }
            rgb_l(CRGB::Green);                                                   //Left compartment RGB strip color set as Green
            rgb_s_l(CRGB::Green);                                                 //Left compartment RGB switch light color set as Green
            Serial.println("Left Swapper opening");
            digitalWrite(BIN1,HIGH);                                              //Left compartment swapper motor rotate clockwise
            rotate_motor_at_angle(PWM2, 180);                                     //Left compartment swapper motorrotate 180 degrees
            digitalWrite(BIN1,LOW);                                               //Left compartment swapper motor rotate anti-clockwise
            delay(1000);
            Serial.println("Left Swapper closing");
            rotate_motor_at_angle(PWM2, 180);                                     //Left compartment swapper motorrotate 180 degrees in opposite direction
            analogWrite(PWM1, 0);                                                 //Left compartment spiral rod motor stop
            analogWrite(PWM2, 0);                                                 //Left compartment swapper motor stop
          }
        }
      }
    }
    no_serial_response_break_flag = false;
    Button_START_LEFT_pressed = false;                                            //Reset button state
  }
}