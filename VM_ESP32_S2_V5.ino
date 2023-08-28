#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

bool debug = true;                                                              //for Serial Monitor

#include <LCD_I2C.h>                                                            //Library for threshold setting LCD

LCD_I2C lcd(0x27);                                                              //Default address of most PCF8574 modules

#include <Arduino.h>
#include "HX711.h"                                                              //Library for Load Cell amplifier
 
const int LOADCELL1_DOUT_PIN = 21;                                              //Left compartment Load Cell's DOUT pin connected to Pin21 of ESP32-S2
const int LOADCELL1_SCK_PIN = 26;                                               //Left compartment Load Cell's SCK pin connected to Pin26 of ESP32-S2

const int LOADCELL2_DOUT_PIN = 33;                                              //Right compartment Load Cell's DOUT pin connected to Pin33 of ESP32-S2
const int LOADCELL2_SCK_PIN = 34;                                               //Right compartment Load Cell's DOUT pin connected to Pin34 of ESP32-S2
 
HX711 scale_left;                                                               //Declaring left load cell amplifier's instance
HX711 scale_right;                                                              //Declaring right load cell amplifier's instance

// Motor Drivers
#define PWM1 11                                                                 //Left compartment spiral rod motor's speed control pin connected to Pin11 of ESP32-S2
#define PWM2 12                                                                 //Left compartment swapper motor's speed control pin connected to Pin12 of ESP32-S2
#define BIN1 5                                                                  //Left compartment swapper motor's direction control pin connected to Pin5 of ESP32-S2

#define PWM3 13                                                                 //Right compartment spiral rod motor's speed control pin connected to Pin13 of ESP32-S2
#define PWM4 14                                                                 //Right compartment swapper motor's speed control pin connected to Pin14 of ESP32-S2
#define DIN1 6                                                                  //Right compartment swapper motor's direction control pin connected to Pin6 of ESP32-S2

uint8_t spiral_motor_speed = 255;                                               //Spiral rod motors speed set. Max speed is 255
uint8_t swapper_motor_speed = 120;                                              //Swapper motors speed set. Max speed is 255

uint16_t value = 0;                                                             //Variable to store motors encoder value
uint16_t motor_speed = 0;                                                       //Variable to set motor speed at run time value

// Encoders
#define ENCODER_SPIRAL_LEFT 35                                                  //Left compartment spiral rod motor encoder pin connected to Pin35 of ESP32-S2
#define ENCODER_SWAPPER_LEFT 36                                                 //Left compartment swapper motor encoder pin connected to Pin36 of ESP32-S2
#define ENCODER_SPIRAL_RIGHT 37                                                 //Right compartment spiral rod motor encoder pin connected to Pin37 of ESP32-S2
#define ENCODER_SWAPPER_RIGHT 38                                                //Right compartment swapper motor encoder pin connected to Pin36 of ESP32-S2

uint16_t ENCODER_SPIRAL_LEFT_value = 0;                                         //Variable to measure left compartment spiral rod motor rotations
uint16_t ENCODER_SWAPPER_LEFT_value = 0;                                        //Variable to measure left compartment swapper motor rotations
uint16_t ENCODER_SPIRAL_RIGHT_value = 0;                                        //Variable to measure right compartment spiral rod motor rotations
uint16_t ENCODER_SWAPPER_RIGHT_value = 0;                                       //Variable to measure right compartment swapper motor rotations

void IRAM_ATTR isr_encoder_spiral_left() 
{
  ENCODER_SPIRAL_LEFT_value++;                                                  //Increment left compartment spiral rod motor rotations based on Interrupts
}

void IRAM_ATTR isr_encoder_swapper_left() 
{
  ENCODER_SWAPPER_LEFT_value++;                                                 //Increment left compartment swapper motor rotations based on Interrupts
}

void IRAM_ATTR isr_encoder_spiral_right() 
{
  ENCODER_SPIRAL_RIGHT_value++;                                                 //Increment right compartment spiral rod motor rotations based on Interrupts
}

void IRAM_ATTR isr_encoder_swapper_right() 
{
  ENCODER_SWAPPER_RIGHT_value++;                                                //Increment right compartment swapper motor rotations based on Interrupts
}

// Switches
#define Button_START_LEFT 1                                                     //Left compartment Start button pin connected to Pin1 of ESP32-S2
#define Button_START_RIGHT 2                                                    //Right compartment Start button pin connected to Pin2 of ESP32-S2
#define Button_PLUS 3                                                           //Threshold setting increment button pin connected to Pin3 of ESP32-S2
#define Button_MINUS 4                                                          //Threshold setting decrement button pin connected to Pin4 of ESP32-S2

bool Button_START_LEFT_pressed = false;                                         //Variable to store left compartment Start button pin state
bool Button_START_RIGHT_pressed = false;                                        //Variable to store right compartment Start button pin state
bool Button_PLUS_pressed = false;                                               //Variable to store threshold setting increment button pin state
bool Button_MINUS_pressed = false;                                              //Variable to store threshold setting decrement button pin state

unsigned long long PRESS_DURATION;

void IRAM_ATTR isr_start_left() 
{
  Button_START_LEFT_pressed = true;                                             //Update left compartment Start button pin state based on Interrupts
}

void IRAM_ATTR isr_start_right() 
{
  Button_START_RIGHT_pressed = true;                                            //Update right compartment Start button pin state based on Interrupts
}

void IRAM_ATTR isr_plus() 
{
  Button_PLUS_pressed = true;                                                   //Update threshold setting increment button pin state based on Interrupts
}

void IRAM_ATTR isr_minus() 
{
  Button_MINUS_pressed = true;                                                  //Update threshold setting decrement button pin state based on Interrupts
}

uint8_t weight_threshold = 0;                                                   //Variable to store weight threshold value

#define BUZZER 10                                                               //Buzzer pin connected to Pin10 of ESP32-S2
#define IR_L 15                                                                 //Left compartment IR sensor pin connected to Pin15 of ESP32-S2
#define IR_R 16                                                                 //Right compartment IR sensor pin connected to Pin16 of ESP32-S2

// RGB LEDs
const uint8_t RGB_S_L_PIN         = 39;                                         //Left compartment RGB Round Strip pin connected to Pin39 of ESP32-S2
const uint8_t RGB_S_L_NUMPIXELS   = 6;                                          //No. of LEDs in left compartment RGB Round Strip
const uint8_t RGB_S_R_PIN         = 40;                                         //Right compartment RGB Round Strip pin connected to Pin40 of ESP32-S2
const uint8_t RGB_S_R_NUMPIXELS   = 6;                                          //No. of LEDs in right compartment RGB Round Strip
const uint8_t RGB_L_PIN           = 41;                                         //Left compartment RGB Straight Strip pin connected to Pin41 of ESP32-S2
const uint8_t RGB_L_NUMPIXELS     = 8;                                          //No. of LEDs in left compartment RGB Straight Strip
const uint8_t RGB_R_PIN           = 42;                                         //Right compartment RGB Straight Strip pin connected to Pin42 of ESP32-S2
const uint8_t RGB_R_NUMPIXELS     = 8;                                          //No. of LEDs in right compartment RGB Straight Strip

uint8_t trail_rgb_s_l_arr[RGB_S_L_NUMPIXELS];
uint8_t trail_rgb_l_arr[RGB_L_NUMPIXELS];
uint8_t trail_rgb_s_r_arr[RGB_S_R_NUMPIXELS];
uint8_t trail_rgb_r_arr[RGB_R_NUMPIXELS];

unsigned long delayMillis = 0;
unsigned long SECOND_TO_MILLIS = 1000;
#define Red_hue 96
#define Green_hue 0
#define CIRCLE_SPEED 10
#define STRIP_SPEED 15

#include <FastLED.h>                                                            //Library for controlling RGB LEDs
#include <Arduino.h>

CHSV output_color;

CRGB RGB_S_L_pixels[RGB_S_L_NUMPIXELS];                                         //Declaring left compartment RGB Straight Strip's instance
CRGB RGB_S_R_pixels[RGB_S_R_NUMPIXELS];                                         //Declaring right compartment RGB Straight Strip's instance
CRGB RGB_L_pixels[RGB_L_NUMPIXELS];                                             //Declaring left compartment RGB Round Strip's instance
CRGB RGB_R_pixels[RGB_R_NUMPIXELS];                                             //Declaring right compartment RGB Round Strip's instance

// EEPROM
#include "EEPROM.h"                                                             //Library for EEPROM
#define EEPROM_SIZE 512                                                         //EEPROM size

int vm_status = 0;                                                              //Variable to get status of Stripe payment process fron Onion Omega 2S
unsigned long long five_seconds, sixty_seconds;                                //Variable to check 30 seconds span
//  Misc. Variables
bool no_serial_response_break_flag = false;
unsigned long long buzzer_beep_delay;
unsigned long long rgb_r_flash_delay, rgb_l_flash_delay, rgb_s_r_flash_delay, rgb_s_l_flash_delay;
bool buzzer_beep_flag = false;
bool rgb_r_flash_flag = false, rgb_l_flash_flag = false, rgb_s_r_flash_flag = false, rgb_s_l_flash_flag = false;

void setup() {
  // put your setup code here, to run once:
  if(debug)
  {
    Serial.begin(115200);                                                       //Starting the Serial Monitor
  }
  boot();                                                                       //Boot function setting the configurations at start up
}

void loop() {
  // put your main code here, to run repeatedly:
  threshold_setting();                                                          //Run the Threshold Setting function in loop
  supplement_dispense();                                                        //Run the Supplement Dispense function in loop
}