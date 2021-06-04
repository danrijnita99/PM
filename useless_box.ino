

/* www.learningbuz.com */
/*Impport following Libraries*/
#include <Wire.h> 
#include <Servo.h> 
 
#include <LiquidCrystal_I2C.h>
//I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

//stepper pins
const int dirPin = 3;
const int stepPin = 4;

//servo pins
const int handPin = 5;
const int doorPin = 6; 

//buttons
const int stopButtonPin = 7;
const int sw1Pin = 8;
const int sw2Pin = 9;
const int sw3Pin = 10;

//status flags
int sw1Status = 0;
int sw2Status = 0;
int sw3Status = 0;
int buttonStatus = 0;

//switch positions

const int sw3X = 115;
const int sw2X = 329;
const int sw1X = 561;

// servodeclarations 
Servo doorServo;
Servo handServo;
int currentHandPosition; 
void setup() 
{

  Serial.begin(9600);
  doorServo.attach(doorPin);
  handServo.attach(handPin);
  doorServo.write(0);    
  handServo.write(0);
 
  // declare pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  
  pinMode(sw1Pin, INPUT);
  digitalWrite(sw1Pin, HIGH);

  pinMode(sw2Pin, INPUT);
  digitalWrite(sw2Pin, HIGH);
  
  pinMode(sw3Pin, INPUT);
  digitalWrite(sw3Pin, HIGH);

//  // edge detection
  pinMode(stopButtonPin, INPUT);
  digitalWrite(stopButtonPin, HIGH);
  digitalWrite(dirPin, LOW);
  buttonStatus = digitalRead(stopButtonPin);
  
  while (buttonStatus == 1) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000); 
    buttonStatus = digitalRead(stopButtonPin);
  }

  // back off 5mm (start position)
  digitalWrite(dirPin, HIGH);
  for (int i = 0; i < 30; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
  currentHandPosition = 0;
  
  lcd.begin(16,2);//defining 16 columns and 2 rows of lcd display
  lcd.backlight();//to Power ON the back light
  lcd.backlight();//to Power OFF the back light
}

void handleSwitch(int currentPos, int finalPos) 
{
  if (currentPos < finalPos) {
    digitalWrite(dirPin, HIGH);
    for (int i = currentPos; i < finalPos; i++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(500);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(500);
    }
  } else {
    digitalWrite(dirPin, LOW);
    for (int i = finalPos; i < currentPos; i++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(500);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(500);
    }
  }
  //doorServo.write(70);
  //delay(200);
  handServo.write(93);
  delay(500);
  handServo.write(0); 
  delay(500);
}

void loop() 
{
  sw1Status = digitalRead(sw1Pin);
  sw2Status = digitalRead(sw2Pin);
  sw3Status = digitalRead(sw3Pin);

  if (sw1Status == 0 && sw2Status == 0 && sw3Status == 0) {
    doorServo.write(0);
  } else {
    doorServo.write(70);
  }
  
  if (sw1Status != 0) {
    handleSwitch(currentHandPosition, sw1X);
    currentHandPosition = sw1X;
  }

  if (sw2Status != 0) {
    handleSwitch(currentHandPosition, sw2X);
    currentHandPosition = sw2X;

  }

  if (sw3Status != 0) {
    handleSwitch(currentHandPosition, sw3X);
    currentHandPosition = sw3X;
  }

  lcd.setCursor(0,0); // first row,first column .
  lcd.print("  Useless  "); //You can write 16 Characters per line .
  lcd.setCursor(0,1);  //Defining positon to write from second row,first column .
  lcd.print("    Box  ");
}
