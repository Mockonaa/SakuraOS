// SakuraOS Display Driver Header
#ifndef SRC_DISPLAY_H_
#define SRC_DISPLAY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"


#define LCD_Backlight_Pin GPIO_PIN_0
#define LCD_Backlight_GPIO_Port GPIOE

#define LCD_Reset_Pin GPIO_PIN_8
#define LCD_Reset_GPIO_Port GPIOE

#define LCD_Power_Pin GPIO_PIN_8
#define LCD_Power_GPIO_Port GPIOC

#define LCD_Extd_Pin GPIO_PIN_6
#define LCD_Extd_GPIO_Port GPIOD

#define LCD_TE_Pin GPIO_PIN_11
#define LCD_TE_GPIO_Port GPIOB

#define color = 0x841F; // That's some blue!

void StartBacklight(void);
void StopBacklight(void);
void ClearScreen(void);
void Display_Init(void);

#endif /* SRC_DISPLAY_H_ */
