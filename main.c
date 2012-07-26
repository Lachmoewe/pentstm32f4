#include "main.h"
#include "arm_math.h" 
#include "string.h"
#include "font.h"

// http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/DATASHEET/DM00037051.pdf
// http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/REFERENCE_MANUAL/DM00031020.pdf
#define LED_WIDTH 5
#define LED_HEIGHT 5

static __IO uint32_t TimingDelay;
static __IO uint32_t tick;
void Delay(__IO uint32_t nTime)
{
	TimingDelay = nTime*200;

	while(TimingDelay != 0);
}

GPIO_InitTypeDef  GPIO_InitStructure;
int buffer0[25]  = {
			15,15,15,15,15,
			15,15,15,15,15,
			15,15,15,15,15,
			15,15,15,15,15,
			15,15,15,15,15
};
int buffer1[25]  = {
			0,0,0,0,0,
			0,0,0,0,0,
			0,0,0,0,0,
			0,0,0,0,0,
			0,0,0,0,0
};

int *frontbuffer=buffer0;
int *backbuffer=&buffer1[0];

void buffer_flip(void) {
	int f = frontbuffer;
	frontbuffer=backbuffer;
	backbuffer=f;
}

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
	/*GPIO_WriteBit(GPIOB, GPIO_Pin_9, buffer0[line*5+4]!=0);
	GPIO_WriteBit(GPIOB, GPIO_Pin_7, buffer0[line*5+3]!=0);
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, buffer0[line*5+2]!=0);
	GPIO_WriteBit(GPIOD, GPIO_Pin_6, buffer0[line*5+1]!=0);
	GPIO_WriteBit(GPIOD, GPIO_Pin_4, buffer0[line*5+0]!=0);
	*/GPIO_WriteBit(GPIOB, GPIO_Pin_9, *(frontbuffer+line*5+4)!=0);
	GPIO_WriteBit(GPIOB, GPIO_Pin_7, *(frontbuffer+line*5+3)!=0);
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, *(frontbuffer+line*5+2)!=0);
	GPIO_WriteBit(GPIOD, GPIO_Pin_6, *(frontbuffer+line*5+1)!=0);
	GPIO_WriteBit(GPIOD, GPIO_Pin_4, *(frontbuffer+line*5+0)!=0);

}
void line_pixel_off(int line, int brightness) {	
	GPIO_WriteBit(GPIOB, GPIO_Pin_9, *(frontbuffer+line*5+4)>brightness);
	GPIO_WriteBit(GPIOB, GPIO_Pin_7, *(frontbuffer+line*5+3)>brightness);
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, *(frontbuffer+line*5+2)>brightness);
	GPIO_WriteBit(GPIOD, GPIO_Pin_6, *(frontbuffer+line*5+1)>brightness);
	GPIO_WriteBit(GPIOD, GPIO_Pin_4, *(frontbuffer+line*5+0)>brightness);
/*	GPIO_WriteBit(GPIOB, GPIO_Pin_9, buffer0[line*5+4]>brightness);
	GPIO_WriteBit(GPIOB, GPIO_Pin_7, buffer0[line*5+3]>brightness);
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, buffer0[line*5+2]>brightness);
	GPIO_WriteBit(GPIOD, GPIO_Pin_6, buffer0[line*5+1]>brightness);
	GPIO_WriteBit(GPIOD, GPIO_Pin_4, buffer0[line*5+0]>brightness);*/
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
void setLedXY(int x, int y, int brightness) {
	*(backbuffer+x*5+y)=brightness;
}
int main(void)
{
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	LED_matrix_init();
	SysTick_Config(RCC_Clocks.HCLK_Frequency /200000);
	char text2[] =" DON'T MESS WITH ME. ";

	const uint8_t text_len2 = 21;

	uint16_t pos2 = 0;

	
	while (1) {
		//frontbuffer=buffer1;
		//buffer0[3]=1;
		//Delay(400);
		//frontbuffer=buffer0;
		//buffer0[3]=10;
		Delay(200);
		uint8_t x, y;


		for(x = 0; x < LED_WIDTH; x++) {
			uint16_t p = pos2 + x;
			char c = text2[p / 4]; 
			uint8_t bits = 0;
			if((p & 3) < 3) bits =font[c - 32][p & 3]; 
			for(y = 0; y < LED_HEIGHT; y++) {
				setLedXY(x, y, 15 * (bits & 1));
				bits >>= 1;
			}   
		}   
		pos2++;
		if(pos2 + LED_WIDTH == text_len2 * 4) pos2 = 0;
		buffer_flip();
	}
}
