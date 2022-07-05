// Define User Types below here or use a .h file
//


// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//

int pinProfile = 3;
int pinReference = 9;
int pinSpeed = 11;
int pinDirection = 10;


// The setup() function runs once each time the micro-controller starts
void setup()
{
    //Initialize Serial Monitor
    Serial.begin(115200);
    Serial.println("Device connected");
    //Set output pins
    analogWrite(pinReference, 127);
    analogWrite(pinSpeed, 127);
    analogWrite(pinDirection, 127);
    //Set profile pin
    analogWrite(pinProfile, 0);
}

// Add the main program code into the continuous loop() function
void loop()
{


}
