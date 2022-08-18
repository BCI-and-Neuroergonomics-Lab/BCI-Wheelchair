


#include <movingAvg.h>

const int sampleLength = 512;

int16_t sampleBuffer[sampleLength];


void setup() {

	Serial.begin(115200);

	Serial.println("Raw_Reading:,Raw_mV:,Max:,Min:");

	analogReadResolution(12);

	//ADC->ADC_MR |= 0x80; // these lines set free running mode on adc 7 (pin A0)
	ADC->ADC_CR = 2;
	ADC->ADC_CHER = 0x80;
	interrupts();
}
int sampleCount = 0;
void loop() {

	//long t = micros();

	noInterrupts();
	if (sampleCount > sampleLength)
	{
		//t = micros() - t;


		for (int sampleCount = 0; sampleCount < sampleLength; sampleCount++)
		{

			Serial.print(sampleBuffer[sampleCount]);
			//Serial.print(",");
			//Serial.print(sampleBuffer[sampleCount] * 3.3 / 4096 * 1000);
			Serial.println(",0,450");
		}
	}
	interrupts();
	/*
	float readPeriod = (float)t / (float)sampleLength;
	float readFrequency = 100000 / readPeriod;
	Serial.println(t);
	Serial.println(sampleLength);
	Serial.println(readPeriod);
	Serial.println(readFrequency);
	*/

}
ISR(ADC_vect)
{
	sampleBuffer[sampleCount] = (ADC->ADC_CDR[7] - 2066);
	uint16_t mask = sampleBuffer[sampleCount] >> 15;
	sampleBuffer[sampleCount] ^= mask;
	sampleBuffer[sampleCount] += mask & 1;
	sampleCount++;
}