#include <QTRSensors.h>
#include <math.h>

#define Kp 29 // experiment to determine this, start by something small that just makes your bot follow the line at a slow speed
#define Kd 220 // experiment to determine this, slowly increase the speeds and adjust this value. ( Note: Kp < Kd) 
#define rightMaxSpeed 250 // max speed of the robot
#define leftMaxSpeed 250 // max speed of the robot
#define rightBaseSpeed 110 // this is the speed at which the motors should spin when the robot is perfectly on the line
#define leftBaseSpeed 110 // this is the speed at which the motors should spin when the robot is perfectly on the line
#define NUM_SENSORS 8 // number of sensors used
#define TIMEOUT 2500 // waits for 2500 us for sensor outputs to go low
#define EMITTER_PIN 23 // emitter is controlled by digital pin 23
#define SET_POINT 3.5 // this is my own algorithm to set the SET_POINT
// notice for 10 sensor you get an array from 0
// to 9; now the setpoint is the middle point of
// the sensors; thus this is 4.5
#define speed 200

//left motor: black-white
#define A 6 //forward-orange
#define B 7 //back-red

//right motor: yellow-green
#define C 8 //forward-yellow
#define D 9 //back-green

QTRSensors qtr;


unsigned int sensorValues[NUM_SENSORS];
unsigned int thresholdValues[NUM_SENSORS];
float position;
uint16_t binaryValue;
uint8_t occurenceCount;

uint16_t sensorConversion() {
    qtr.readLineBlack(sensorValues);
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
    qtr.setTypeAnalog();
    qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, NUM_SENSORS);
    qtr.setEmitterPin(EMITTER_PIN);

    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(D, OUTPUT);

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(40, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode

  // analogRead() takes about 0.1 ms on an AVR.
  // 0.1 ms per sensor * 4 samples per sensor read (default) * 6 sensors
  // * 10 reads per calibrate() call = ~24 ms per calibrate() call.
  // Call calibrate() 400 times to make calibration take about 10 seconds.
    for (uint16_t i = 0; i < 200; i++) {
        qtr.calibrate();
    }
    digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED to indicate we are through with calibration

      // print the calibration minimum values measured when emitters were on
    Serial.begin(9600);
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        Serial.print(qtr.calibrationOn.minimum[i]);
        Serial.print(' ');
    }
    Serial.println();
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        Serial.print(qtr.calibrationOn.maximum[i]);
        Serial.print(' ');
    }
    
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        thresholdValues[i] = floor((float)(qtr.calibrationOn.minimum[i] + qtr.calibrationOn.maximum[i]) / 2);
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

    motorPower(leftMotorSpeed, 0, rightMotorSpeed, 0);
}

void turningCase(char dir) {
    if (dir == 'r') {
        digitalWrite(33, HIGH);
        while(occurenceCount > 3) {
            motorPower(speed, 0, 0, speed);
            sensorConversion();
        }
    }
    if (dir == 'l') {
        digitalWrite(33, HIGH);
        while(occurenceCount > 3) {
            motorPower(0, speed, speed, 0);
            sensorConversion();
        }
    }
    digitalWrite(33, LOW);
}


void loop() {
    sensorConversion();
    
    if (binaryValue == 0b00000001) turningCase('r');
    if (binaryValue == 0b00000011) turningCase('r');
    if (binaryValue == 0b00000111) turningCase('r');
    if (binaryValue == 0b00001111) turningCase('r');
    if (binaryValue == 0b00011111) turningCase('r');
    if (binaryValue == 0b00111111) turningCase('r');

    if (binaryValue == 0b10000000) turningCase('l');
    if (binaryValue == 0b11000000) turningCase('l');
    if (binaryValue == 0b11100000) turningCase('l');
    if (binaryValue == 0b11110000) turningCase('l');
    if (binaryValue == 0b11111000) turningCase('l');
    if (binaryValue == 0b11111100) turningCase('l');

    if (binaryValue == 255 || binaryValue == 0) {
        motorPower(0, 0, 0, 0);
    } else advancedMotorControl();
}
