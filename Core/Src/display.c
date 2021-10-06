// SakuraOS Display Driver
#include "display.h"
#include "stdint.h"

uint16_t const SCREEN_WIDTH = 320;
uint16_t const SCREEN_HEIGHT = 240;

void StartBacklight(){
	HAL_GPIO_WritePin(LCD_Backlight_GPIO_Port, LCD_Backlight_Pin, GPIO_PIN_SET);
}

void StopBacklight(){
	HAL_GPIO_WritePin(LCD_Backlight_GPIO_Port, LCD_Backlight_Pin, GPIO_PIN_RESET);
}

void ClearScreen() {
	HAL_GPIO_WritePin(LCD_Reset_GPIO_Port, LCD_Reset_Pin, GPIO_PIN_SET);
	HAL_Delay(25);
	HAL_GPIO_WritePin(LCD_Reset_GPIO_Port, LCD_Reset_Pin, GPIO_PIN_RESET);
	HAL_Delay(50);
}

void Display_Init() {
	HAL_GPIO_WritePin(LCD_Power_GPIO_Port, LCD_Power_Pin, GPIO_PIN_SET);
	ClearScreen();
	StartBacklight();
}
