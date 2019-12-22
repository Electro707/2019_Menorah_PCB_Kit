/************************************************************************************************************************
 *  Touch Menorah Attiny84 Code
 *  By Jamal Bouajjaj
 *
 *  This program controls the LEDs on the 2019 UNH Makerspace Menorah soldering kit. There is a capacitive touch
 *       sensor on the board that you are able to touch to increment the LEDs that are on. All the LEDs are all driven
 *       to GND by a 2N3904 controlled by this controller for PWM control
************************************************************************************************************************/
#include <CapacitiveSensor.h>     // Import the library

#define TRANSISTOR_PIN 8          // Define the pin in which the transistor for the LEDs.
#define CAPSENSE_SEND 9           // Define the capacitive touch send pin
#define CAPSENSE_REC  10          // Define the capacitive touch recieve pin
#define TOUCH_THRESHOLD 110       // Set the threshold value for the captouch

int LEDs_on = 0;                  // Store the number of LEDs that are on
bool last_state = false;          // Store the last capacitive touch state ( pressed or not pressed)
int pwm_millis = 0;

CapacitiveSensor   cs = CapacitiveSensor(CAPSENSE_SEND, CAPSENSE_REC);   // Create the CapacitiveSensor class

void setup()                    
{
   //cs.set_CS_AutocaL_Millis(0xFFFFFFFF);     // Turn off autocalibrate on the capacitive touch channel
   pinMode(TRANSISTOR_PIN, OUTPUT);          // Set the Transistor pin as an output
   // Set the entirety of PORT A as output (All LEDs are connected to this port)
   DDRA = 0xFF;
   
   // The following code is to save power on unused preiferals
   ACSR |= (1<<ACD);              // Turn off Analog Comparator
   ADCSRA &= (1<<ADEN);           // Turn off ADC
   PRR |= (1<<PRADC);             // Shut down ADC
   
    //digitalWrite(TRANSISTOR_PIN, HIGH);.
    pwm_millis = millis();
}

void loop()                    
{
    long cs_val =  cs.capacitiveSensor(30);     // Detect the arbritrary capacitance on the copper pad

    // If above threshold and previous state is off, increment the operating mode
    if(cs_val > TOUCH_THRESHOLD){
      if(last_state == false){    // If pressed and the previous state is off, change the mode
        last_state = true;
        LEDs_on++;
        if(LEDs_on > 8){
          LEDs_on = 0;
        }
        turnOnLEDs(LEDs_on);
        delay(100);
      }
    }
    else{   // If the previous press state is on and the pad isn't pressed, revert the previous pressed state to false
      if(last_state == true){
        last_state = false;
        delay(50);
      }
    }

    if(millis() - pwm_millis > 50){
      uint8_t rand_pwm = random(100,180);
      analogWrite(TRANSISTOR_PIN, rand_pwm);
      pwm_millis = millis();
    }
}

// Function to turn on a certain number of LEDs from left to right
void turnOnLEDs(int numb_leds){
  if(numb_leds > 8){numb_leds = 8;}
  PORTA = ((1<<(8-numb_leds)) - 1);
}
