// constants won't change. They're used here to 
// set pin numbers:
#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 


const int buttonPin1 = 2;     // the number of the pushbutton 1 pin 2
const int buttonPin2 = 4;     // the number of the pushbutton 2 pin 4
const int buttonPin3 = 6;    //the number of bushbutton 3 pin 6
const int buttonPin4 = 12;    //the number of bushbutton 4 pin 12
const int encoder0PinA = 8;  //the number of the encoder pin 8
const int ledPin =  13;      // the number of the LED pin
const int time = 15;          //the amount of time between changing position of motor

// variables will change:
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int buttonState1 = 0;         // variable for reading the pushbutton 1 status
int buttonState2 = 0;         // variable for reading the pushbutton 2 status
int buttonState3 = 0;         // variable for reading the pushbutton 3 status
int buttonState4 = 0;         // variable for reading the pushbutton 4 status
int move = 0;                   //variable used to move the motor
int moveTo = 0;                 //variable used as restraint of motor movement
int resistanceLevel = 0;        //variable to hold the current resistance level
int pos = 0;                    // variable to store the servo position 
int encoder0Pos = 0;           //counter for 1/48 revolutions of the encoder
int encoder0PinALast = LOW;    //variable used to hold last value of channel A
int n = LOW;                   //variable used to hold value of channel A
char inChar, inHold, p, *l;
boolean start = false;       //used for startup of program (resets motor to 0) to synch the pos variable
boolean press1 = false;      //whether button 2 is pressed
boolean press2 = false;      //whether button 2 is pressed
boolean press3 = false;      //whether button 3 is pressed
boolean press4 = false;      //whether button 4 is pressed


void setup() {
// initialize the LED pin as an output:
  // initialize serial:
  Serial.begin(1200);             //initializes serial port
  inputString.reserve(200);       // reserve 200 bytes for the inputString:
  pinMode(ledPin, OUTPUT);        //initialize pin for LED      
  pinMode (encoder0PinA,INPUT);   //initialize pin for encoder
  // initialize the pushbutton pins as an input:
  pinMode(buttonPin1, INPUT); 
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);  
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
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  buttonState4 = digitalRead(buttonPin4);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  
  
  //for Button1
  
  if (buttonState1 == HIGH && press1 == false) 
  {                                //if button 1 is pressed
      
      digitalWrite(ledPin, HIGH); 
      press1 = true;
      Serial.println(1);  //send computer flag for button1
       
  }
  buttonState1 = digitalRead(buttonPin1);  //read button state
  if (buttonState1 == LOW && press1 == true)      //if button 1 is released
  {
    press1 = false;  //set flag as false
    digitalWrite(ledPin, LOW);  //turn LED off
    delay(25);
  }
  
  /////////////////////////////////////////////////////////////////////////////////////
  
  //for Button 2
  
  if (buttonState2 == HIGH && press2 == false) 
  {                                //if button 2 is pressed
      digitalWrite(ledPin, HIGH); 
      press2 = true;
      Serial.println(2);  //send computer flag for button 2
  }
  buttonState2 = digitalRead(buttonPin2);  //read button state
  if (buttonState2 == LOW && press2 == true)      //if button 2 is released
  {
    press2 = false;  //set flag as false
    digitalWrite(ledPin, LOW);  //turn LED off
    delay(25);
  }
  
////////////////////////////////////////////////////////////////////////  

 //for Button 3
  
  if (buttonState3 == HIGH && press3 == false) 
  {                                //if button 3 is pressed
      digitalWrite(ledPin, HIGH);  
      press3 = true;
      Serial.println(3);  //send computer flag for button 3
  }
  buttonState3 = digitalRead(buttonPin3);  //read button state
  if (buttonState3 == LOW && press3 == true)      //if button 3 is released
  {
    press3 = false;  //set flag as false
    digitalWrite(ledPin, LOW);  //turn LED off
    delay(25);
  }
  
  /////////////////////////////////////////////////////////////////////////////////////
  
  //for Button 4
  
  if (buttonState4 == HIGH && press4 == false) 
  {                                //if button 4 is pressed
      digitalWrite(ledPin, HIGH);  
      press4 = true;
      Serial.println(4);  //send computer flag for button 4
  }
  buttonState4 = digitalRead(buttonPin4);  //read button state
  if (buttonState4 == LOW && press4 == true)      //if button 4 is released
  {
    press4 = false;  //set flag as false
    digitalWrite(ledPin, LOW);  //turn LED off
    delay(25);
  }
  
  /////////////////////////////////////////////////////////////////////////////////////
  
  moveTo = 40 + (resistanceLevel * 12);   //sets the position motor should go to based off of current resistance
  
  if (pos < moveTo)  //if the current position is less than it should be
  {
   pos = pos + 1;        //moves the motor up if needed
   myservo.write(pos); 
   delay(time);             //moves the motor slower
  }
  
  if (pos > moveTo)  //if the current posision is more than it should be
  {
   pos = pos - 1;         //moves the motor down if needed
   myservo.write(pos); 
   delay(time);              //moves the motor slower
  }
  
 ///////////////////////////////////////////////////////////////////////// 
  /* 
  This part is dedicated to sending the computer flags when the encoder
  senses a full rotation
*/

  n = digitalRead(encoder0PinA);
   if ((encoder0PinALast == LOW) && (n == HIGH)) {
       encoder0Pos++;
     }
     if (encoder0Pos == 1) //48 ticks per revolution
     {
     Serial.println (5); // '5' is the flag for 1/48 of a rotation
     encoder0Pos = 0;
     }
   encoder0PinALast = n;
 
 
}//end of loop


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
    inChar = (char)Serial.read(); 
    
    if (inChar == '\n')   //if the new character is a new line
    {
    } 
    else        //if the new character isn't new line
    {
      p = inChar;     //convert the character to an integer
    *l = p;
    if (atoi(l) < 9 && atoi(l) > -1)
      {
        resistanceLevel = atoi(l);    //updates the resistance level
      }
    }
  }
}