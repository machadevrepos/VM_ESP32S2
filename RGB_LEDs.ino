void rgb_r(uint32_t color)
{
  //Right compartment RGB strip color set
  for(int i=0; i<RGB_R_NUMPIXELS; i++) 
  {
    RGB_R_pixels[i] = color;
    FastLED.show();
  }
}

void rgb_r_flash(uint32_t color)
{
  //Right compartment RGB strip flashing at 1Hz
  if(millis() - rgb_r_flash_delay >= 500)
  {
    rgb_r_flash_flag = !rgb_r_flash_flag;
    rgb_r_flash_delay = millis();
    if(rgb_r_flash_flag) rgb_r(color);
    else rgb_r(CRGB::Black);
  }
}

void rgb_r_move(uint8_t hue)
{
  //Right compartment RGB strip move upward
  for(int i=0; i<RGB_R_NUMPIXELS; i++)
  {
    trail_rgb_r_arr[i] = ((i+1) * (255 / RGB_R_NUMPIXELS));
  }
  for(int i=0; i<RGB_R_NUMPIXELS; i++)
  {
    for (int k = 0; k < RGB_R_NUMPIXELS; k++) 
    {
      int m;
      if((i-k) < 0)  m = RGB_R_NUMPIXELS+(i-k);
      else m = i-k; 
      RGB_R_pixels[(RGB_R_NUMPIXELS-1)-k] = CHSV(hue, 255, trail_rgb_r_arr[m]);
    }
    FastLED.show();
    while(millis()-delayMillis < SECOND_TO_MILLIS / STRIP_SPEED)
    {
      //wait
    }
    delayMillis = millis();
  }
}

void rgb_s_r(uint32_t color)
{
  //Right compartment RGB switch light color set
  for(int i=0; i<RGB_S_R_NUMPIXELS; i++) 
  {
    RGB_S_R_pixels[i] = color;
    FastLED.show();
  }
}

void rgb_s_r_flash(uint32_t color)
{
  //Right compartment RGB switch light flashing at 1Hz
  if(millis() - rgb_s_r_flash_delay >= 500)
  {
    rgb_s_r_flash_flag = !rgb_s_r_flash_flag;
    rgb_s_r_flash_delay = millis();
    if(rgb_s_r_flash_flag) rgb_s_r(color);
    else rgb_s_r(CRGB::Black);
  }
}

void rgb_s_r_move(uint8_t hue, bool serial)
{
  //Right compartment RGB switch light move in circle
  for(int i=0; i<RGB_S_R_NUMPIXELS; i++)
  {
    trail_rgb_s_r_arr[i] = ((i+1) * (255 / RGB_S_R_NUMPIXELS));
  }
  for(int i=0; i<RGB_S_R_NUMPIXELS; i++)
  {
    for (int k = 0; k < RGB_S_R_NUMPIXELS; k++) 
    {
      int m;
      if((i-k) < 0)  m = RGB_S_R_NUMPIXELS+(i-k);
      else m = i-k; 
      RGB_S_R_pixels[(RGB_S_R_NUMPIXELS-1)-k] = CHSV(hue, 255, trail_rgb_s_r_arr[m]);
    }
    FastLED.show();
    while(millis()-delayMillis < SECOND_TO_MILLIS / CIRCLE_SPEED)
    {
      if(serial && Serial1.available()) break;
    }
    delayMillis = millis();
  }
}

void rgb_l(uint32_t color)
{
  //Left compartment RGB strip color set
  for(int i=0; i<RGB_L_NUMPIXELS; i++) 
  {
    RGB_L_pixels[i] = color;
    FastLED.show();
  }
}

void rgb_l_flash(uint32_t color)
{
  //Left compartment RGB strip flashing at 1Hz
  if(millis() - rgb_l_flash_delay >= 500)
  {
    rgb_l_flash_flag = !rgb_l_flash_flag;
    rgb_l_flash_delay = millis();
    if(rgb_l_flash_flag) rgb_l(color);
    else rgb_l(CRGB::Black);
  }
}

void rgb_l_move(uint8_t hue)
{
  //Left compartment RGB strip move upward
  for(int i=0; i<RGB_L_NUMPIXELS; i++)
  {
    trail_rgb_l_arr[i] = ((i+1) * (255 / RGB_L_NUMPIXELS));
  }
  for(int i=0; i<RGB_L_NUMPIXELS; i++)
  {
    for (int k = 0; k < RGB_L_NUMPIXELS; k++) 
    {
      int m;
      if((i-k) < 0)  m = RGB_L_NUMPIXELS+(i-k);
      else m = i-k; 
      RGB_L_pixels[(RGB_L_NUMPIXELS-1)-k] = CHSV(hue, 255, trail_rgb_l_arr[m]);
    }
    FastLED.show();
    while(millis()-delayMillis < SECOND_TO_MILLIS / STRIP_SPEED)
    {
      //wait
    }
    delayMillis = millis();
  }
}

void rgb_s_l(uint32_t color)
{
  //Left compartment RGB switch light color set
  for(int i=0; i<RGB_S_L_NUMPIXELS; i++) 
  {
    RGB_S_L_pixels[i] = color;
    FastLED.show();
  }
}

void rgb_s_l_flash(uint32_t color)
{
  //Left compartment RGB switch light flashing at 1Hz
  if(millis() - rgb_s_l_flash_delay >= 500)
  {
    rgb_s_l_flash_flag = !rgb_s_l_flash_flag;
    rgb_s_l_flash_delay = millis();
    if(rgb_s_l_flash_flag) rgb_s_l(color);
    else rgb_s_l(CRGB::Black);
  }
}

void rgb_s_l_move(uint8_t hue, bool serial)
{
  //Left compartment RGB switch light move in circle
  for(int i=0; i<RGB_S_L_NUMPIXELS; i++)
  {
    trail_rgb_s_l_arr[i] = ((i+1) * (255 / RGB_S_L_NUMPIXELS));
  }
  for(int i=0; i<RGB_S_L_NUMPIXELS; i++)
  {
    for (int k = 0; k < RGB_S_L_NUMPIXELS; k++) 
    {
      int m;
      if((i-k) < 0)  m = RGB_S_L_NUMPIXELS+(i-k);
      else m = i-k; 
      RGB_S_L_pixels[(RGB_S_L_NUMPIXELS-1)-k] = CHSV(hue, 255, trail_rgb_s_l_arr[m]);
    }
    FastLED.show();
    while(millis()-delayMillis < SECOND_TO_MILLIS / CIRCLE_SPEED)
    {
      if(serial && Serial1.available()) break;
    }
    delayMillis = millis();
  }
}