void threshold_setting()
{
  if(Button_PLUS_pressed)                                                         //Check for threshold increment button press
  {
    if(weight_threshold < 100)                                                    //Increment only if the value is less than 100
    {
      weight_threshold++;                                                         //Increment the value
      lcd.setCursor(0,1);
      lcd.print(String(weight_threshold) + "  ");                                 //Displaying weight threshold on LCD screen
      lcd.setCursor(5,1);
      lcd.print("GRAMS");
      Serial.println("Increment: " + String(weight_threshold));
      EEPROM.write(1, weight_threshold);                                          //Save the value in EEPROM
      EEPROM.commit();
    }
    delay(100);
    Button_PLUS_pressed = false;                                                  //Reset button state
  }
  else if(Button_MINUS_pressed)                                                   //Check for threshold decrement button press
  {
    if(weight_threshold > 0)                                                      //Decrement only if the value is greater than 0
    {
      weight_threshold--;                                                         //Decrement the value
      lcd.setCursor(0,1);
      lcd.print(String(weight_threshold) + "  ");                                 //Displaying weight threshold on LCD screen
      lcd.setCursor(5,1);
      lcd.print("GRAMS");
      Serial.println("Decrement: " + String(weight_threshold));
      EEPROM.write(1, weight_threshold);                                          //Save the value in EEPROM
      EEPROM.commit();
    }
    delay(100);
    Button_MINUS_pressed = false;                                                 //Reset button state
  }
}