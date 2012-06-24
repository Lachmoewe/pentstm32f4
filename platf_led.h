#pragma once 
#ifndef LED_H_
#define LED_H_

#define LED_WEST_PIN                          GPIO_Pin_12
#define LED_NORTH_PIN                         GPIO_Pin_13
#define LED_EAST_PIN                          GPIO_Pin_14
#define LED_SOUTH_PIN                         GPIO_Pin_15

#define LEDS_GPIO_PORT                       GPIOD
#define LEDS_GPIO_CLK                        RCC_AHB1Periph_GPIOD  
  
#define LED_NORTH_GPIO_PORT                   GPIOD
#define LED_NORTH_GPIO_CLK                    RCC_AHB1Periph_GPIOD  
  
#define LED_EAST_GPIO_PORT                   GPIOD
#define LED_EAST_GPIO_CLK                    RCC_AHB1Periph_GPIOD  
  
#define LED_SOUTH_GPIO_PORT                   GPIOD
#define LED_SOUTH_GPIO_CLK                    RCC_AHB1Periph_GPIOD

//void initLED(Led_TypeDef led);

void initLEDs(void);

int setLed(const unsigned int leds, const unsigned int state);

#endif //LED_H_ 

