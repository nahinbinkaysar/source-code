#include <QTRSensors.h>
#include <math.h>

#define Kp 23 // experiment to determine this, start by something small that just makes your bot follow the line at a slow speed
#define Kd 500 // experiment to determine this, slowly increase the speeds and adjust this value. ( Note: Kp < Kd) 
#define rightMaxSpeed 180 // max speed of the robot
#define leftMaxSpeed 180 // max speed of the robot
#define rightBaseSpeed 90 // this is the speed at which the motors should spin when the robot is perfectly on the line
#define leftBaseSpeed 90 // this is the speed at which the motors should spin when the robot is perfectly on the line
#define NUM_SENSORS 10
#define EMITTER_PIN 23
#define SET_POINT 4.5

#define A 6
#define B 7

#define C 8
#define D 9

QTRSensors qtr;

unsigned int sensorValues[NUM_SENSORS];
unsigned int thresholdValues[NUM_SENSORS] = {706, 706, 504, 596, 444, 476, 382, 446, 706, 748};
float position;
uint16_t binaryValue;
uint8_t occurenceCount;
int flag;

uint16_t sensorConversion() {
    qtr.read(sensorValues);
    bool digitalValue;
    binaryValue = 0;
    occurenceCount = 0;
    uint8_t positionTemp = 0;

    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        if (sensorValues[i] > thresholdValues[i]) {
            digitalValue = 1;
            binaryValue |= digitalValue << (NUM_SENSORS - (1 + i));
            positionTemp += i;
            occurenceCount++;
        } else {
            digitalValue = 0;
            binaryValue |= digitalValue << (NUM_SENSORS - (1 + i));
        }
        Serial.print(digitalValue);
    }
    Serial.print("     position: ");
    position = (float) positionTemp / (float) occurenceCount;
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
    Serial.begin(9600);
}

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

void advancedMotorControl(int turn) {
    float error = SET_POINT - position;

    float motorSpeed = Kp * error + Kd * (error - lastError);

    lastError = error;

    int rightMotorSpeed = rightBaseSpeed + (int) motorSpeed;
    int leftMotorSpeed = leftBaseSpeed - (int) motorSpeed;

    if (rightMotorSpeed > rightMaxSpeed) rightMotorSpeed = rightMaxSpeed; // prevent the motor from going beyond max speed
    if (leftMotorSpeed > leftMaxSpeed) leftMotorSpeed = leftMaxSpeed; // prevent the motor from going beyond max speed
    if (rightMotorSpeed < 0) rightMotorSpeed = 0; // keep the motor speed positive
    if (leftMotorSpeed < 0) leftMotorSpeed = 0; // keep the motor speed positive

    if (turn == 1) {
        motorPower(leftMotorSpeed * 0.8, 0, 0, rightMotorSpeed * 0.8);
        flag = 1;
    }
    else if (turn == -1) {
        motorPower(0, leftMotorSpeed * 0.8, rightMotorSpeed * 0.8, 0);
        flag = -1;
    }
    else motorPower(leftMotorSpeed, 0, rightMotorSpeed, 0);
}

void loop() {
	sensorConversion();
  
    if (binaryValue == 0b0000000111 || binaryValue == 0b0000001111 || binaryValue == 0b0000011111 || binaryValue == 0b0000111111) {
    	advancedMotorControl(1);
    }

    if (binaryValue == 0b1110000000 || binaryValue == 0b1111000000 || binaryValue == 0b1111100000 || binaryValue == 0b1111110000) {
        advancedMotorControl(-1);
    }

    else if (binaryValue == 0b0000000000) {
        if (flag == 1) motorPower(200, 0, 0, 200);
        else if (flag == -1) motorPower(0, 200, 200, 0);
    }


    else advancedMotorControl(0);
    flag = 0;
}
