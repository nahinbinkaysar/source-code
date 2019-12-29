#include <QTRSensors.h>
#include <math.h>
#include <LiquidCrystal.h>

#define Kp 7 // experiment to determine this, start by something small that just makes your bot follow the line at a slow speed
#define Kd 50 // experiment to determine this, slowly increase the speeds and adjust this value. ( Note: Kp < Kd) 
#define rightMaxSpeed 180 // max speed of the robot
#define leftMaxSpeed 180 // max speed of the robot
#define rightBaseSpeed 150 // this is the speed at which the motors should spin when the robot is perfectly on the line
#define leftBaseSpeed 150 // this is the speed at which the motors should spin when the robot is perfectly on the line
#define NUM_SENSORS 10 // number of sensors used
#define TIMEOUT 2500 // waits for 2500 us for sensor outputs to go low
#define EMITTER_PIN 23 // emitter is controlled by digital pin 2
#define SET_POINT 4.5
//left motor: black-white
#define A 41 //forward-black
#define B 40 //back-white

//right motor: yellow-green
#define C 35 //forward-yellow
#define D 34 //back-green

//LCD pin to Arduino
#define pin_RS 8 
#define pin_EN 9 
#define pin_d4 4 
#define pin_d5 5
#define pin_d6 6 
#define pin_d7 7 

#define pin_BL 10

QTRSensorsRC qtrrc((unsigned char[]) {A1, A2, A3, A4, A5, A8 ,A9 ,A10, A11, A12}, NUM_SENSORS, TIMEOUT, EMITTER_PIN); // sensor connected through analog pins A0 - A5 i.e. digital pins 14-19
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);

unsigned int sensorValues[NUM_SENSORS];
unsigned int thresholdValues[NUM_SENSORS];
float position;

uint8_t sensorConversion(void) {
	bool digitalValue;
	uint8_t binaryValue = 0;
	uint8_t occurenceCount = 0;
	uint8_t positionTemp = 0;
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        if (sensorValues[i] > thresholdValues[i]) {
        	digitalValue = 1;
        	binaryValue |= digitalValue << (NUM_SENSORS - (1 + i));
        	positionTemp += i;
        	occurenceCount++;
        }
        else {
        	digitalValue = 0;
        	binaryValue |= digitalValue << (NUM_SENSORS - (1 + i));
        }
    }
    position = (float)positionTemp / (float)occurenceCount; 
    return binaryValue;
}


void turn_right() {
    analogWrite(A, 150);
    analogWrite(B, 0);
    analogWrite(C, 0);
    analogWrite(D, 150);
}

void turn_left() {
    analogWrite(A, 0);
    analogWrite(B, 150);
    analogWrite(C, 150);
    analogWrite(D, 0);
}
void stop() {
    analogWrite(A, 0);
    analogWrite(B, 0);
    analogWrite(C, 0);
    analogWrite(D, 0);
}

void setup() {
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(D, OUTPUT);
    Serial.begin(9600);
	lcd.begin(16, 2);

  	lcd.setCursor(0,0);
  	lcd.print("Calibrating");

    for (uint8_t i = 0; i < 200; i++) { // calibrate for sometime by sliding the sensors across the line, or you may use auto-calibration instead

        qtrrc.calibrate();
        delay(20);
    }
    stop();
    lcd.print("Cal Done");
    delay(2000); // wait for 2s to position the bot before entering the main loop 
    lcd.print("                ");
    // comment out for serial printing  
    Serial.begin(9600);
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        Serial.print(qtrrc.calibratedMinimumOn[i]);
        Serial.print(' ');
    }
    Serial.println();

    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        Serial.print(qtrrc.calibratedMaximumOn[i]);
        Serial.print(' ');
    }
    for(uint8_t i = 0; i < NUM_SENSORS; i++) {
        thresholdValues[i] = floor((float)(qtrrc.calibratedMaximumOn[i] + qtrrc.calibratedMinimumOn[i]) / 2);
    }

    Serial.println();
    Serial.println();
    //
}

float lastError = 0;

void loop() {
    qtrrc.readLine(sensorValues);
    position = 0;
    uint8_t binaryValue = sensorConversion();


    
    Serial.print(binaryValue, BIN);
    Serial.print('\t');
    Serial.print(position);
	Serial.println();    

    float error = SET_POINT - position;
    

    float motorSpeed = Kp * error + Kd * (error - lastError);
    
    lastError = error;

    int rightMotorSpeed = rightBaseSpeed + (int)motorSpeed;
    int leftMotorSpeed = leftBaseSpeed - (int)motorSpeed;


    Serial.print(rightMotorSpeed);
    lcd.setCursor(0,0);
    lcd.print(rightMotorSpeed);
    

    Serial.print(' ');
   

    Serial.println(leftMotorSpeed);
    lcd.setCursor(0,1);
    lcd.print(leftMotorSpeed);
    
    if (rightMotorSpeed > rightMaxSpeed) rightMotorSpeed = rightMaxSpeed; // prevent the motor from going beyond max speed
    if (leftMotorSpeed > leftMaxSpeed) leftMotorSpeed = leftMaxSpeed; // prevent the motor from going beyond max speed
    if (rightMotorSpeed < 0) rightMotorSpeed = 0; // keep the motor speed positive
    if (leftMotorSpeed < 0) leftMotorSpeed = 0; // keep the motor speed positive

    
    analogWrite(A, leftMotorSpeed);
    analogWrite(B, 0);
    analogWrite(C, rightMotorSpeed);
    analogWrite(D, 0);
    lcd.print("                ");
    delay(300);

}
