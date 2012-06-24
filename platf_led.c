
#include "main.h"
#include "platf_led.h"

void initLEDs(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(LEDS_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin   = (LED_NORTH_PIN |LED_EAST_PIN |
					 LED_SOUTH_PIN| LED_WEST_PIN);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(LEDS_GPIO_PORT, &GPIO_InitStructure);
}


int 
setLed(const unsigned int leds, const unsigned int state)
{				//1:green, 2:orange, 3:red, 4:blue
	if (leds & ~(LED_NORTH_PIN |LED_EAST_PIN |
				LED_SOUTH_PIN| LED_WEST_PIN) ) {
		return -1 ;
	}
	if (state) {
		GPIO_SetBits(LEDS_GPIO_PORT,leds);
	} else {
		GPIO_ResetBits(LEDS_GPIO_PORT ,leds);
	}
}

