#include "main.h"
#include "arm_math.h"
#include "math.h"

#include "platf_usart1.h"
/*#include "platf_led.h"*/
/*#include "platf_i2c3.h"*/


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

void mainhead(void)
{
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	unsigned int langweil = 0 ;

	/* SysTick end of count event each 0.1ms */
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 10000);
}



static void debug_dump_i2c_v(void){

	char buf[32];

	sprintf(buf,"cr1:%x   cr2:%x\r\n",
			(((I2C_TypeDef*)I2C3_BASE)->CR1),
			(((I2C_TypeDef*)I2C3_BASE)->CR2));
	USART_puts(buf);
	sprintf(buf,"oar1:%x   oar2:%x\r\n",
			(((I2C_TypeDef*)I2C3_BASE)->OAR1),
			(((I2C_TypeDef*)I2C3_BASE)->OAR2));
	USART_puts(buf);
	sprintf(buf,"dr:%x   ccr:%x\r\n",
			(((I2C_TypeDef*)I2C3_BASE)->DR),
			(((I2C_TypeDef*)I2C3_BASE)->CCR));
	USART_puts(buf);
	sprintf(buf,"sr1:%x   sr2:%x\r\n",
			(((I2C_TypeDef*)I2C3_BASE)->SR1),
			(((I2C_TypeDef*)I2C3_BASE)->SR2));
	USART_puts(buf);
	sprintf(buf,"trise:%x   test:%x\r\n",
			(((I2C_TypeDef*)I2C3_BASE)->TRISE),
			(uint16_t)0xc3d2 );
	USART_puts(buf);

}


void main(void)
{ 
	mainhead();
	char buf[32];
	init_USART1();

	 
	USART_puts("ttttttttttt\r\n\n\n\ngoing init i2c3: \r\n");
	i2cinit();

	Delay(1);
	debug_dump_i2c_v();
	
	USART_puts(" gen start & wait for master mode:\r\n");
	 I2C_GenerateSTART( I2C3, ENABLE );

	 debug_dump_i2c_v();
	 I2C_GenerateSTART( I2C3, ENABLE );
  	while( I2C_CheckEvent( I2C3, I2C_EVENT_MASTER_MODE_SELECT ) != SUCCESS );

	USART_puts("done!\r\n");


	for(;;)
		USART_puts("Endloop \r\n");

}
