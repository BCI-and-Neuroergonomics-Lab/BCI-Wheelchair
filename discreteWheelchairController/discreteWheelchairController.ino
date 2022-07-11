#include <Adafruit_LSM303DLH_Mag.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

unsigned long lasturnMillis;
unsigned long currentMillis1;

int joyForward = 2;
int joyBackward = 3;
int joyLeft = 4;
int joyRight = 5;
int profile = 6;

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

    pinMode(joyRight, INPUT_PULLUP);
    pinMode(joyRight, OUTPUT);
    digitalWrite(joyRight, HIGH);

    pinMode(profile, INPUT_PULLUP);
    pinMode(profile, OUTPUT);


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
        case 'r': //Wheel chair rotates for specified seconds.
            rotateTime(atoi(buffer));
            //Clears string and sets buffer pointer to start
            buffer[0] = 0;
            charPointer = 0;
            break;
        case 'p': //Changes profile of wheelchair.
            digitalWrite(profile, LOW);
            delay(100);
            digitalWrite(profile, HIGH);
            Serial.println("Switching profiles.");
            //Clears string and sets buffer pointer to start
            buffer[0] = 0;
            charPointer = 0;
            break;
        }

    }
}
void translateTime(int time) {

    int pin = joyForward;

    if (time > 5000)
    {
        time = 5000;
    }
    else if (abs(time) < 500) {
        time = 0;
    }
    else if (time <= -500) {
        pin = joyBackward;
        time = abs(time);
    }
    else if (time < -5000)
    {
        pin = joyBackward;
        time = 5000;
    }

    Serial.print("Moving the wheel chair for: ");
    Serial.print(time);
    Serial.println(" milliseconds.");

    /* Move forward or backwards */
    digitalWrite(pin, LOW);
    delay(time);
    digitalWrite(pin, HIGH);
}

void rotateTime(int time) {
    int pin = joyRight;

    if (time > 5000)
    {
        time = 5000;
    }
    else if (abs(time) < 500) {
        time = 0;
    }
    else if (time <= -500) {
        pin = joyLeft;
        time = abs(time);
    }
    else if (time < -5000)
    {
        pin = joyRight;
        time = 5000;
    }

    Serial.print("Rotating wheel chair for: ");
    Serial.print(time);
    Serial.println(" milliseconds.");

    digitalWrite(pin, LOW);
    delay(time);
    digitalWrite(pin, HIGH);
}
