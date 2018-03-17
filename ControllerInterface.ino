// constants won't change. They're used here to 
// set pin numbers:
#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 

const int buttonPinUp = 2;     // the number of the pushbutton pin 2
const int buttonPinDown = 4;     // the number of the pushbutton pin 4
const int encoder0PinA = 8;
const int ledPin =  13;      // the number of the LED pin
const int time = 15;          //the amount of time between changing position of motor

// variables will change:
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int buttonStateUp = 0;         // variable for reading the pushbutton status
int buttonStateDown = 0;         // variable for reading the pushbutton status
int move = 0;                    //variable used to move the motor
int moveTo = 0;                 //variable used as restraint of motor movement
int resistanceLevel = 0;        //variable to hold the current resistance level
int pos = 0;    // variable to store the servo position 
int encoder0Pos = 0;
int encoder0PinALast = LOW;
int n = LOW;

boolean start = false;          //used for startup of program (resets motor to 0) to synch the pos variable
boolean pressUp = false;        //whether the green button is pressed
boolean pressDown = false;      //whether the red button is pressed


void setup() {
// initialize the LED pin as an output:
  // initialize serial:
  Serial.begin(1200);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  pinMode(ledPin, OUTPUT);      //initializes LED
  pinMode (encoder0PinA,INPUT); //initializes encoder
  // initialize the pushbutton pin as an input:
  pinMode(buttonPinUp, INPUT); 
  pinMode(buttonPinDown, INPUT);   
  myservo.attach(11);  // attaches the servo on pin 11 to the servo object 
}


void loop()
{
    //Resets the motor
  if (start == false)  //if program has started already
  {
     myservo.write(pos);  //resets motor to position 0
     start = true;   //sets program to have started
  }
  
  // read the state of the pushbutton value:
  buttonStateUp = digitalRead(buttonPinUp);
  buttonStateDown = digitalRead(buttonPinDown);
  Serial.print(resistanceLevel);
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  
  
  //for up button
  
  if (buttonStateUp == HIGH) 
  {                                //if button is pressed
  if (pressUp == false)
  {
    {
      digitalWrite(ledPin, HIGH);  //increases resistance by 1 if within 0-8
      pressUp = true;
      if (resistanceLevel < 8)  //if the resistance level is below 8
      {
        resistanceLevel = resistanceLevel +1;   //increase level by 1
        //Serial.println(resistanceLevel);  //update level to computer
      }
    } 
  }
  }
  buttonStateUp = digitalRead(buttonPinUp);  //read button state
  if (buttonStateUp == LOW)      //if button is released
  {
    pressUp = false;  //set flag as false
    digitalWrite(ledPin, LOW);  //turn LED off
  }
  
  /////////////////////////////////////////////////////////////////////////////////////
  
    if (buttonStateDown == HIGH)   //if red button is pressed
  {
  if (pressDown == false)    //and flag is false
  {
    {
      digitalWrite(ledPin, HIGH);  //turns LED on
      pressDown = true;    //reset flag
      if (resistanceLevel > 0)  //If the resistance Level is above 0
      {
        resistanceLevel = resistanceLevel - 1;   //resistance level decreased
        //Serial.println(resistanceLevel);  //update level to computer
      }
    } 
  }
  }
  buttonStateDown = digitalRead(buttonPinDown);   //read button state
  
  if (buttonStateDown == LOW)  //if button is released
  {
    pressDown = false;  //resets flag
    digitalWrite(ledPin, LOW);  //LED turned off
  }
  
  //////////////////////////////////////
  
  moveTo = resistanceLevel * 20;   //sets the position motor should go to based off of current resistance
 
  if (pos < moveTo)  //if the current position is less than it should be
  {
   pos = pos + 1;        //moves the motor up if needed
   myservo.write(pos);
  Serial.print(pos); 
   delay(time);             //moves the motor slower
  }
  
  if (pos > moveTo)  //if the current posision is more than it should be
  {
   pos = pos - 1;         //moves the motor down if needed
   myservo.write(pos);
  Serial.print(pos); 
   delay(time);              //moves the motor slower
  }
  
  ////////////////////////////////////////////////////////////////////////
/* 
  This part is dedicated to sending the computer flags when the encoder
  senses a full rotation
*/

  n = digitalRead(encoder0PinA);
   if ((encoder0PinALast == LOW) && (n == HIGH)) {
       encoder0Pos++;
     }
     if (encoder0Pos == 48)
     {
     Serial.print (-1); // '-1' is the flag for a rotation
     }
   encoder0PinALast = n;
  
} //end of loop


//////////////////////////////////////////////////////////////////////////////

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}
