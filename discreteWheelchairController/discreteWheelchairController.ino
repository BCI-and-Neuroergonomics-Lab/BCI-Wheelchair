#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303DLH_Mag.h>

#define pinForward 2
#define pinBackward 3
#define pinLeft 4
#define pinRight 5
#define profile 6

int charPointer = 0;

char inputBuffer[10] = "";

Adafruit_LSM303DLH_Mag_Unified mag = Adafruit_LSM303DLH_Mag_Unified(12345);

sensors_event_t event;

float currentHeading;

float tau = 6.283185;

void setup() {

	// Set up serial connection to read commands from BCI application/.
	Serial.begin(9600);

	Serial.println("BCI wheelchair controller connected");

	// Configure digital output for each pin Set to input_pullup first to avoid going low immediately (and thus avoid unwarranted wheelchair movement)
	resetPins();

	if (mag.begin()) 
	{
		//Enables auto gain of magnetometer
		mag.enableAutoRange(true);
		Serial.println("LSM303 Magnetometer initialization completed.");
	}
	else 
	{
		Serial.println("LSM303 Magetometer initilization failed.");
	}
}

void loop() {
	//Recieves serial commands from BCI application.
	readCommand();
}

void readCommand() {
	// Reads serial commands if active
	if (Serial.available())
	{
		char input = Serial.read();
		switch (input)
		{
		default:
			if (charPointer < 10)
			{
				// Add the character to the buffer and increment the pointer
				inputBuffer[charPointer++] = input;
				inputBuffer[charPointer] = 0;
				// Don't forget to terminate the string
			}
			else {
				inputBuffer[0] = 0;
				charPointer = 0;
			}
			break;
		case 'S':
			// Resets pins
			resetPins();
			inputBuffer[0] = 0;
			//Clears string and sets buffer pointer to start
			charPointer = 0;
			break;
		case 'p':
			//Changes profile of wheelchair.
			Serial.println("Switching IOM profile");
			digitalWrite(profile, LOW);
			delay(200);
			digitalWrite(profile, HIGH);
			//Clears string and sets buffer pointer to start
			inputBuffer[0] = 0;
			charPointer = 0;
			break;
		case 'C':
			// Clears string input.
			//Clears string and sets buffer pointer to start
			inputBuffer[0] = 0;
			charPointer = 0;
			break;
		case 'h':
			Serial.print("Current heading: ");
			Serial.println(getHeading());
			//Clears string and sets buffer pointer to start
			inputBuffer[0] = 0;
			charPointer = 0;
			break;
		case 'm':
			//Wheel chair moves forward for specified seconds
			translateTime(atoi(inputBuffer));
			//Clears string and sets buffer pointer to start
			inputBuffer[0] = 0;
			charPointer = 0;
			break;
		case 'r':
			//Wheel chair rotates for specified seconds.
			rotateTime(atoi(inputBuffer));
			//Clears string and sets buffer pointer to start
			inputBuffer[0] = 0;
			charPointer = 0;
			break;
		case 'R':
			//Rotates wheelchair by specified angle
			rotateAngle(atoi(inputBuffer));
			//Clears string and sets buffer pointer to start
			inputBuffer[0] = 0;
			charPointer = 0;
			break;
		}
	}
}
void resetPins() {
	// Configure digital output for each pin Set to input_pullup first to avoid going low immediately (and thus avoid unwarranted wheelchair movement)
	pinMode(pinForward, INPUT_PULLUP);
	pinMode(pinForward, OUTPUT);
	digitalWrite(pinForward, HIGH);

	pinMode(pinBackward, INPUT_PULLUP);
	pinMode(pinBackward, OUTPUT);
	digitalWrite(pinBackward, HIGH);

	pinMode(pinLeft, INPUT_PULLUP);
	pinMode(pinLeft, OUTPUT);
	digitalWrite(pinLeft, HIGH);

	pinMode(pinRight, INPUT_PULLUP);
	pinMode(pinRight, OUTPUT);
	digitalWrite(pinRight, HIGH);

	pinMode(profile, INPUT_PULLUP);
	pinMode(profile, OUTPUT);
	digitalWrite(profile, HIGH);

}

float getHeading() 
{
	mag.getEvent(&event);

	// Calculate the angle of the vector y,x
	float heading = atan2(event.magnetic.y, event.magnetic.x) * (360 / tau);

	//constrainAngle(heading);

	return heading;
} 

void translateTime(int time) {

	int pin = pinForward;

	if (time > 5000)
	{
		time = 5000;
	}
	else if (abs(time) < 500) 
	{
		time = 0;
	}
	else if (time <= -500) 
	{
		pin = pinBackward;
		time = abs(time);
	}
	else if (time < -5000)
	{
		pin = pinBackward;
		time = 5000;
	}

	Serial.print("Moving the wheelchair for: ");
	Serial.print(time);
	Serial.println(" milliseconds.");


	digitalWrite(pin, LOW);
	delay(time);
	digitalWrite(pin, HIGH);
}

void rotateTime(int time) {
	int pin = pinRight;

	if (time > 5000)
	{
		time = 5000;
	}
	else if (abs(time) < 500) 
	{
		time = 0;
	}
	else if (time <= -500) 
	{
		pin = pinLeft;
		time = abs(time);
	}
	else if (time < -5000)
	{
		pin = pinRight;
		time = 5000;
	}

	Serial.print("Rotating wheelchair for: ");
	Serial.print(time);
	Serial.println(" milliseconds.");

	digitalWrite(pin, LOW);
	delay(time);
	digitalWrite(pin, HIGH);
}


void rotateAngle(float dTheta) {
	int pin;
	if (dTheta >= 0)
	{
		pin = pinRight;
	}
	else 
	{
		pin = pinLeft;
	}

	float initial = getHeading();

	Serial.println(initial);

	float target = initial + dTheta;

	if (target >= 360)
		target -= 360;
	
	Serial.println(target);

	while (1) 
	{
		currentHeading = getHeading();
		digitalWrite(pin, LOW);
		Serial.print(currentHeading); Serial.print(" ");
		if (currentHeading > (target - 1) && currentHeading < (target + 1)) 
		{
			Serial.println("Rotation Completed");
			Serial.println(currentHeading);
			break;
		}
	}

	digitalWrite(pin, HIGH);
}
