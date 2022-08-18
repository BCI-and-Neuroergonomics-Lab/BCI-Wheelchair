#undef HID_ENABLED

#include <movingAvg.h> 

#define tickFrequency (105000)

uint32_t timerCount;

uint32_t previousCount;

const int bufferCount = 4;

const int sampleLength = 256;

int16_t sampleBuffer[bufferCount][sampleLength];   // 4 buffers of 256 readings

volatile int currentBufferIndex, previousBufferIndex;

movingAvg sampleFilter(64);

void setup()
{
	Serial.begin(115200);

	pinMode(LED_BUILTIN, OUTPUT);
	
	Serial.println("Communicating");

	sampleFilter.begin();

	setupADC();

	setupTC();

}

void setupADC()
{
	pmc_enable_periph_clk(ID_ADC);

	//adc_init(ADC, SystemCoreClock, ADC_FREQ_MAX, ADC_STARTUP_FAST);

	PMC->PMC_PCER1 |= PMC_PCER1_PID37;                     // ADC power on
	ADC->ADC_CR = ADC_CR_SWRST;                            // Reset ADC
	ADC->ADC_MR |= ADC_MR_TRGEN_EN | ADC_MR_PRESCAL(1) | ADC_MR_TRGSEL_ADC_TRIG3 | ADC_MR_LOWRES_BITS_12;

	//ADC->ADC_MR |= ADC_MR_FREERUN;

	NVIC_EnableIRQ(ADC_IRQn);

	ADC->ADC_CHER = 0x80;
	//ADC->ADC_CHDR = ~(0ul);

	ADC->ADC_IER = ADC_IDR_ENDRX;
	ADC->ADC_IDR = ~ADC_IDR_ENDRX;

	ADC->ADC_RPR = (uint32_t)sampleBuffer[0];   // DMA buffer
	ADC->ADC_RCR = sampleLength;
	ADC->ADC_RNPR = (uint32_t)sampleBuffer[1]; // next DMA buffer
	ADC->ADC_RNCR = sampleLength;

	currentBufferIndex = previousBufferIndex = 1;

	ADC->ADC_PTCR = 1;
	ADC->ADC_CR = 2;
}

void setupTC()
{
	PMC->PMC_PCER0 |= PMC_PCER0_PID29;                       // TC2 power ON : Timer Counter 0 channel 2 IS TC2

	TC0->TC_CHANNEL[2].TC_CMR |= TC_CMR_TCCLKS_TIMER_CLOCK2 | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET;  // MCK/8, clk on rising edge    

	TC0->TC_CHANNEL[2].TC_RC = (F_CPU * 1 / 8) / tickFrequency; //Frequency = (Mck/8)/TC_RC  Hz = 10 Hz

	TC0->TC_CHANNEL[2].TC_RA = 50;  // Any Duty cycle in between 1 and (TC_RC - 1)

	TC0->TC_CHANNEL[2].TC_IER |= TC_IER_CPCS | TC_IER_CPAS;

	NVIC_EnableIRQ(TC2_IRQn);

	TC0->TC_CHANNEL[2].TC_CCR = TC_CCR_SWTRG | TC_CCR_CLKEN; // Software trigger TC2 counter and enable
}

uint16_t timerDifference;
void loop()
{
	if (previousBufferIndex == currentBufferIndex) // wait for buffer to be full
		return;
	previousBufferIndex = (previousBufferIndex + 1) & (bufferCount - 1);
	timerDifference = timerCount - previousCount;
	previousCount = timerCount;
	//measureFrequency(timerDifference);
	printBuffer();
	//NVIC_DisableIRQ(TC2_IRQn);
	//delay(500);
	//NVIC_EnableIRQ(TC2_IRQn);
}

void inline printBuffer() 
{
	for (int i = 0; i < sampleLength; i++)
	{
		//sampleBuffer[previousBufferIndex][i] = fastAbs(sampleBuffer[previousBufferIndex][i]);

		Serial.println(sampleBuffer[previousBufferIndex][i]);

		//Serial.print(",");

		//sampleBuffer[previousBufferIndex][i] = sampleFilter.reading(sampleBuffer[previousBufferIndex][i]);

		//Serial.print(sampleBuffer[previousBufferIndex][i]);

		//Serial.println(",1000,2000");
	}
}

void measureFrequency(uint16_t timerDifference)
{
	float timeDifference = 100000 * timerDifference / (tickFrequency);
	float samplePeriod = timeDifference / sampleLength;
	float sampleFrequency = 100000 / samplePeriod;
	//Serial.println(timerDifference);
	//Serial.println(timeDifference);
	Serial.println(samplePeriod);
	Serial.println(sampleFrequency);
}

uint16_t inline fastAbs(int16_t sample)
{
	sample -= 2072;
	uint16_t mask = sample >> 15;
	sample ^= mask;
	sample += mask & 1;
	return sample;
}


void ADC_Handler()
{
	// move DMA pointers to next buffer
	if (ADC->ADC_ISR & ADC_ISR_ENDRX)
	{
		currentBufferIndex = (currentBufferIndex + 1) & (bufferCount - 1);
		ADC->ADC_RNPR = (uint32_t)sampleBuffer[currentBufferIndex];
		ADC->ADC_RNCR = sampleLength;
	}

}

void TC2_Handler() {

	static uint32_t lightCount;

	uint32_t status = TC0->TC_CHANNEL[2].TC_SR;

	timerCount++;

	if (lightCount++ >= tickFrequency) {
		lightCount = 0;
		PIOB->PIO_ODSR ^= PIO_ODSR_P27;

	}
}

