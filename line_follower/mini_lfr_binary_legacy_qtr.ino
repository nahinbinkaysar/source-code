#include <QTRSensors.h>
#include <math.h>

#define Kp 23 // experiment to determine this, start by something small that just makes your bot follow the line at a slow speed
#define Kd 500 // experiment to determine this, slowly increase the speeds and adjust this value. ( Note: Kp < Kd) 
#define rightMaxSpeed 180 // max speed of the robot
#define leftMaxSpeed 180 // max speed of the robot
#define rightBaseSpeed 90 // this is the speed at which the motors should spin when the robot is perfectly on the line
#define leftBaseSpeed 90 // this is the speed at which the motors should spin when the robot is perfectly on the line
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

QTRSensorsRC qtr((unsigned char[]) {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9}, NUM_SENSORS, TIMEOUT, EMITTER_PIN); // sensor connected through analog pins A0 - A5 i.e. digital pins 14-19

unsigned int sensorValues[NUM_SENSORS];
unsigned int thresholdValues[NUM_SENSORS];
float position;
uint16_t binaryValue;
uint8_t occurenceCount;
char turningCase;
uint8_t flag = 0;
uint16_t sensorConversion() {
    qtr.readLine(sensorValues);
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
    Serial.print("  ");
    position = (float) positionTemp / (float) occurenceCount; //finally calculating average to determine where exactly the bot is
    Serial.print(position);
    Serial.print("\t");
}

void setup() {
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(D, OUTPUT);

    Serial.begin(9600);
      pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode


    for (uint8_t i = 0; i < 200; i++) // calibrate for sometime by sliding the sensors across the line, or you may use auto-calibration instead
    {
        qtr.calibrate();
        delay(20);
    }
    
     digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED to indicate we are through with calibration

    delay(2000); // wait for 2s to position the bot before entering the main loop

    Serial.begin(9600);
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        Serial.print(qtr.calibratedMinimumOn[i]);
        Serial.print(' ');
    }
    Serial.println();

    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        Serial.print(qtr.calibratedMaximumOn[i]);
        Serial.print(' ');
    }
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        thresholdValues[i] = floor((float)(qtr.calibratedMaximumOn[i] + qtr.calibratedMinimumOn[i]) / 2);
    }

    Serial.println();
    Serial.println();
}

void motorPower(int lFront, int lBack, int rFront, int rBack) {
    analogWrite(A, lFront);
    analogWrite(B, lBack);
    analogWrite(C, rFront);
    analogWrite(D, rBack);


    if (lBack == 0 && rBack == 0) {
        Serial.print(rFront); Serial.print(' '); Serial.print(lFront);
    } else if (rFront == 0) {
        Serial.print(rBack * (-1)); Serial.print(' '); Serial.print(lFront);
    } else if (lFront == 0) {
        Serial.print(rFront); Serial.print(' '); Serial.print(lBack * (-1));
    } else if (lFront == 0 && rFront == 0) {
        Serial.print(0); Serial.print(' '); Serial.print(0);
    }

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
        motorPower(leftMotorSpeed-30, 0, 0, rightMotorSpeed-30);
        flag = 1;
    }
    else
     if (turningCase == 'l') {
        motorPower(0, leftMotorSpeed-30, rightMotorSpeed-30, 0);
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
        if (flag == 1) motorPower(200, 0, 0, 200);
        else if (flag == 2) motorPower(0, 200, 200, 0);
    }


    else advancedMotorControl();
    flag = 0;
}