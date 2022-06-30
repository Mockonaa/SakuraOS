// SakuraOS LED Driver.

#include "led.h"

void set_led(int red, int green, int blue) {
	if (red == 1) {
		HAL_GPIO_WritePin(redled_GPIO_Port, redled_Pin, GPIO_PIN_SET);
	}
	else {
		HAL_GPIO_WritePin(redled_GPIO_Port, redled_Pin, GPIO_PIN_RESET);
	}
	if (green == 1) {
			HAL_GPIO_WritePin(greenled_GPIO_Port, greenled_Pin, GPIO_PIN_SET);
	}
	else {
		HAL_GPIO_WritePin(greenled_GPIO_Port, greenled_Pin, GPIO_PIN_RESET);
	}
	if (blue == 1) {
		HAL_GPIO_WritePin(blueled_GPIO_Port, blueled_Pin, GPIO_PIN_SET);
	}
	else {
		HAL_GPIO_WritePin(blueled_GPIO_Port, blueled_Pin, GPIO_PIN_RESET);
	}
}

void clear_led() {
	HAL_GPIO_WritePin(redled_GPIO_Port, blueled_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(greenled_GPIO_Port, blueled_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(blueled_GPIO_Port, blueled_Pin, GPIO_PIN_RESET);
}
