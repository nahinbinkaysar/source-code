#include <QTRSensors.h>
#include <math.h>

#define Kp 31 // experiment to determine this, start by something small that just makes your bot follow the line at a slow speed
#define Kd 620 // experiment to determine this, slowly increase the speeds and adjust this value. ( Note: Kp < Kd) 
#define rightMaxSpeed 180 // max speed of the robot
#define leftMaxSpeed 180 // max speed of the robot
#define rightBaseSpeed 50 // this is the speed at which the motors should spin when the robot is perfectly on the line
#define leftBaseSpeed 50 // this is the speed at which the motors should spin when the robot is perfectly on the line
#define NUM_SENSORS 10 // number of sensors used
#define TIMEOUT 2500 // waits for 2500 us for sensor outputs to go low
#define EMITTER_PIN 23 // emitter is controlled by digital pin 23
#define SET_POINT 4.5 // this is my own algorithm to set the SET_POINT
// notice for 10 sensor you get an array from 0
// to 9; now the setpoint is the middle point of
// the sensors; thus this is 4.5

//left motor: black-white
#define A 6 //forward-orange
#define B 7 //back-red

//right motor: yellow-green
#define C 8 //forward-yellow
#define D 9 //back-green

QTRSensors qtr;

unsigned int sensorValues[NUM_SENSORS];
unsigned int thresholdValues[NUM_SENSORS] = {706, 706, 504, 596, 444, 476, 382, 446, 706, 748};
float position;
uint16_t binaryValue;
uint8_t occurenceCount;
char turningCase;
uint8_t flag = 0;

uint16_t sensorConversion() {
    qtr.read(sensorValues); // please look for readLineWhite() for future case
    bool digitalValue; // stores the value of the sensor as 0 or 1 comparing with the value from thresholValues
    binaryValue = 0; // resets the binary representaion of position everytime
    occurenceCount = 0; // this is to store how many 1's there are for a line
    uint8_t positionTemp = 0; // this is value stores all the 1's array-ed value; for example
    // 00 00 11 00 00 should make positionTemp 9 and 11 11 11 00 00
    // should result 15
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        if (sensorValues[i] > thresholdValues[i]) {
            digitalValue = 1;
            binaryValue |= digitalValue << (NUM_SENSORS - (1 + i)); // please do some manual calculation
            positionTemp += i;
            occurenceCount++;
        } else {
            digitalValue = 0;
            binaryValue |= digitalValue << (NUM_SENSORS - (1 + i));
        }
        Serial.print(digitalValue);
    }
    Serial.print("     position: ");
    position = (float) positionTemp / (float) occurenceCount; //finally calculating average to determine where exactly the bot is
    Serial.print(position);
    Serial.print("\t");
}

void setup() {
    qtr.setTypeRC();
    qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7, A8, A9}, NUM_SENSORS);
    qtr.setEmitterPin(EMITTER_PIN);
  
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(D, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(9600);
    
    Serial.println();
    Serial.println();
}

// (0, 0, 0, 0) stops
// (255, 0, 255, 0) forward
// (0, 255, 0, 255) backward
// (255, 0, 0, 255) right
// (0, 255, 255, 0) left

void motorPower(int lFront, int lBack, int rFront, int rBack) {
    analogWrite(A, lFront);
    analogWrite(B, lBack);
    analogWrite(C, rFront);
    analogWrite(D, rBack);

    Serial.print(lFront); Serial.print(" ");
    Serial.print(lBack); Serial.print(" ");
    Serial.print(rFront); Serial.print(" ");
    Serial.print(rBack); Serial.print(" ");
    Serial.println();
}

float lastError = 0;

void advancedMotorControl() {
    float error = SET_POINT - position;

    float motorSpeed = Kp * error + Kd * (error - lastError);

    lastError = error;

    int rightMotorSpeed = rightBaseSpeed + (int) motorSpeed;
    int leftMotorSpeed = leftBaseSpeed - (int) motorSpeed;

    if (rightMotorSpeed > rightMaxSpeed) rightMotorSpeed = rightMaxSpeed; // prevent the motor from going beyond max speed
    if (leftMotorSpeed > leftMaxSpeed) leftMotorSpeed = leftMaxSpeed; // prevent the motor from going beyond max speed
    if (rightMotorSpeed < 0) rightMotorSpeed = 0; // keep the motor speed positive
    if (leftMotorSpeed < 0) leftMotorSpeed = 0; // keep the motor speed positive

    if (turningCase == 'r') {
        motorPower(leftMotorSpeed, 0, 0, rightMotorSpeed);
        flag = 1;
    }
    else if (turningCase == 'l') {
        motorPower(0, leftMotorSpeed, rightMotorSpeed, 0);
        flag = 2;
    }
    else motorPower(leftMotorSpeed, 0, rightMotorSpeed, 0);
}

void loop() {
    sensorConversion();
    turningCase = 'n';

    if (binaryValue == 0b0000000111) {
        turningCase = 'r';
        advancedMotorControl();
    }
    if (binaryValue == 0b0000001111) {
        turningCase = 'r';
        advancedMotorControl();
    }
    if (binaryValue == 0b0000011111) {
        turningCase = 'r';
        advancedMotorControl();
    }
    if (binaryValue == 0b0000111111) {
        turningCase = 'r';
        advancedMotorControl();
    }

    if (binaryValue == 0b1110000000) {
        turningCase = 'l';
        advancedMotorControl();
    }
    if (binaryValue == 0b1111000000) {
        turningCase = 'l';
        advancedMotorControl();
    }
    if (binaryValue == 0b1111100000) {
        turningCase = 'l';
        advancedMotorControl();
    }
    if (binaryValue == 0b1111110000) {
        turningCase = 'l';
        advancedMotorControl();
    }

    if (binaryValue == 0b0000000000) {
        if (flag == 1) motorPower(rightBaseSpeed, 0, 0, leftBaseSpeed);
        else if (flag == 2) motorPower(0, rightBaseSpeed, leftBaseSpeed, 0);
    }


    else advancedMotorControl();
    flag = 0;
}
