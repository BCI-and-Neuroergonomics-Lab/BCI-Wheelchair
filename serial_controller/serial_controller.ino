#include <Adafruit_LSM303DLH_Mag.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>


unsigned long lasturnMillis;
unsigned long currentMillis1;

int joyForward = 2;
int joyBackward = 3;
int joyLeft = 4;
int joyProfile = 5;
int profile = 6;

int joySpeed = 3;
int joyDirection = 5;

int joyReference = 6;

Adafruit_LSM303DLH_Mag_Unified mag = Adafruit_LSM303DLH_Mag_Unified(12345);
sensors_event_t event;

void setup() {
	/* Set up serial connection to read signal from OpenVibe */
	Serial.begin(115200);
	Serial.println("Device connected");

	/*  Configure digital output for each pin
		Set to input_pullup first to avoid going low immediately
		(and thus avoid unwarranted wheelchair movement) */
	pinMode(joyForward, INPUT_PULLUP);
	pinMode(joyForward, OUTPUT);
	digitalWrite(joyForward, HIGH);

	pinMode(joyBackward, INPUT_PULLUP);
	pinMode(joyBackward, OUTPUT);
	digitalWrite(joyBackward, HIGH);

	pinMode(joyLeft, INPUT_PULLUP);
	pinMode(joyLeft, OUTPUT);
	digitalWrite(joyLeft, HIGH);

	pinMode(joyProfile, INPUT_PULLUP);
	pinMode(joyProfile, OUTPUT);
	digitalWrite(joyProfile, HIGH);

	pinMode(profile, INPUT_PULLUP);
	pinMode(profile, OUTPUT);


	/* Initialise the magnetometer sensor */
	if (mag.begin()) {
		Serial.println("LSM303 Magnetometer detected and started.");
	}
	else {
		Serial.println("Ooops, no LSM303 Magnetometer detected ... Check your wiring!");
	}
}

void loop() {
	/* Read data from BCI */
	readCommand();

}
int charPointer = 0;
char buffer[80] = "";
void readCommand() {
	/* Read serial buffer if it has anything */
	if (Serial.available())
	{
		char incoming = Serial.read();
		switch (incoming)
		{
		default:
			if (charPointer < 10) {
				// Add the character to the buffer and increment the pointer
				buffer[charPointer++] = incoming;
				buffer[charPointer] = 0; // Don't forget to terminate the string
			}
			break;
		case 'S': // Restarts arduino and resets pins.
	  //Clears string and sets buffer pointer to start
			buffer[0] = 0;
			charPointer = 0;
			setup();
			break;
		case 'C': // Clears string input.
		  //Clears string and sets buffer pointer to start
			buffer[0] = 0;
			charPointer = 0;
			break;
		case 'm': //Wheel chair moves forward for specified seconds
			translateTime(atoi(buffer));
			//Clears string and sets buffer pointer to start
			buffer[0] = 0;
			charPointer = 0;
			break;
		case 'R': //Wheel chair rotates for specified seconds.
			rotateTime(atoi(buffer));
			//Clears string and sets buffer pointer to start
			buffer[0] = 0;
			charPointer = 0;
			break;
		case 'r': //Wheel chair rotates to specified angle
			rotateAngle(atoi(buffer));
			//Clears string and sets buffer pointer to start
			buffer[0] = 0;
			charPointer = 0;
			break;
		case 'p': //Changes profile of wheelchair.
			digitalWrite(profile, LOW);
			delay(100);
			digitalWrite(profile, HIGH);
			//Clears string and sets buffer pointer to start
			buffer[0] = 0;
			charPointer = 0;
			break;
		case 'h': //Gets heading of wheelchair
			Serial.println(getHeading());
			//Clears string and sets buffer pointer to start
			buffer[0] = 0;
			charPointer = 0;
			break;
		}

	}
}

float getHeading() {

	mag.getEvent(&event);

	float tau = 6.283185;

	float h = atan2(event.magnetic.y, event.magnetic.x) * (2 / tau);

	h = 1 + h;

	return h;
}

void translateTime(int time) {

	int pin = joyForward;

	if (t > 5000)
	{
		t = 5000;
	}
	else if (abs(t) <= 500) {
		t = 0;
	}
	else if (t < -5000)
	{
		pin = joyBackward;
		t = 5000;
	}

	Serial.print("Moving the wheel chair for ");
	Serial.print(time);
	Serial.println(" milliseconds.");

	/* Move forward or backwards */
	digitalWrite(pin, LOW);
	delay(time);
	digitalWrite(pin, HIGH);
}

void rotateTime(int time) {
	int pin = joyProfile;

	if (t > 5000)
	{
		t = 5000;
	}
	else if (abs(t) <= 500) {
		t = 0;
	}
	else if (t < -5000)
	{
		pin = joyLeft;
		t = 5000;
	}


	Serial.print("Rotating wheel chair for");
	Serial.print(time) :
		Serial.println(" milliseconds.");

	digitalWrite(pin, LOW);
	delay(time);
	digitalWrite(pin, HIGH);
}

void rotateAngle(float theta) {
	/* Turn left or right with magnetometer data */


	Serial.print("Rotating wheel chair ");

	int pin;

	float current = getHeading();

	target = current + 10;

	error = target - current
		if (error > 1) 
		{

	}


	digitalWrite(left, HIGH);

}
