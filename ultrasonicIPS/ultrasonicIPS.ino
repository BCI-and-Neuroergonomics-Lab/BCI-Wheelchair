int sampleCount = 0;
long t, t0;
const int sampleLength = 500;

void setup()
{
	//Look on my works, ye mighty, and despair.
	Serial.begin(115200);

	DIDR0 = 0x01;
	ADCSRA = 0;
	ADCSRB = 0;
	ADMUX |= (0 & 0x07);
	ADMUX |= (1 << REFS0);
	ADMUX |= (1 << ADLAR);


	//ADCSRA |= (1 << ADPS2) | (1 << ADPS0);    
	ADCSRA |= (1 << ADPS2);
	//ADCSRA |= (1 << ADPS1) | (1 << ADPS0);    

	ADCSRA |= (1 << ADATE);
	ADCSRA |= (1 << ADIE);
	ADCSRA |= (1 << ADEN);
	ADCSRA |= (1 << ADSC);

	t0 = micros();
}
int sampleBuffer[sampleLength];
ISR(ADC_vect)
{
	sampleBuffer[sampleCount] = ADCH;//(ADCH << 8) | ADCL;
	//Serial.println(sampleBuffer[numSamples]);
	sampleCount++;
}

void loop()
{
	if (sampleCount >= sampleLength)
	{

		ADCSRA &= (0 << ADIE);

		t = micros() - t0;  // calculate elapsed time

	   // Serial.print("Sampling frequency: ");
	   // Serial.print((float)sampleCount * 1000/t);
		//Serial.println(" KHz");


		for (int i = 0; i < sampleLength; i++)
		{
			Serial.println(sampleBuffer[i] * 0.0196 * 50, 6);
			// Serial.print(",");
		}
		sampleCount = 0;
		ADCSRA |= (1 << ADIE);
		
	}

}