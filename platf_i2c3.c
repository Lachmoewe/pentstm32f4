
#include "main.h"
#include "platf_i2c3.h"



void i2cinit(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;



	/* I2C3 peripheral configuration */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0xa3;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = confd_i2c3_clockspeed;
	/* Enable the I2C peripheral */
	I2C_DeInit(I2C3);
	I2C_Init(I2C3, &I2C_InitStructure);
	I2C_Cmd(I2C3, ENABLE);  


	/* Enable the I2C3 peripheral clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);
	RCC_AHB1PeriphClockCmd((RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOA), ENABLE);


	GPIO_StructInit( &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;  //SCL at PA8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
       /* GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;*/
	/*GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;*/
	/*GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;*/
	GPIO_Init(GPIOA , &GPIO_InitStructure);

	GPIO_StructInit( &GPIO_InitStructure );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  //SDA at PC9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
       /* GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;*/
	/*GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;*/
	/*GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;*/
	GPIO_Init(GPIOC , &GPIO_InitStructure);


	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8 , GPIO_AF_I2C3);  
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9 , GPIO_AF_I2C3);  



	return ;
}

