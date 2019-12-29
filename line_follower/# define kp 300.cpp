# define kp 300
# define kd 350
# define ki 0
# define motorL 10
# define motorR 9
# define motorL1 6
# define motorL2 8
# define motorR1 12
# define motorR2 11
int baseSpeed = 80;
int maxSpeed =100 ;
int sensorValue = 0;
int setPoint = 5;
int error = 0;
int cumError = 0;
int last_error = 0;
unsigned int currentValue = 0;
float correction = 0;
int motorResponse = 0;
unsigned int LMspeed = 0;
unsigned int RMspeed = 0;
int weightedValue = 0;
int prevWeightedValue = 0;
int compareValue[6] = {600, 600, 600, 600, 600, 600};


void forward() {
	analogWrite(motorL, baseSpeed);
	analogWrite(motorR, baseSpeed);
	digitalWrite(motorL1, HIGH);
	digitalWrite(motorL2, LOW);
	digitalWrite(motorR1, HIGH);
	digitalWrite(motorR2, LOW);
}

void backward() {
	analogWrite(motorL, baseSpeed);
	analogWrite(motorR, baseSpeed);
	digitalWrite(motorL1, LOW);
	digitalWrite(motorL2, HIGH);
	digitalWrite(motorR1, LOW);
	digitalWrite(motorR2, HIGH);
}

void right() {
	analogWrite(motorL, baseSpeed);
	analogWrite(motorR, baseSpeed);
	digitalWrite(motorL1, HIGH);
	digitalWrite(motorL2, LOW);
	digitalWrite(motorR1, LOW);
	digitalWrite(motorR2, LOW);
}

void left() {
	analogWrite(motorL, baseSpeed);
	analogWrite(motorR, baseSpeed);
	digitalWrite(motorL1, LOW);
	digitalWrite(motorL2, LOW);
	digitalWrite(motorR1, HIGH);
	digitalWrite(motorR2, LOW);
}

void halt() {
	analogWrite(motorL, baseSpeed);
	analogWrite(motorR, baseSpeed);
	digitalWrite(motorL1, LOW);
	digitalWrite(motorL2, LOW);
	digitalWrite(motorR1, LOW);
	digitalWrite(motorR2, LOW);
}

void right_sharp()
{
	analogWrite(motorL, maxSpeed);
	analogWrite(motorR, maxSpeed);
	digitalWrite(motorL1, HIGH);
	digitalWrite(motorL2, LOW);
	digitalWrite(motorR1, LOW);
	digitalWrite(motorR2, HIGH);
}

void left_sharp(){
	analogWrite(motorL, maxSpeed);
	analogWrite(motorR, maxSpeed);
	digitalWrite(motorL1, LOW);
	digitalWrite(motorL2, HIGH);
	digitalWrite(motorR1, HIGH);
	digitalWrite(motorR2, LOW);
}
void sensorConversion(void)
{
	unsigned int sensorNumber = 0;
	int digitalValue = 0;
	int weightCount = 0;
	sensorValue = 0;
	weightedValue = 0;
	for (sensorNumber = 0; sensorNumber < 6; sensorNumber++)
	{
		int temp = analogRead(sensorNumber);
		Serial.print(temp);
		Serial.print("\t");
		
		if (temp > compareValue[sensorNumber]) digitalValue = 0;
		else
		{
			digitalValue = 1;
			weightedValue += (sensorNumber * 2);
			weightCount++;
		}
		sensorValue |= (digitalValue << sensorNumber);
	}
	Serial.print(sensorValue, BIN);
	Serial.print("\t");
	
	weightedValue = weightedValue / weightCount;
	
	if (sensorValue == 0b000000)
	{
		weightedValue = prevWeightedValue;
	}
	prevWeightedValue = weightedValue;
//  Serial.print(weightedValue);
//  Serial.print("\t");
}
void PID(void) {
	error = weightedValue - setPoint;
	cumError += error;
	correction = kp * error + ki * cumError + kd * (error - last_error);
	last_error = error;
	motorResponse = (int) correction;

//  Serial.print(motorResponse);
	Serial.print("\n");


	if (motorResponse >= 0) {
//    LMspeed = baseSpeed + motorResponse;
//    if (LMspeed > maxSpeed) LMspeed = maxSpeed;
//    else if (LMspeed < 0) LMspeed = 0;
//    RMspeed = baseSpeed - motorResponse;
//    if (RMspeed > maxSpeed) RMspeed = maxSpeed;
//    else if (RMspeed < 0) RMspeed = 0;
		LMspeed = baseSpeed + motorResponse;
		if ((baseSpeed + motorResponse) > maxSpeed) LMspeed = maxSpeed;
		else if ((baseSpeed + motorResponse < 0)) LMspeed = 0;
		RMspeed = baseSpeed - motorResponse;
		if ((baseSpeed - motorResponse) > maxSpeed) RMspeed = maxSpeed;
		else if ((baseSpeed - motorResponse < 0)) RMspeed = 0;
	}
	else if (motorResponse < 0) {
		LMspeed = baseSpeed + motorResponse;
		if ((baseSpeed + motorResponse) > maxSpeed) LMspeed = maxSpeed;
		else if ((baseSpeed + motorResponse < 0)) LMspeed = 0;
		RMspeed = baseSpeed - motorResponse;
		if ((baseSpeed - motorResponse) > maxSpeed) RMspeed = maxSpeed;
		else if ((baseSpeed - motorResponse < 0)) RMspeed = 0;
	}
}
void setup() {
  // put your setup code here, to run once:
	Serial.begin(9600);
  //forward ();
	pinMode(motorL1,OUTPUT);
	pinMode(motorL2,OUTPUT);
	pinMode(motorR1,OUTPUT);
	pinMode(motorR2,OUTPUT);

	digitalWrite(motorL1,HIGH);
	digitalWrite(motorL2,LOW);
	digitalWrite(motorR1,HIGH);
	digitalWrite(motorR2,LOW);
	
	

}

void loop() {
//  right_sharp();
//  forward();
////  delay(5000);
////  halt();
////  delay(500);
////  backward();
////  delay(3000);
////  right();
////  delay(2000);
////  left();
////  delay(2000);
	
	sensorConversion();
	if(sensorValue==0b011111||sensorValue==0b001111||sensorValue==0b000111){
		right_sharp();
		while(sensorValue!=0b001100){
			sensorConversion();
		}
	}
	if(sensorValue==0b111110||sensorValue==0b111100||sensorValue==0b111000){
		left_sharp();
		while(sensorValue!=0b001100){
			sensorConversion();
		}
	}
	
	
	PID();
////  Serial.print(LMspeed);
////  Serial.print("\t");
////  Serial.print(RMspeed);
////  Serial.print("\t");
	analogWrite(motorL,LMspeed);
	analogWrite(motorR,RMspeed);
}