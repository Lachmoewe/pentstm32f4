#include "main.h"
#include "arm_math.h" 
#include "string.h"
#include "font.h"
#include <stdlib.h>
#include "string.h"

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
void init_rand() {
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);
	RNG_Cmd(ENABLE);
}
int buffer0[25]  = {
			15, 0,15, 0,15,
			 0,15, 0,15, 0,
			15, 0,15, 0,15,
			 0,15, 0,15, 0,
			15, 0,15, 0,15
};
int buffer1[25]  = {
			 0,15, 0,15, 0,
			15, 0,15, 0,15,
			 0,15, 0,15, 0,
			15, 0,15, 0,15,
			 0,15, 0,15, 0
};

int *frontbuffer=buffer0;
int *backbuffer=&buffer1[0];

void buffer_flip(void) {
	int f = frontbuffer;
	frontbuffer=backbuffer;
	backbuffer=f;   // TODO maybe we don't need this?
	// why would you need the frontbuffer to be back in backbuffer?
	// for this case we now have buffer_push()
}
/*void buffer_push(void) {
	frontbuffer = backbuffer;
}*/

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
	GPIO_WriteBit(GPIOB, GPIO_Pin_9, *(frontbuffer+line*5+4)!=0);
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
int getLedXY_front(int x, int y) {
	return *(frontbuffer+x*5+y);
}
int getLedXY_back(int x, int y) {
	return *(backbuffer+x*5+y);
}
void draw_rect(int startx,int starty,int endx,int endy,int col) {
	uint8_t x,y;
	for(x=startx; x <= endx; x++) {
		for(y=starty; y <= endy; y++) {
			setLedXY(x, y, col);
		}   
	}   
}
/*int random(int salt) {
	//int r = salt*tick;
	//r=r+12;
	//r=r*5/3;
	return step*salt;
}*/
void draw_random(void) {
	uint8_t x,y,col;
	for (x=0; x<=LED_WIDTH; x++) {
		for (y=0; y<=LED_HEIGHT; y++) {
			col=(RNG_GetRandomNumber() & 0x01)*15;
			setLedXY(x,y,col);
		}
	}
}
void startsequence(void) {
	init_rand();
	int initshit_counter=0;
	//buzzer_on();
	Delay(100);
	//buzzer_off();
	while (initshit_counter<=4) {
		Delay(200);
		buffer_flip();
		initshit_counter++;
	}
	draw_rect(0,0,4,4,0);
	Delay(200);
	uint8_t i,x,x1,x2,y,y1,y2;
	x1=0;
	x2=0;
	y1=0;
	y2=0;
	for(i=0; i<(LED_WIDTH*LED_HEIGHT); i++) {
		draw_rect(0,0,4,4,0);
		setLedXY(x2,y2, 8);
		setLedXY(x1,y1,12);
		y=LED_HEIGHT-i/LED_HEIGHT-1;
		if (y%2) {
			x=i%LED_WIDTH;
		} else {
			x=LED_WIDTH-i%LED_WIDTH-1;
		}
		setLedXY(x,y,15);
		x2=x1;
		y2=y1;
		x1=x;
		y1=y;
		buffer_flip();
		Delay(50);
	}
	draw_rect(0,0,4,4,0);
	buffer_flip();
	int ran;
	for(ran=1; ran<=60; ran++) {
		Delay(20);
		draw_random();
		buffer_flip();
	}
}
void draw_text(char text2[]) {

	//char text2[] =" Hello World! ";
	//const uint8_t text_len2 = 14;
	uint8_t text_len2=strlen(text2);
	uint16_t pos2 = 0;
	uint8_t x, y;
	while (pos2 + LED_WIDTH != text_len2 * 4) {
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
		Delay(200);
		buffer_flip();
	}
}
void spiral(void) {
	uint8_t posx = 2;						//current position
	uint8_t posy = 2;
	uint8_t dx = 1;												//to add
	uint8_t dy = 0;
	uint8_t cycle = 1;												//times to cycle
	//uint8_t cy = 1;
	uint8_t loopx=0;										//what loop are we in?
	uint8_t loopy=0;										//i dont even know what im doing here
	setLedXY(posx,posy,15);
	uint8_t counter = 0;
	uint8_t store;
	while (counter < 3) {
		while (loopx < cycle) {
			posx += dx;
			while (loopy < cycle) {
				posy += dy;
				loopy++;
				setLedXY(posx,posy,15);
				buffer_flip();
				Delay(50);
			}
			loopy = 0;
			loopx++;
			store = dx;
			dx = dy;
			dy = store;
		}
		loopx = 0;
		cycle++;
		dx = -dx;
		dy = -dy;
		counter++;
	}
	/*+1, 0
	 0,+1
	-1, 0
	-1, 0
	 0,-1
	 0,-1
   1, 0
	 1, 0
	 1, 0*/


}


int main(void)
{
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	LED_matrix_init();
	buzzer_init();
	SysTick_Config(RCC_Clocks.HCLK_Frequency /200000);
	//startsequence();
	int ran;
	while (1) {
		//draw_text(" C3D2 Hardware Hacking Area ");
	
		draw_rect(0,0,4,4,8);	
		Delay(100);
		spiral();
		/*for(ran=1; ran<=60; ran++) {
			Delay(20);
			draw_random();
			buffer_flip();
		}*/
	}


	//static void init_ball(void) ATTRIBUTES;
	//static uint8_t tick_ball(void);
/*#define BOFF            -1
	static int8_t x = 2, y = 2; 
	static int8_t dx = 1, dy = 1;
	while(1) {
		if(x == BOFF || x == LED_WIDTH - BOFF - 1) {
			dx *= -1;
		}
		if(y == BOFF || y == LED_HEIGHT - BOFF - 1) {
			dy *= -1;
		}

	
		// move
		x += dx; 
		y += dy; 
		 // display  
		const uint8_t ball[3][3] = { 
			{5, 10, 5},
			{10, 15, 10},
			{5, 10, 5}, 
		};
		int8_t tx=-2;	
		for(tx <= 2; ++tx;) {
			int8_t ty=-2;
			for(ty <= 2; ++ty;) {
				const int8_t ax = tx + x, 
				      ay = ty + y; 
				if(ax >= 0 && ay >= 0 && ax < LED_WIDTH && ay < LED_HEIGHT) {
					uint8_t bright = 0;
					if(abs(tx) <= 1 && abs(ty) <= 1) {   
						bright = ball[tx+1][ty+1]; 
					}
					setLedXY(ax, ay, bright);
				}
			}
		}
		buffer_flip();
		draw_rect(0,0,4,4,0);
		Delay(50);
	}
*/
	
}
