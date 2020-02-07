#include <QTRSensors.h>
#include <math.h>
#define Kp 10
#define Kd 150
#define rightMaxSpeed 180
#define leftMaxSpeed 180 
#define rightBaseSpeed 90
#define leftBaseSpeed 90
#define NUM_SENSORS 10
#define EMITTER_PIN 53
#define SET_POINT 4
#define A 6
#define B 7
#define C 8
#define D 9

QTRSensors qtr;

unsigned int sensorValues[NUM_SENSORS];
unsigned int thresholdValues[NUM_SENSORS] = {706, 706, 504, 596, 444, 476, 382, 446, 706, 748};
unsigned int historyValues[3];
float position;
uint16_t binaryValue;
uint8_t occurenceCount;
float lastError = 0;
char turn = 'n';
int reverseLine = 1;
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
    if (occurenceCount == 0) position = 0;
    else {
    	position = (float) positionTemp / (float) occurenceCount;
        historyValues[0] = historyValues[1];
        historyValues[1] = historyValues[2];
        historyValues[2] = binaryValue;
    }
    Serial.print(position);
    Serial.print("\t");
}

void motorPower(int L, int M) {
	int a, b, c, d;
	if (L > 0) a = L, b = 0;
	if (L < 0) a = 0, b = -L;
	if (M > 0) c = M, d = 0;
	if (M < 0) c = 0, d = -M;
	analogWrite(A, a); analogWrite(B, b); analogWrite(C, c); analogWrite(D, d);
	Serial.print(L); Serial.print(" "); Serial.print(M); Serial.println(); 
}

void setup() {
    qtr.setTypeRC();
    qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7, A8, A9}, NUM_SENSORS);
    qtr.setEmitterPin(EMITTER_PIN);
  
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(D, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);z
    Serial.begin(9600);
    
    digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode


    // 2.5 ms RC read timeout (default) * 10 reads per calibrate() call
    // = ~25 ms per calibrate() call.
    // Call calibrate() 400 times to make calibration take about 10 seconds.
    for (uint16_t i = 0; i < 200; i++) {
        qtr.calibrate();
    }
    
    digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED to indicate we are through with calibration
    Serial.println("Calibration Completed");
    delay(2000); // wait for 2s to position the bot before entering the main loop

    
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        Serial.print(qtr.calibrationOn.minimum[i]);
        Serial.print(' ');
    }
    Serial.println();

    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        Serial.print(qtr.calibrationOn.maximum[i]);
        Serial.print(' ');
    }
    Serial.println();

    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        thresholdValues[i] = floor((float)(qtr.calibrationOn.maximum[i] + qtr.calibrationOn.minimum[i]) / 2);
    }

    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        Serial.print(thresholdValues[i]);
        Serial.print(" ");
    }

    Serial.println();
    Serial.println();
}

void advancedMotorControl(){
	float error = SET_POINT - position;
    float motorSpeed = Kp * error + Kd * (error - lastError);
    lastError = error;
    int rightMotorSpeed = rightBaseSpeed + (int) motorSpeed;
    int leftMotorSpeed = leftBaseSpeed - (int) motorSpeed;

    motorPower(leftMotorSpeed, rightMotorSpeed);
}

void loop() {
	int flag = 0;
    sensorConversion();
    digitalWrite(12, 0);
    if (binaryValue == 0 && turn != 'n') {
    	if (turn == 'l') {
			while(position < 2.5 || position > 6.5) { 
				sensorConversion();
				motorPower(-50, 50);
			}
    	}
    	if (turn == 'r') {
			while(position < 2.5 || position > 6.5) { 
				sensorConversion();
				motorPower(50, -50);
			}
    	}
    }
    /*else if (binaryValue == 0b1111001111 || binaryValue == 0b1110001111 || binaryValue == 0b1111000111) {
    	motorPower(0, 0);
    	if (historyValues[1] == 0b0000110000 || historyValues[1] == 0b0001110000 || historyValues[1] == 0b0000111000) {
    		reverseLine = 1 ^ reverseLine;
    		sensorConversion();    		
    		advancedMotorControl();
    	}
    }*/
    else if(binaryValue == 0b1111000000 || binaryValue == 0b1111100000 || binaryValue == 0b1111110000 || binaryValue == 0b1111111000 || binaryValue == 0b1111111100   ||   binaryValue == 0b0011110000 || binaryValue == 0b0111110000 || binaryValue == 0b0011111000 || binaryValue == 0b0111111000) {
       	digitalWrite(12, 1);
       	motorPower(50, 50);
       	if (binaryValue == 0b0011110000 || binaryValue == 0b0111110000 || binaryValue == 0b0011111000 || binaryValue == 0b0111111000) motorPower(50, 50);
       	turn = 'l';	
    }
    else if(binaryValue == 0b0000001111 || binaryValue == 0b0000011111 || binaryValue == 0b0000111111 || binaryValue == 0b0001111111 || binaryValue == 0b0011111111   ||   binaryValue == 0b0000111100 || binaryValue == 0b0000111110 || binaryValue == 0b0001111100 || binaryValue == 0b0001111110) {
       	digitalWrite(12, 1);
       	motorPower(50, 50);
       	if (binaryValue == 0b0000111100 || binaryValue == 0b0000111110 || binaryValue == 0b0001111100 || binaryValue == 0b0001111110) motorPower(50, 50);
       	turn = 'r';    	
    }
    
    else { 
    	advancedMotorControl();
        turn = 'n';
    }
}