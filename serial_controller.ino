#include <Adafruit_LSM303DLH_Mag.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

char recvdChar;
boolean recvd = false;
int forward = 3;
int backward = 4;
int left = 5;
int right = 6;
int profile = 2;
Adafruit_LSM303DLH_Mag_Unified mag = Adafruit_LSM303DLH_Mag_Unified(12345);

void setup() {
  /* Set up serial connection to read signal from OpenVibe */
  Serial.begin(115200);
  Serial.println("Device ready");

  /*  Configure digital output for each pin
      Set to input_pullup first to avoid going low immediately
      (and thus avoid unwarranted wheelchair movement) */
  pinMode(forward, INPUT_PULLUP);
  pinMode(forward, OUTPUT);

  pinMode(backward, INPUT_PULLUP);
  pinMode(backward, OUTPUT);

  pinMode(left, INPUT_PULLUP);
  pinMode(left, OUTPUT);

  pinMode(right, INPUT_PULLUP);
  pinMode(right, OUTPUT);

  pinMode(profile, INPUT_PULLUP);
  pinMode(profile, OUTPUT);

  /* Initialise the magnetometer sensor */
  if (!mag.begin()) {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while (1);
  }
}

void loop() {
  /* Read data from BCI */
  recvcommand();

  /* Send to chair */
  sendcommand();
}

float heading() {
  float Pi = 3.14159;
  float h;

  /* If the sensor returns 0.00 (impossible) then resample */
  while (h == 0.00) {
    /* Get a new sensor event */
    sensors_event_t event;
    mag.getEvent(&event);
    /* Convert micro-Tesla reading to degrees 0-360 */
    h = (atan2(event.magnetic.y, event.magnetic.x) * 180) / Pi;
    if (h < 0) {
      h = 360 + h;
    }
  }
  return h;
}

void mv(int pin, int t) {
  /* Move forward or backwards */
  digitalWrite(pin, LOW);
  delay(t);
  digitalWrite(pin, HIGH);
}

void turn(int pin, int deg) {
  /* Turn left or right with magnetometer data */
  float start = heading();
  float current;

  /* Begin chair movement until we've turned $deg degrees */
  digitalWrite(pin, LOW);

  /* Reset diff to 0 each time */
  float diff = 0.0;
  /* Start the 1000 ms timer ensuring turns aren't too short */
  unsigned long previousMillis = millis();
  while (true) {
    /* calculate the degrees that the chair has turned since the last iteration */
    current = heading();
    diff = abs(start - current);
    if (diff > 180) {
      diff = 360 - diff; /* use obtuse angle, not reflex angle */
    }
    /* Break once 1000ms AND degrees have occurred) */
    if ( (diff > deg) && (millis() - previousMillis >= 1000) ) {
      /* Stop chair movement once we're done */
      digitalWrite(pin, HIGH);
      break;
    }
  }
}


void recvcommand() {
  /* Read serial buffer if it has anything */
  if (Serial.available() > 0) {
    recvdChar = Serial.read();
    Serial.println(recvdChar);
    recvd = true;
  }
}

void sendcommand() {
  /* If we have new data to send */
  if (recvd == true) {
    /* Determine target pin from command */
    float c;
    float d;
    float s = heading();
    switch (recvdChar) {
      case 'f':
        mv(forward, 1500);
        break;
      case 'b':
        mv(backward, 1500);
        break;
      case 'l':
        turn(left, 20);
        break;
      case 'r':
        turn(right, 20);
        break;
      case 'p':
        digitalWrite(profile, LOW);
        delay(100);
        digitalWrite(profile, HIGH);
        break;
      case 'h':
        Serial.println(heading());
        break;
    }
    recvd = false;
  }
}