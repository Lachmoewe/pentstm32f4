#include "main.h"
#include "arm_math.h" 
#include "math.h"

// http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/DATASHEET/DM00037051.pdf
// http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/REFERENCE_MANUAL/DM00031020.pdf

static __IO uint32_t TimingDelay;
static __IO uint32_t tick;
void Delay(__IO uint32_t nTime)
{
	TimingDelay = nTime;

	while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
	tick++;
}


GPIO_InitTypeDef  GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
GPIO_InitTypeDef     GPIO_InitStructureTimer;

void USART_puts(const char *text)
{
	while (*text)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		USART_SendData(USART1, *text);
		text++;
	}

}
__attribute__( ( always_inline ) ) static __INLINE float __VSQRTF(float op1)
{
	float result;

	__ASM volatile ("vsqrt.f32 %0, %1" : "=w" (result) : "w" (op1) );
	return(result);
}
void initLed(void) {	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}
void setLed(int led, int state) {
	if (led>4) {
		return;
	}
	if (state==1) {
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
	}
	if (state==0) {
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
	}
}

int main(void)
{
	RCC_ClocksTypeDef RCC_Clocks;



	RCC_GetClocksFreq(&RCC_Clocks);
	// SysTick end of count event each 0.1ms
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

	initLed();
/*
	// GPIOD Periph clock enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	// Configure PD12, PD13, PD14 and PD15 in output pushpull mode

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// Connect USART pins to AF
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1); // USART1_TX
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1); // USART1_RX



	GPIO_InitStructureTimer.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructureTimer.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructureTimer.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructureTimer.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureTimer.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOB, &GPIO_InitStructureTimer);
	//USART_InitStructure.USART_BaudRate = 230400;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	GPIO_InitStructureTimer.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructureTimer.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructureTimer.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructureTimer.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureTimer.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructureTimer);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	TIM_TimeBaseStructure.TIM_Period = 32000; // auto reaload register
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_PrescalerConfig(TIM2, 52, TIM_PSCReloadMode_Immediate);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 5000;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); 


	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_Cmd(TIM2,ENABLE);
	float winkel = 0;
	int led = 0;
	while (1)
	{
		TIM_SetCompare2(TIM2,sin(winkel)*800+2400);
		Delay(2);
		winkel+=0.001;
		if (winkel==3.14*2) {
			winkel=0;
		}
		if (winkel==1) {
			setLed(1,0);
		}
		if (winkel==2) {
			setLed(2,0);
		}
		if (winkel==3) {
			setLed(3,0);
		}
		if (winkel==4) {
			setLed(4,0);
		}	
		if (winkel==5) {
			setLed(1,1);
			setLed(2,1);
			setLed(3,1);
			setLed(4,1);
		}
	}
}
