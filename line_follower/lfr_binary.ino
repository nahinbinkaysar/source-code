#include <QTRSensors.h>
#include <math.h>

using namespace std;

#define Kp 17 // experiment to determine this, start by something small that just makes your bot follow the line at a slow speed
#define Kd 220 // experiment to determine this, slowly increase the speeds and adjust this value. ( Note: Kp < Kd) 
#define rightMaxSpeed 250 // max speed of the robot
#define leftMaxSpeed 250 // max speed of the robot
#define rightBaseSpeed 100 // this is the speed at which the motors should spin when the robot is perfectly on the line
#define leftBaseSpeed 100 // this is the speed at which the motors should spin when the robot is perfectly on the line
#define NUM_SENSORS 10 // number of sensors used
#define TIMEOUT 2500 // waits for 2500 us for sensor outputs to go low
#define EMITTER_PIN 23 // emitter is controlled by digital pin 23
#define SET_POINT 4.5 // this is my own algorithm to set the SET_POINT
// notice for 10 sensor you get an array from 0
// to 9; now the setpoint is the middle point of
// the sensors; thus this is 4.5
#define CAL_LED 13

//left motor: black-white
#define A 6 //forward-orange
#define B 7 //back-red

//right motor: yellow-green
#define C 8 //forward-yellow
#define D 9 //back-green

QTRSensorsRC qtrrc((unsigned char[]) {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9}, NUM_SENSORS, TIMEOUT, EMITTER_PIN); // sensor connected through analog pins A0 - A5 i.e. digital pins 14-19

unsigned int sensorValues[NUM_SENSORS];
unsigned int thresholdValues[NUM_SENSORS];
float position;

uint16_t sensorConversion() {
    bool digitalValue; // stores the value of the sensor as 0 or 1 comparing with the value from thresholValues
    uint16_t binaryValue = 0; // resets the binary representaion of postion everytime
	uint8_t occurenceCount = 0; // this is to store how many 1's there are for a line
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
    }

    position = (float) positionTemp / (float) occurenceCount; //finally calculating average to determine where exactly the bot is
    return binaryValue;
}

void setup() {
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(D, OUTPUT);
    pinMode(CAL_LED, OUTPUT);

    Serial.begin(9600);

    analogWrite(CAL_LED, 255);
    for (uint8_t i = 0; i < 200; i++) // calibrate for sometime by sliding the sensors across the line, or you may use auto-calibration instead
    {
        qtrrc.calibrate();
        delay(20);
    }
    
    analogWrite(CAL_LED, 0);

    delay(2000); // wait for 2s to position the bot before entering the main loop

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
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        thresholdValues[i] = floor((float)(qtrrc.calibratedMaximumOn[i] + qtrrc.calibratedMinimumOn[i]) / 2);
    }

    Serial.println();
    Serial.println();
}

void motorPower(int lFront, int lBack, int rFront, int rBack) {
	analogWrite(A, lFront);
	analogWrite(B, lBack);
	analogWrite(C, rFront);
	analogWrite(D, rBack);

	// Serial.print(lFront);
	// Serial.print(' ');
 //    Serial.print(lBack);
 //    Serial.print(' ');
 //    Serial.print(rFront);
 //    Serial.print(' ');
 //    Serial.print(rBack);
 //    Serial.print(' ');
 //    Serial.println();
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

void loop() {
    qtrrc.readLine(sensorValues);
    uint16_t binaryValue = sensorConversion();

    int digit;
    char binaryValueString[] = "0000000000";
	int i = 0;
    while(binaryValue != 0) {
        digit = binaryValue % 2;
        binaryValue /= 2;
        binaryValueString[i] = digit + '0';
        i++;
    }
    for(i = 9; i >= 0; i--) {
            Serial.print(binaryValueString[i]);
    }
    
    Serial.print('\t');
    // Serial.print(position);
    // Serial.print('\t');

    for (int i = 0; i < NUM_SENSORS; ++i)
    {
    	Serial.print(sensorValues[i]);
    	Serial.print(' ');
    }
    Serial.println();

	if (binaryValue == 0b0000111111 || binaryValue == 0b0000011111) {
    	motorPower(0, 150, 150, 0);
    }

	else if (binaryValue == 0b1111110000 || binaryValue == 0b1111100000) {
    	motorPower(150, 0, 0, 150);
    }
    // this is to check whether a 90 degree rotaion is needed or not
    // default 00 00 11 00 00
    // if (binaryValue == 0b0000111111 || binaryValue == 0b0000011111) {
    //     while(sensorConversion() != 0) {
    //         forward();
    //     }
    //     uint8_t temp = sensorConversion();
    //     while(occurenceCount > 3) {
    //         sharpRight();
    //     }
    // }

    // else if (binaryValue == 0b1111110000 || binaryValue == 0b1111110000) {
    //     while(sensorConversion() != 0) {
    //         forward();
    //     }
    //     uint8_t temp = sensorConversion();
    //     while(occurenceCount > 3) {
    //         sharpLeft();
    //     }
    // }
    // // run the generic motor control scope
    else advancedMotorControl();
    
}
