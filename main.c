#include "main.h"
#include "arm_math.h" 
#include "string.h"


// http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/DATASHEET/DM00037051.pdf
// http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/REFERENCE_MANUAL/DM00031020.pdf

static __IO uint32_t TimingDelay;
static __IO uint32_t tick;
void Delay(__IO uint32_t nTime)
{
	TimingDelay = nTime*100;

	while(TimingDelay != 0);
}

GPIO_InitTypeDef  GPIO_InitStructure;
int buffer0[5][5] = { 	{1,2,3,4,5},
			{6,7,8,9,10},
			{11,12,13,14,15},
			{0,15,0,15,0},
			{15,0,15,0,15}};
int buffer1[5][5] = { 	{1,0,1,0,1},
			{0,1,0,1,0},
			{1,0,1,0,1},
			{0,1,0,1,0},
			{1,0,1,0,1}};
void line_off(int line) {
	switch (line) {
		case 0:	GPIO_SetBits(GPIOD, GPIO_Pin_0);
			break;
		case 1:	GPIO_SetBits(GPIOC, GPIO_Pin_11);
			break;
		case 2:	GPIO_SetBits(GPIOA, GPIO_Pin_9);
			break;
		case 3:	GPIO_SetBits(GPIOC, GPIO_Pin_9);
			break;
		case 4:	GPIO_SetBits(GPIOC, GPIO_Pin_7);
	} 
}
void line_on(int line) {
	switch (line) {
		case 0:	GPIO_ResetBits(GPIOD, GPIO_Pin_0);
			break;
		case 1:	GPIO_ResetBits(GPIOC, GPIO_Pin_11);
			break;
		case 2:	GPIO_ResetBits(GPIOA, GPIO_Pin_9);
			break;
		case 3:	GPIO_ResetBits(GPIOC, GPIO_Pin_9);
			break;
		case 4:	GPIO_ResetBits(GPIOC, GPIO_Pin_7);
	}
}
void line_pixel_on(int line) {
	GPIO_WriteBit(GPIOB, GPIO_Pin_9, buffer0[line][4]!=0);
	GPIO_WriteBit(GPIOB, GPIO_Pin_7, buffer0[line][3]!=0);
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, buffer0[line][2]!=0);
	GPIO_WriteBit(GPIOD, GPIO_Pin_6, buffer0[line][1]!=0);
	GPIO_WriteBit(GPIOD, GPIO_Pin_4, buffer0[line][0]!=0);

}
void line_pixel_off(int line, int brightness) {	
	GPIO_WriteBit(GPIOB, GPIO_Pin_9, buffer0[line][4]>brightness);
	GPIO_WriteBit(GPIOB, GPIO_Pin_7, buffer0[line][3]>brightness);
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, buffer0[line][2]>brightness);
	GPIO_WriteBit(GPIOD, GPIO_Pin_6, buffer0[line][1]>brightness);
	GPIO_WriteBit(GPIOD, GPIO_Pin_4, buffer0[line][0]>brightness);
}
int *frontbuffer=&buffer0[0];
int *backbuffer=&buffer1[0];

void buffer_flip(void) {
	int f = frontbuffer;
	frontbuffer=backbuffer;
	backbuffer=f;
}

int line = 0;
int step = 0;
int stepmap[] = {
	0,1,2,3,4,5,0,0,6,0,0,
	7,0,0,0,0,8,0,0,0,0,
	0,0,0,9,0,0,0,0,0,0,
	0,0,0,10,0,0,0,0,0,0,
	0,0,0,0,0,0,0,11,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,12,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,13,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	14,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0};

void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
	tick++;

	step++;
	if (step==200) {
		step=0;
		line_off(line);
		line++;
		if (line == 5) {
			line=0;
		}
		line_pixel_on(line);
		line_on(line);
	} 
	if (stepmap[step]!=0) {
		line_pixel_off(line,stepmap[step]);
	}
}


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
	
	GPIO_SetBits(GPIOD, GPIO_Pin_0);
	GPIO_SetBits(GPIOC, GPIO_Pin_11|GPIO_Pin_9|GPIO_Pin_7);
	GPIO_SetBits(GPIOA, GPIO_Pin_9);
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);

	//   D0  C11 A9  C9  C7
	//B9  x   x   x   x   x
	//B7  x   x   x   x   x
	//B5  x   x   x   x   x
	//D6  x   x   x   x   x
	//D4  x   x   x   x   x
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
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 200000);

	LED_matrix_init();
	while (1) ;
}
