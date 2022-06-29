/*
 * led.h
 *
 *  Created on: Jun 29, 2022
 *      Author: Flandre Scarlet
 */

#ifndef INC_LED_H_
#define INC_LED_H_

/* Start of LED's GPIO Pins & Ports Definition */

#include "stm32f7xx_hal.h"

#define redled_Pin GPIO_PIN_4
#define redled_GPIO_Port GPIOB

#define blueled_Pin GPIO_PIN_0
#define blueled_GPIO_Port GPIOB

#define greenled_Pin GPIO_PIN_5
#define greenled_GPIO_Port GPIOB

/* End of LED's GPIO Pins & Ports Definition*/


void set_led(int red, int green, int blue);
void clear_led();




#endif /* INC_LED_H_ */
