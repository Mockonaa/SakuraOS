// SakuraOS LED Driver.

#include "led.h"

void set_led(int red, int green, int blue) {
	if (red == 1) {
		HAL_GPIO_WritePin(redled_GPIO_Port, redled_Pin, 1);
	}
	else {
		HAL_GPIO_WritePin(redled_GPIO_Port, redled_Pin, 0);
	}
	if (green == 1) {
			HAL_GPIO_WritePin(greenled_GPIO_Port, greenled_Pin, 1);
	}
	else {
		HAL_GPIO_WritePin(greenled_GPIO_Port, greenled_Pin, 0);
	}
	if (blue == 1) {
		HAL_GPIO_WritePin(blueled_GPIO_Port, blueled_Pin, 1);
	}
	else {
		HAL_GPIO_WritePin(blueled_GPIO_Port, blueled_Pin, 0);
	}
}

void clear_led() {
	HAL_GPIO_WritePin(redled_GPIO_Port, blueled_Pin, 0);
	HAL_GPIO_WritePin(greenled_GPIO_Port, blueled_Pin, 0);
	HAL_GPIO_WritePin(blueled_GPIO_Port, blueled_Pin, 0);
}
