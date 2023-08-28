void beep()
{
  //Buzzer beeps at 1Hz
  if(millis() - buzzer_beep_delay >= 500)
  {
    buzzer_beep_flag = !buzzer_beep_flag;
    digitalWrite(BUZZER, buzzer_beep_flag);                                         //Change the Buzzer state
    buzzer_beep_delay = millis();
  }
}