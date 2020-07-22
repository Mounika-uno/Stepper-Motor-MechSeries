#include "Stepper.h"
#define STEPS 32 // number of steps for one revolution of internal shaft
           // 2048 steps for one revolution of external shaft

volatile boolean TurnDetected; // need volatile for interrupts
volatile boolean rotationdirection; // cw or ccw rotation

const int PinCLK=2;// generating interrupts using CLK signal
const int PinDT=3; // reading DT signal
const int PinSW=4; // reading Push button switch

int RotaryPosition=0; // to store stepper motor position

int PrevPosition; // previous rotatory position value to check accuracy
int StepsToTake; // how much to move stepper
/*
We are using 4 pins to control the Stepper and 3 pins for the rotary encoder module.

Pin 8-11 are controlling the Stepper motor and pin 2-4 are receiving information from the rotary encoder.

We connect the 5V and Ground from to UNO to the rotary encoder and as a precaution, use a breadboard power supply to power the Stepper motor since it can use more power that the UNO can provide.

We also connect the UNO Ground to the breadboard to serve as a reference.
*/
// setup of proper sequencing for motor driver pins
// In1, In2, In3, In4 in the sequence 1-3-2-4
Stepper small_stepper(STEPS,8,10,9,11);


// Interrupt routine runs if CLK goes from HIGH to LOW
void isr(){
  delay(4); // delay for debouncing
  if (digitalRead(PinCLK))
    rotationdirection = digitalRead(PinDT);
  else
    rotationdirection = !digitalRead(PinDT);
  TurnDetected = true;
  }


void setup() {
  // put your setup code here, to run once:
  pinMode(PinCLK, INPUT);
  pinMode(PinDT, INPUT);
  pinMode(PinSW, INPUT);
  digitalWrite(PinSW,HIGH); // Pull-Up resistor for switch
  attachInterrupt(0,isr,FALLING); // interrupt 0 always connected to pin 2 on ardunio
  
}

void loop() {
  // put your main code here, to run repeatedly:
  small_stepper.setSpeed(600); // Max seems to be 700
  
  
  if(!(digitalRead(PinSW))){ // check if button is pressed
    if(RotaryPosition == 0){ // check if button was already pressed
      
      }else{
          small_stepper.step(-(RotaryPosition*50));
          RotaryPosition = 0;// Reset position to zero
        }
    } // if

   if(TurnDetected){
    PrevPosition = RotaryPosition; // save previous position in variable
    if(rotationdirection){
      RotaryPosition = RotaryPosition-1; // decrease Position by 1
      }
    else{
      RotaryPosition = RotaryPosition+1; // increase Position by 1
      }
      TurnDetected=false; // do not Repeat IF loop until new rotation detected 
    } // if turn detected


    if( (RotaryPosition + 1) == PrevPosition ){ // Move motor CCW
      StepsToTake =- 50;
      small_stepper.step(StepsToTake);
      } // if
    
}
