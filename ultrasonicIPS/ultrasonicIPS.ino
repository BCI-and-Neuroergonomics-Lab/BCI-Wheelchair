long t, t0;

const uint16_t sampleLength = 512;

int16_t sampleBuffer[sampleLength];

int sampleCount = 0;


void setup()
{
	Serial.begin(115200);
	Serial.println("Raw:,On:,Min:,Max:");
	pinMode(13, OUTPUT);

	setupClock();

	//setupADC();
	
	t0 = micros();
}

void setupClock()
{
	//set timer1 interrupt at 1Hz
	TCCR1A = 0;// set entire TCCR1A register to 0
	TCCR1B = 0;// same for TCCR1B
	TCNT1 = 0;//initialize counter value to 0
	// set compare match register for 1hz increments
	OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
	// turn on CTC mode
	TCCR1B |= (1 << WGM12);
	// Set CS12 and CS10 bits for 1024 prescaler
	TCCR1B |= (5 << CS10);
	// enable timer compare interrupt
	TIMSK1 |= (1 << ICIE1);
}

boolean toggle0 = 0;
uint32_t count;
ISR(TIMER1_CAPT_vect) 
{//timer0 interrupt 2kHz toggles pin 8
//generates pulse wave of frequency 2kHz/2 = 1kHz (takes two cycles for full wave- toggle high then toggle low)
	count++;
}

void setupADC()
{
	DIDR0 = 0x01;
	ADCSRA = 0;
	ADCSRB = 0;
	ADMUX |= (0 & 0x07);
	ADMUX |= (0 << REFS0);
	//ADMUX |= (1 << ADLAR);


	//ADCSRA |= (1 << ADPS2) | (1 << ADPS0);
	ADCSRA |= (1 << ADPS2);
	//ADCSRA |= (1 << ADPS1) | (1 << ADPS0);    

	ADCSRA |= (1 << ADATE);
	ADCSRA |= (1 << ADIE);
	ADCSRA |= (1 << ADEN);
	ADCSRA |= (1 << ADSC);
}

ISR(ADC_vect)
{
	sampleBuffer[sampleCount] = fastAbs((int16_t)ADC);
	sampleCount++;
}

uint16_t inline fastAbs(int16_t sample)
{
	sample += -783;
	uint16_t mask = sample >> 15;
	sample ^= mask;
	sample += mask & 1;
	return sample;
}

void loop()
{
	Serial.println(count);
	if (sampleCount >= sampleLength)
	{
		sampleCount = 0;

		t = micros() - t0;  // calculate elapsed time

		noInterrupts();//ADCSRA &= (0 << ADIE);

		printBuffer();

		t0 = micros();

		interrupts();//ADCSRA |= (1 << ADIE);
	}
}

void printBuffer()
{
	for (int i = 0; i < sampleLength; i++)
	{
		Serial.print(sampleBuffer[i]);
		Serial.print(",");
		if (sampleBuffer[i] >= 10) 
		{
			Serial.print(1000);
		}
		else 
		{
			Serial.print(0);
		}
		Serial.println(",0,1024");
		//Serial.println(((float)sampleBuffer[i] + 0.5) * 5/1024 * 1000);

	}
}
void printFrequency()
{

	float Fs = (float)sampleLength / t;

	Serial.print("Sampling Period: ");
	Serial.print(1 / Fs * 100000);
	Serial.println("uS");

	Serial.print("Sampling frequency: ");
	Serial.print(Fs * 1000);
	Serial.println(" KHz");

	delayMicroseconds(100);
}