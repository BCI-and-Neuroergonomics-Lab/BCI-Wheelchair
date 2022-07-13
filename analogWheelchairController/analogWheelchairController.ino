// Define User Types below here or use a .h file
//


// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//
int ledPin = 9;

// The setup() function runs once each time the micro-controller starts
void setup()
{
	// Set up serial connection to read commands from BCI application/.
	Serial.begin(115200);
	Serial.println("BCI wheelchair controller connected");

}

// Add the main program code into the continuous loop() function
void loop()
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
