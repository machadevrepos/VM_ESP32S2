void rotate_motor_at_angle(uint16_t en_pin, uint16_t angle)
{
  //function to turn the swapper motor at a certain angle. Like to rotate at 180 degrees to open the swapper and -180 degrees to close it.
  value = 0;
  motor_speed = 0;
  update_encoder_value_and_speed(en_pin);

  uint16_t current_value = 0;
  uint16_t previous_value = value;
  uint16_t angle_to_value = (angle*700)/360;                                        //The encoder read signals approx. 700 times in one revolution
  Serial.println("angle" + String(angle));
  Serial.println("angle_to_value" + String(angle_to_value));
  while(current_value - previous_value < angle_to_value)
  {
    //keep rotating until desired angle is achieved
    Serial.println("current_value" + String(current_value));
    Serial.println("previous_value" + String(previous_value));
    analogWrite(en_pin, motor_speed);
    update_encoder_value_and_speed(en_pin);
    current_value = value;
  }
  analogWrite(en_pin, 0);                                                           //Stop the motor
  reset_encoder_value(en_pin);
}

void update_encoder_value_and_speed(uint16_t en_pin)
{
  //Depending upon the motor, get the set motor speed and encoder values
  if(en_pin == PWM1)
  {
    value = ENCODER_SPIRAL_LEFT_value;
    motor_speed = spiral_motor_speed;
  }
  else if(en_pin == PWM2)
  {
    value = ENCODER_SWAPPER_LEFT_value;
    motor_speed = swapper_motor_speed;
  }
  else if(en_pin == PWM3)
  {
    value = ENCODER_SPIRAL_RIGHT_value;
    motor_speed = spiral_motor_speed;
  }
  else if(en_pin == PWM4)
  {
    value = ENCODER_SWAPPER_RIGHT_value;
    motor_speed = swapper_motor_speed;
  }
}

void reset_encoder_value(uint16_t en_pin)
{
  //Depending upon the motor, reset encoder value
  if(en_pin == PWM1)
  {
    ENCODER_SPIRAL_LEFT_value = 0;
  }
  else if(en_pin == PWM2)
  {
    ENCODER_SWAPPER_LEFT_value = 0;
  }
  else if(en_pin == PWM3)
  {
    ENCODER_SPIRAL_RIGHT_value = 0;
  }
  else if(en_pin == PWM4)
  {
    ENCODER_SWAPPER_RIGHT_value = 0;
  }
}