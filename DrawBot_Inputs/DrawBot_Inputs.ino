/* 
Preliminary draw bot sketch - Etch-A-Sketch type functionality
James Hosken
July 2018
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *xControl = AFMS.getMotor(1);
Adafruit_DCMotor *zControl = AFMS.getMotor(2);
Adafruit_StepperMotor *yControl = AFMS.getStepper(200, 2);



void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Starting");
  
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  Serial.println("MOTOR INIT");
  // Set the speed to start, from 0 (off) to 255 (max speed)
  xControl->setSpeed(100);
  yControl->setSpeed(60);
  zControl->setSpeed(20);
  Serial.println("All Speed Set");
  xControl->run(RELEASE);
  zControl->run(RELEASE);

  pinMode(A0, INPUT);       // X Control Limit Switch
  pinMode(2, INPUT_PULLUP); //Y Control Limit Switch

  pinMode(A1, INPUT);       //Extra Pot
  pinMode(A2, INPUT);       //Debug Control X
  pinMode(A3, INPUT);       //Debug Control Y
}

void loop() {
  read_inputs();
  //home_axes(); //Todo: create home axes function

  int xVal = analogRead(A3)/2;
  if(xVal < 256){
    xControl ->run(FORWARD);
    xControl ->setSpeed(256 - xVal);
  }else{
    xControl ->run(BACKWARD);
    xControl -> setSpeed(xVal - 256);
  }

  int yVal = analogRead(A2);
  if(yVal < 265){
    yControl ->step(10, FORWARD, SINGLE);
    yControl ->setSpeed((256 - yVal));
  }else if(yVal > 765){
    yControl -> step(10, BACKWARD, SINGLE);
    yControl -> setSpeed((yVal - 765));
  }else{
    yControl ->release();
  }

  int zVal = analogRead(A1)/2;
  if(zVal < 256){
    zControl ->run(FORWARD);
    zControl ->setSpeed((256 - zVal)/2);
  }else{
    zControl ->run(BACKWARD);
    zControl -> setSpeed((zVal - 256)/2);
  }
}

void home_axes(){
  // Do Nothing
}

void read_inputs(){
  
  Serial.print("XLIM: \t");
  Serial.print(analogRead(A0));

  Serial.print("\t\tYLIM: \t");
  Serial.print(digitalRead(2));

  Serial.print("\t\tPOT: \t");
  Serial.print(analogRead(A1));

  Serial.print("\t\tYPOT: \t");
  Serial.print(analogRead(A2));

  Serial.print("\t\tXPOT: \t");
  Serial.println(analogRead(A3));

}

