#include "main.h"
#include "arm_math.h"
#include "math.h"


#include "platf_led.h"



// http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/DATASHEET/DM00037051.pdf
// http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/REFERENCE_MANUAL/DM00031020.pdf

static __IO uint32_t TimingDelay;
static __IO uint32_t tick;
void Delay(__IO uint32_t nTime)
{
	TimingDelay = nTime * 10;

	while (TimingDelay != 0) ;
}

void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00) {
		TimingDelay--;
	}
	tick++;
}

GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;

static void initTIM2(void)
{
	GPIO_InitTypeDef GPIO_InitStructureTimer;


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE) ;	 // GPIOB gets system clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE)  ;	 // TIM2 gets system clock
	GPIO_InitStructureTimer.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructureTimer.GPIO_Mode = GPIO_Mode_AF;	// alternate function
	GPIO_InitStructureTimer.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructureTimer.GPIO_OType = GPIO_OType_PP;	// set output type to pushpull
	GPIO_InitStructureTimer.GPIO_PuPd = GPIO_PuPd_UP;	// ?
	GPIO_Init(GPIOB, &GPIO_InitStructureTimer);

	// TIM2 is a 32 bit timer
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);	// alternate function TIM2 gets selected

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	TIM_TimeBaseStructure.TIM_Period = 32000;                   // TIM2 upper limit, reset after overflow
	TIM_TimeBaseStructure.TIM_Prescaler = 0;                    // value between 0 and 65535, will be set to
	                                                            // 52 later but we need this step. timerclock=systemclock/(prescaler+1)
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                // ?
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // TIM counts upwards -_-
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);             // activate all

	TIM_PrescalerConfig(TIM2, 52, TIM_PSCReloadMode_Immediate);   // roughly 50Hz or 20ms

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             // outputcomparemode set to PWM mode 1,
								      // output is high as long as counter<pulse
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // output gets enabled
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     // set pulse to high polarity (invert by setting High to Low)
	TIM_OCInitStructure.TIM_Pulse = 1000;                         // duty cycle=pulse/period
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);                      // activate all
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);             // ?

	TIM_ARRPreloadConfig(TIM2, ENABLE);	// ?
	TIM_Cmd(TIM2, ENABLE);	// start timer
	return;

}

void main(void)
{
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);

	/* SysTick end of count event each 0.1ms */
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 10000);

	initLEDs();
	initTIM2();

	uint32_t winkel = 0;
	int led = 0;

	for(;;){
		TIM_SetCompare2(TIM2, sin(winkel / 1000.0) * 800 + 2400);
		Delay(2);
		winkel++;
		if (winkel >= 3.14 * 2 * 1000) {
			winkel = 0;
		} else if (winkel == 1000) {
			setLed(LED_WEST_PIN, 1);
		} else if (winkel == 2000) {
			setLed(LED_NORTH_PIN, 1);
		} else if (winkel == 3000) {
			setLed(LED_EAST_PIN, 1);
		} else if (winkel == 4000) {
			setLed(LED_SOUTH_PIN, 1);
		} else if (winkel == 5000) {
			setLed( (LED_NORTH_PIN |LED_EAST_PIN |
				LED_SOUTH_PIN| LED_WEST_PIN),0);

		}

	}
	//never get here
	return;
}
