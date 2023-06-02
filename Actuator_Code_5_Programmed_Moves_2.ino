#define RPWM1 2
#define LPWM1 3
#define REN1 14
#define LEN1 15

#define RPWM2 4
#define LPWM2 5
#define REN2 16
#define LEN2 17

#define RPWM3 6
#define LPWM3 7
#define REN3 23
#define LEN3 25

#define SW1 38
#define SW2 52
#define SW3 36
#define SW4 50
#define SW5 34
#define SW6 48

#define ENC1 18
#define ENC2 19
#define ENC3 20

#define LIM1 26
#define LIM2 27
#define LIM3 28

#define EncoderConstant1 2600 //conversion from mm to encoder counts for linear actuators
#define EncoderConstant3 377
#define MaxExtension1 105 //max. linear actuator extension in mm
#define MaxExtension3 200 //max. linear slider movement range in mm

volatile long int ENC1count = 0;
volatile long int ENC2count = 0;
volatile long int ENC3count = 0;

int motordir1 = 0;
int motordir2 = 0;
int motordir3 = 0;

//Approximately 2600 encoder counts per mm of actuator travel
void ISR_ENC1(){
  if (motordir1 == 1) {
    ENC1count++;
  }
  if (motordir1 == -1) {
    ENC1count--;
  }
}

void ISR_ENC2(){
  if (motordir2 == 1) {
    ENC2count++;
  }
  if (motordir2 == -1) {
    ENC2count--;
  }
}

void ISR_ENC3(){
  if (motordir3 == 1) {
    ENC3count++;
  }
  if (motordir3 == -1) {
    ENC3count--;
  }
}

//function to set zero point for actuators using their limit switches
void Calibration(){

  analogWrite(RPWM1,0);
  analogWrite(LPWM1,255); //run actuator inwards 
  while (digitalRead(LIM1) == HIGH) {} //keep running actuator until the limit switch is pressed
  analogWrite(RPWM1,0);
  analogWrite(LPWM1,0); //stop the actuator
  delay(100);
  ENC1count = 0; //zero the encoder count
  motordir1 = 1;
  analogWrite(RPWM1,100); //run the actuator outwards at low speed to set soft endstop
  analogWrite(LPWM1,0);
  while (ENC1count < 1000) {} //wait until actuator is 1000 encoder counts away from limit switch endstop
  analogWrite(RPWM1,0);
  analogWrite(LPWM1,0); //stop the actuator
  ENC1count = 0; //zero the encoder count again

  analogWrite(RPWM2,0);
  analogWrite(LPWM2,255); //run actuator inwards 
  while (digitalRead(LIM2) == HIGH) {} //keep running actuator until the limit switch is pressed
  analogWrite(RPWM2,0);
  analogWrite(LPWM2,0); //stop the actuator
  delay(100);
  ENC2count = 0; //zero the encoder count
  motordir2 = 1;
  analogWrite(RPWM2,100); //run the actuator outwards at low speed to set soft endstop
  analogWrite(LPWM2,0);
  while (ENC2count < 1000) {} //wait until actuator is 1000 encoder counts away from limit switch endstop
  analogWrite(RPWM2,0);
  analogWrite(LPWM2,0); //stop the actuator
  ENC2count = 0; //zero the encoder count again
  
  ENC3count = 0; //zero encoder 3
  
}

//function to make actuator 1 reach a particular extension at a defined speed
void actuator1Position(int extension, int velocity) { //note: extension is in mm, velocity is between 0 and 255
  if (extension > MaxExtension1) {
    extension = MaxExtension1;
  }
  
  if (ENC1count/EncoderConstant1 < extension) {
    motordir1 = 1;
    analogWrite(RPWM1,velocity);
    analogWrite(LPWM1,0);
    while ((ENC1count/EncoderConstant1 < extension)) {}
    analogWrite(RPWM1,0);
    analogWrite(LPWM1,0);
  }
  else {
    motordir1 = -1;
    analogWrite(RPWM1,0);
    analogWrite(LPWM1,velocity);
    while ((ENC1count/EncoderConstant1 > extension)) {}
    analogWrite(RPWM1,0);
    analogWrite(LPWM1,0);
  }

  
}

//function to make actuator 1 reach a particular extension at a defined speed
void actuator2Position(int extension, int velocity) { //note: extension is in mm, velocity is between 0 and 255
  if (extension > MaxExtension1) {
    extension = MaxExtension1;
  }
  
  if (ENC2count/EncoderConstant1 < extension) {
    motordir2 = 1;
    analogWrite(RPWM2,velocity);
    analogWrite(LPWM2,0);
    while ((ENC2count/EncoderConstant1 < extension)) {}
    analogWrite(RPWM2,0);
    analogWrite(LPWM2,0);
  }
  else {
    motordir2 = -1;
    analogWrite(RPWM2,0);
    analogWrite(LPWM2,velocity);
    while ((ENC2count/EncoderConstant1 > extension)) {}
    analogWrite(RPWM2,0);
    analogWrite(LPWM2,0);
  }
}

//function to make actuator 1 reach a particular extension at a defined speed
void actuator3Position(int extension, int velocity) { //note: extension is in mm, velocity is between 0 and 255
  if (extension > MaxExtension1) {
    extension = MaxExtension1;
  }
  
  if (ENC3count/EncoderConstant3 < extension) {
    motordir3 = 1;
    analogWrite(RPWM3,velocity);
    analogWrite(LPWM3,0);
    while ((ENC3count/EncoderConstant3 < extension)) {}
    analogWrite(RPWM3,0);
    analogWrite(LPWM3,0);
  }
  else {
    motordir3 = -1;
    analogWrite(RPWM3,0);
    analogWrite(LPWM3,velocity);
    while ((ENC3count/EncoderConstant3 > extension)) {}
    analogWrite(RPWM3,0);
    analogWrite(LPWM3,0);
  }

  
}


void DrillPosition(){
  actuator1Position(0,255);
  actuator3Position(-110,255);
  actuator2Position(0,255);
  actuator1Position(95,255);
  //actuator2Position(20,150);
}

void DropOffPosition(){
  //actuator1Position(50,255);
  //actuator2Position(50,255);
  actuator1Position(0,255);
  actuator3Position(0,255);
  actuator2Position(70,255);
  actuator1Position(38,150);
}

void StowagePosition(){
  actuator3Position(30,255);
}


void setup() {
  Serial.begin(9600);

  //Define motor driver pins
  pinMode(RPWM1,OUTPUT);
  pinMode(LPWM1,OUTPUT);
  pinMode(LEN1,OUTPUT);
  pinMode(REN1,OUTPUT);
  
  pinMode(RPWM2,OUTPUT);
  pinMode(LPWM2,OUTPUT);
  pinMode(LEN2,OUTPUT);
  pinMode(REN2,OUTPUT);
  
  pinMode(RPWM3,OUTPUT);
  pinMode(LPWM3,OUTPUT);
  pinMode(LEN3,OUTPUT);
  pinMode(REN3,OUTPUT);


  //Define switch input pins
  pinMode(SW1,INPUT_PULLUP);
  pinMode(SW2,INPUT_PULLUP);
  pinMode(SW3,INPUT_PULLUP);
  pinMode(SW4,INPUT_PULLUP);
  pinMode(SW5,INPUT_PULLUP);
  pinMode(SW6,INPUT_PULLUP);

  //define encoder input pins
  pinMode(ENC1, INPUT);
  pinMode(ENC2, INPUT);
  pinMode(ENC3, INPUT);

  //define limit switch input pins
  pinMode(LIM1, INPUT_PULLUP);
  pinMode(LIM2, INPUT_PULLUP);
  pinMode(LIM3, INPUT_PULLUP);

  //turn on interrupts for each motor encoder, refering to the correct ISR
  attachInterrupt(digitalPinToInterrupt(ENC1), ISR_ENC1, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC2), ISR_ENC2, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC3), ISR_ENC3, RISING);

  

  //Set all motor driver enable pins to high
  digitalWrite(REN1,HIGH);
  digitalWrite(LEN1,HIGH);
  digitalWrite(REN2,HIGH);
  digitalWrite(LEN2,HIGH);
  digitalWrite(REN3,HIGH);
  digitalWrite(LEN3,HIGH);

  Calibration(); //run calibration sequence
  
}
 
 
void loop() {

if (digitalRead(SW1) == LOW) {
    //programmed sequence of actuator positions
    DrillPosition();
    //actuator3Position(-90,255);
} 

if (digitalRead(SW2) == LOW) {
    //programmed sequence of actuator positions
    DropOffPosition();
    //actuator3Position(0,255);
}
}
