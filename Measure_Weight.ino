double current_weight_right()
{
  double raw_weight = scale_right.get_units();                                    //Reading right weight sensor value
  Serial.println("Right raw_weight: " + String(raw_weight));
  double weight_in_gm = raw_weight/1000;                                          //Scaling the sensor values to grams
  Serial.println("Right weight_in_gm: " + String(weight_in_gm));
  return abs(weight_in_gm);
}

double current_weight_left()
{
  double raw_weight = scale_left.get_units();                                     //Reading left weight sensor value
  Serial.println("Left raw_weight: " + String(raw_weight));
  double weight_in_gm = raw_weight/1000;                                          //Scaling the sensor values to grams
  Serial.println("Left weight_in_gm: " + String(weight_in_gm));
  return abs(weight_in_gm);
}