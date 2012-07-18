#include "main.h"
#include "arm_math.h" 
#include "string.h"


// http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/DATASHEET/DM00037051.pdf
// http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/REFERENCE_MANUAL/DM00031020.pdf

static __IO uint32_t TimingDelay;
static __IO uint32_t tick;
void Delay(__IO uint32_t nTime)
{
	TimingDelay = nTime*10;

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


void LED_matrix_init(void) {
	/* Initialyse Matrix */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_7|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_9|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_4|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//   D0  C11 A9  C9  C7
	//B9  x   x   x   x   x
	//B7  x   x   x   x   x
	//B5  x   x   x   x   x
	//D6  x   x   x   x   x
	//D4  x   x   x   x   x
}
void LED_matrix_setX(int x, int value) {
	uint16_t pin;
	GPIO_TypeDef *group;
	switch (x) {
		case 0:	group=GPIOD;
			pin=GPIO_Pin_0;
			break;
		case 1:	group=GPIOC;
			pin=GPIO_Pin_11;
			break;
		case 2:	group=GPIOA;
			pin=GPIO_Pin_9;
			break;
		case 3:	group=GPIOC;
			pin=GPIO_Pin_9;
			break;
		case 4:	group=GPIOC;
			pin=GPIO_Pin_7;
	}
	if (value) {
		GPIO_ResetBits(group, pin);
	} else {
		GPIO_SetBits(group, pin);
	}
}
void LED_matrix_setY(int y, int line[]) {
	GPIO_ResetBits(GPIOB, GPIO_Pin_9|GPIO_Pin_7|GPIO_Pin_5);
	GPIO_ResetBits(GPIOD, GPIO_Pin_6|GPIO_Pin_4);
	GPIO_SetBits(GPIOD, GPIO_Pin_0);
	GPIO_SetBits(GPIOC, GPIO_Pin_11|GPIO_Pin_9|GPIO_Pin_7);
	GPIO_SetBits(GPIOA, GPIO_Pin_9);
	switch (y) {
		case 0:	GPIO_SetBits(GPIOB, GPIO_Pin_9);
			break;
		case 1:	GPIO_SetBits(GPIOB, GPIO_Pin_7);
			break;
		case 2:	GPIO_SetBits(GPIOB, GPIO_Pin_5);
			break;
		case 3: GPIO_SetBits(GPIOD, GPIO_Pin_6);
			break;
		case 4:	GPIO_SetBits(GPIOD, GPIO_Pin_4);
	}
	int i=0;
	while (i<=4) {
		LED_matrix_setX(i,line[i]);
		i++;
	}
	//delay(10);
}
void LED_matrix_draw(int image[], int treshold) {
	while (treshold>0) {
		int i=0;
		while(i<5) {
			LED_matrix_setY(i,&image[i*5]);
			i++;
			Delay(2);
		}
		treshold--;
	}
	
}
void buzzer_init(void) {
	/*Buzzer!*/	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
void buzzer_on(void) {
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOE, GPIO_Pin_5);
}
void buzzer_off(void) {
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
}

int main(void)
{
	RCC_ClocksTypeDef RCC_Clocks;



	RCC_GetClocksFreq(&RCC_Clocks);
	/* SysTick end of count event each 0.1ms */
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 10000);


	LED_matrix_init();
	int image1[]={	1,1,1,1,0,
			1,1,1,1,0,
			1,1,1,1,0,
			1,1,1,1,0,
			1,1,1,1,0};
	int image2[]={	1,1,1,0,1,
			1,1,1,0,1,
			1,1,1,0,1,
			1,1,1,0,1,
			1,1,1,0,1};
	int image3[]={	1,1,0,1,1,
			1,1,0,1,1,
			1,1,0,1,1,
			1,1,0,1,1,
			1,1,0,1,1};
	int image4[]={	1,0,1,1,1,
			1,0,1,1,1,
			1,0,1,1,1,
			1,0,1,1,1,
			1,0,1,1,1};
	int image5[]={	0,1,1,1,1,
			0,1,1,1,1,
			0,1,1,1,1,
			0,1,1,1,1,
			0,1,1,1,1};
	while (1)
	{
		LED_matrix_draw(image1,10);
		LED_matrix_draw(image2,10);
		LED_matrix_draw(image3,10);
		LED_matrix_draw(image4,10);
		LED_matrix_draw(image5,10);
	}
}
