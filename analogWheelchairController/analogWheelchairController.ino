// Define User Types below here or use a .h file
//


// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//
int ledPin = 5;
int pwm = 127;

int charPointer = 0;

char inputBuffer[10] = "";
// The setup() function runs once each time the micro-controller starts
void setup()
{
	// Set up serial connection to read commands from BCI application/.
	Serial.begin(115200);
	Serial.println("BCI wheelchair controller connected");
	analogWrite(ledPin, pwm);
}	

// Add the main program code into the continuous loop() function
void loop()
{
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
		case 'e':
			pwm = atoi(inputBuffer);
			if (pwm > 255 || pwm < 0)
			{
				pwm = 127;
			}
			Serial.println(pwm); 
			analogWrite(ledPin, pwm);
			break;
		}
	}
}
void fadePin()
{
    // fade in from min to max in increments of 5 points:
    for (int fadeValue = 0; fadeValue <= 255; fadeValue += 5) {
        // sets the value (range from 0 to 255):
        analogWrite(ledPin, fadeValue);
        // wait for 30 milliseconds to see the dimming effect
        delay(250);
    }
    delay(3000);
    // fade out from max to min in increments of 5 points:
    for (int fadeValue = 255; fadeValue >= 0; fadeValue -= 5) {
        // sets the value (range from 0 to 255):
        analogWrite(ledPin, fadeValue);
        // wait for 30 milliseconds to see the dimming effect
        delay(250);
    }
    delay(3000);
}
