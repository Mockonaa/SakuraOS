/*
*
*   Author: Matthias Guthörl (Github - fliescher)
*   Based on ILI9341.c published on https://github.com/NickNagy/Cortet/blob/master/Src/IL9341.c
*
*   Using a 16bit LCD Interface!
*
*/


/*
 * This file was grabbed from https://drive.google.com/file/d/1f4WZ3Bz8Tb-dCiqacXoX_CF3trXw5EcH/view and was originally written by Mohamed Yaqoob,
 * whom in turn referred a variety of open-source libraries.
 * I have since added/modified a lot of the functions but register macros were pre-defined (and can also be verified by the ST7789V datasheet)
 *
*/

#include <ST7789V.h>
#include "main.h"
#include <stdlib.h>
#include <algorithm>

static uint8_t rotationNum=1;
static bool _cp437    = false;

void ST7789V_SEND_COMMAND(uint16_t cmd)
{
    ST7789_REG = cmd;

}
void ST7789V_SEND_DATA(uint16_t data)
{
    ST7789_RAM = data;
}
uint16_t ST7789V_READ_DATA(void)
{
	uint16_t ram;
    ram = ST7789_RAM;
    return ram;
}

void ST7789V_SEND_DATA_BUFFERED(uint16_t *buff, size_t buff_size) {
	for (uint16_t i = 0; i < buff_size; i++){
		ST7789V_SEND_DATA(buff[i]);
	}
}

void ST7789V_enableBacklight() {
	HAL_GPIO_WritePin(LCD_Backlight_GPIO_Port, LCD_Backlight_Pin, GPIO_PIN_SET);
}

void ST7789V_disableBacklight() {
	HAL_GPIO_WritePin(LCD_Backlight_GPIO_Port, LCD_Backlight_Pin, GPIO_PIN_RESET);
}

void ST7789V_enablePower() {
	HAL_GPIO_WritePin(LCD_PWR_GPIO_Port, LCD_PWR_Pin, GPIO_PIN_SET);
}

void ST7789V_disablePower() {
	HAL_GPIO_WritePin(LCD_PWR_GPIO_Port, LCD_PWR_Pin, GPIO_PIN_RESET);
}

void ST7789V_enableExtendedControl() {
	HAL_GPIO_WritePin(LCD_EXTC_GPIO_Port, LCD_EXTC_Pin, GPIO_PIN_SET);
}

void ST7789V_disableExtendedControl() {
	HAL_GPIO_WritePin(LCD_EXTC_GPIO_Port, LCD_EXTC_Pin, GPIO_PIN_RESET);
}


void ST7789V_writeRegister16(uint8_t r, uint16_t d) {
	ST7789V_SEND_COMMAND(r);
	ST7789V_SEND_DATA((d >> 8));
	ST7789V_SEND_DATA(d);
}

void ST7789V_writeRegister32(uint8_t r, uint32_t d) {
	ST7789V_SEND_COMMAND(r);
	ST7789V_SEND_DATA((d >> 24));
	ST7789V_SEND_DATA((d >> 16));
	ST7789V_SEND_DATA((d >> 8));
	ST7789V_SEND_DATA(d);
}

void ST7789V_setCursorPosition(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
  uint32_t t;
  t = (x0 << 16) | x1;
  ST7789V_SEND_COMMAND(ST7789V_RAMWR);
  ST7789V_writeRegister32(ST7789V_CASET, t);
  t = (y0 << 16) | y1;
  ST7789V_SEND_COMMAND(ST7789V_RAMWR);
  ST7789V_writeRegister32(ST7789V_RASET, t);
  ST7789V_SEND_COMMAND (ST7789V_RAMWR);
}

/* inverts colors of given rows of screen */
void ST7789V_invertRows(uint16_t y0, uint16_t y1) {
	/* define partial area */
	uint32_t t;
	t = (y0 << 16) | y1;
	ST7789V_writeRegister32(ST7789V_PTLAR, t);
	/* turn on partial mode */
	ST7789V_SEND_COMMAND(ST7789V_PTLON);
	/* invert area */
	ST7789V_SEND_COMMAND(ST7789V_INVON);
}

void ST7789V_init(void)
 {
   ExternalSRAMSpecStruct ST7789VSpec = {
/*
           .dataSize = 8,
		   .tACC = 40,
		   .tAS = 0,
		   .tWRLW = 15,
		   .tCycRead = 160,
		   .tCycWrite = 66,
		   .writeOnly = 1
*/
           .dataSize = 16,
		   .tACC = 50,
		   .tAS = 0,
		   //.tWRLW = 15,
		   //.tCycRead = 160,
		   //.tCycWrite = 66,
		   .writeOnly = 1

   };

   FMCSRAMInit(&ST7789VSpec, ST7789V_SRAM_BANK);

   HAL_GPIO_WritePin(FMC_RESET_GPIO_Port, FMC_RESET_Pin, GPIO_PIN_SET);
   HAL_Delay(1);
   HAL_GPIO_WritePin(FMC_RESET_GPIO_Port, FMC_RESET_Pin, GPIO_PIN_RESET);
   HAL_Delay(10);
   HAL_GPIO_WritePin(FMC_RESET_GPIO_Port, FMC_RESET_Pin, GPIO_PIN_SET);
   HAL_Delay(120);


   ST7789V_SEND_COMMAND(ST7789V_SLPOUT);
   HAL_Delay(120);

   ST7789V_SEND_COMMAND(ST7789V_MADCTL); //Memory data acccess control
   ST7789V_SEND_DATA(0x00);

   ST7789V_SEND_COMMAND(ST7789V_PORCTRL); //Porch Setting
   ST7789V_SEND_DATA(0x0C);
   ST7789V_SEND_DATA(0x0C);
   ST7789V_SEND_DATA(0x00);
   ST7789V_SEND_DATA(0x33);
   ST7789V_SEND_DATA(0x33);

   ST7789V_SEND_COMMAND(ST7789V_GCTRL); //Gate Control
   ST7789V_SEND_DATA(0x70); //VGH, VGL

   ST7789V_SEND_COMMAND(ST7789V_VCOMS);
   ST7789V_SEND_DATA(0x3A);

   ST7789V_SEND_COMMAND(ST7789V_LCMCTRL);
   ST7789V_SEND_DATA(0x2C);

   ST7789V_SEND_COMMAND(ST7789V_VDVVRHEN);
   ST7789V_SEND_DATA(0x01);
   ST7789V_SEND_COMMAND(ST7789V_VRHS);
   ST7789V_SEND_DATA(0x14);
   ST7789V_SEND_COMMAND(ST7789V_VDVS);
   ST7789V_SEND_DATA(0x20);

   ST7789V_SEND_COMMAND(ST7789V_FRCTRL2);
   ST7789V_SEND_DATA(0x0F); //60Hz 0A

   ST7789V_SEND_COMMAND(ST7789V_PWCTRL1);
   ST7789V_SEND_DATA(0xA4);
   ST7789V_SEND_DATA(0xA1); //AVDD VCL

   ST7789V_SEND_COMMAND(ST7789V_PVGAMCTRL);
   ST7789V_SEND_DATA(0xD0);
   ST7789V_SEND_DATA(0x07);
   ST7789V_SEND_DATA(0x0D);
   ST7789V_SEND_DATA(0x09);
   ST7789V_SEND_DATA(0x08);
   ST7789V_SEND_DATA(0x25);
   ST7789V_SEND_DATA(0x28);
   ST7789V_SEND_DATA(0x53);
   ST7789V_SEND_DATA(0x39);
   ST7789V_SEND_DATA(0x12);
   ST7789V_SEND_DATA(0x0B);
   ST7789V_SEND_DATA(0x0A);
   ST7789V_SEND_DATA(0x17);
   ST7789V_SEND_DATA(0x34);

   ST7789V_SEND_COMMAND(ST7789V_NVGAMCTRL);
   ST7789V_SEND_DATA(0xD0);
   ST7789V_SEND_DATA(0x07);
   ST7789V_SEND_DATA(0x0D);
   ST7789V_SEND_DATA(0x09);
   ST7789V_SEND_DATA(0x09);
   ST7789V_SEND_DATA(0x25);
   ST7789V_SEND_DATA(0x29);
   ST7789V_SEND_DATA(0x35);
   ST7789V_SEND_DATA(0x39);
   ST7789V_SEND_DATA(0x13);
   ST7789V_SEND_DATA(0x0A);
   ST7789V_SEND_DATA(0x0A);
   ST7789V_SEND_DATA(0x16);
   ST7789V_SEND_DATA(0x34);

   ST7789V_SEND_COMMAND(ST7789V_INVON);

   ST7789V_SEND_COMMAND(ST7789V_COLMOD);
   ST7789V_SEND_DATA(0x55); //65K colors, 16 bit/px

   ST7789V_SEND_COMMAND(ST7789V_CASET);
   ST7789V_SEND_DATA(0x00);
   ST7789V_SEND_DATA(0x00);
   ST7789V_SEND_DATA(0x00);
   ST7789V_SEND_DATA(0xEF);

   ST7789V_SEND_COMMAND(ST7789V_RASET);
   ST7789V_SEND_DATA(0x00);
   ST7789V_SEND_DATA(0x00);
   ST7789V_SEND_DATA(0x01);
   ST7789V_SEND_DATA(0x3F);

   ST7789V_SEND_COMMAND(ST7789V_DISPON);
//      ST7789V_SEND_COMMAND(ST7789V_RAMWR);
 }

void ST7789V_drawPixel(uint16_t x, uint16_t y, uint16_t color) {
	ST7789V_setCursorPosition(x, y, x, y);
	ST7789V_SEND_DATA(color);
}

void ST7789V_fill(uint16_t color) {
	uint32_t n = ST7789V_PIXEL_COUNT;
	
	if(rotationNum==1 || rotationNum==3)
	{
		ST7789V_setCursorPosition(0, 0,   ST7789V_WIDTH -1, ST7789V_HEIGHT -1);
	}
	else if(rotationNum==2 || rotationNum==4)
	{
		ST7789V_setCursorPosition(0, 0, ST7789V_HEIGHT -1, ST7789V_WIDTH -1);
	}

	while (n) {
	   n--;
	   ST7789V_SEND_DATA(color);
	}
}

/* assumes x1>x0 and y1>y0 */
void ST7789V_fillRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
	uint32_t n = ((x1+1)-x0)*((y1+1)-y0);
	if (n>ST7789V_PIXEL_COUNT) n=ST7789V_PIXEL_COUNT;
	ST7789V_setCursorPosition(x0, y0, x1, y1);
	while (n) {
			n--;
			ST7789V_SEND_DATA(color);
	}
}

void ST7789V_drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  ST7789V_drawPixel(x0  , y0+r, color);
  ST7789V_drawPixel(x0  , y0-r, color);
  ST7789V_drawPixel(x0+r, y0  , color);
  ST7789V_drawPixel(x0-r, y0  , color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  
    ST7789V_drawPixel(x0 + x, y0 + y, color);
    ST7789V_drawPixel(x0 - x, y0 + y, color);
    ST7789V_drawPixel(x0 + x, y0 - y, color);
    ST7789V_drawPixel(x0 - x, y0 - y, color);
    ST7789V_drawPixel(x0 + y, y0 + x, color);
    ST7789V_drawPixel(x0 - y, y0 + x, color);
    ST7789V_drawPixel(x0 + y, y0 - x, color);
    ST7789V_drawPixel(x0 - y, y0 - x, color);
  }
}

void drawCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, uint16_t color)
{
	int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if (cornername & 0x4) {
      ST7789V_drawPixel(x0 + x, y0 + y, color);
      ST7789V_drawPixel(x0 + y, y0 + x, color);
    } 
    if (cornername & 0x2) {
      ST7789V_drawPixel(x0 + x, y0 - y, color);
      ST7789V_drawPixel(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8) {
      ST7789V_drawPixel(x0 - y, y0 + x, color);
      ST7789V_drawPixel(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1) {
      ST7789V_drawPixel(x0 - y, y0 - x, color);
      ST7789V_drawPixel(x0 - x, y0 - y, color);
    }
  }
}
void fillCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, uint16_t delta, uint16_t color)
{
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1) {
      ST7789V_drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
      ST7789V_drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
    }
    if (cornername & 0x2) {
      ST7789V_drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
      ST7789V_drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
    }
  }
}

void ST7789V_fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
  ST7789V_drawFastVLine(x0, y0-r, 2*r+1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
}

void ST7789V_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
	std::swap(x0, y0);
	std::swap(x1, y1);
  }

  if (x0 > x1) {
	std::swap(x0, x1);
	std::swap(y0, y1);
  }

  uint16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx>>1;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0<=x1; x0++) {
    if (steep) {
      ST7789V_drawPixel(y0, x0, color);
    } else {
      ST7789V_drawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}	

void ST7789V_drawFastHLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color)
{
	ST7789V_drawLine(x, y, x+w-1, y, color);
}

void ST7789V_drawFastVLine(uint16_t x, uint16_t y, uint16_t h, uint16_t color)
{
	ST7789V_drawLine(x, y, x, y+h-1, color);
}

void ST7789V_drawRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
	ST7789V_drawFastHLine(x0, y0, x1-x0, color);
	ST7789V_drawFastVLine(x0, y0, y1-y0, color);
	ST7789V_drawFastHLine(x0, y1, x1-x0, color);
	ST7789V_drawFastVLine(x1, y0, y1-y0, color);
}

void ST7789V_drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
  ST7789V_drawLine(x0, y0, x1, y1, color);
  ST7789V_drawLine(x1, y1, x2, y2, color);
  ST7789V_drawLine(x2, y2, x0, y0, color);
}

/* left params as signed ints just to reaffirm logic inside function */
void ST7789V_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
	int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    std::swap(y0, y1); std::swap(x0, x1);
  }
  if (y1 > y2) {
    std::swap(y2, y1); std::swap(x2, x1);
  }
  if (y0 > y1) {
	std::swap(y0, y1); std::swap(x0, x1);
  }

  if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a)      a = x1;
    else if(x1 > b) b = x1;
    if(x2 < a)      a = x2;
    else if(x2 > b) b = x2;
    ST7789V_drawFastHLine(a, y0, b-a+1, color);
    return;
  }

  int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1,
    sa   = 0,
    sb   = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if(y1 == y2) last = y1;   // Include y1 scanline
  else         last = y1-1; // Skip it

  for(y=y0; y<=last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;

    if(a > b) std::swap(a,b);
    ST7789V_drawFastHLine(a, y, b-a+1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;

    if(a > b) std::swap(a,b);
    ST7789V_drawFastHLine(a, y, b-a+1, color);
	}
}

void ST7789V_drawChar(uint16_t x, uint16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size)
{
	if((x >= ST7789V_WIDTH)            || // Clip right
     (y >= ST7789V_HEIGHT)           || // Clip bottom
     ((x + 6 * size - 1) < 0) || // Clip left
     ((y + 8 * size - 1) < 0))   // Clip top
    return;

  if(!_cp437 && (c >= 176)) c++; // Handle 'classic' charset behavior

  for (int8_t i=0; i<6; i++ ) {
    uint8_t line;
    if (i == 5) 
      line = 0x0;
    else 
      line = pgm_read_byte(font1+(c*5)+i);
    for (int8_t j = 0; j<8; j++) {
      if (line & 0x1) {
        if (size == 1) // default size
          ST7789V_drawPixel(x+i, y+j, color);
        else {  // big size
          ST7789V_fillRect(x+(i*size), y+(j*size), size + x+(i*size), size+1 + y+(j*size), color);
        } 
      } else if (bg != color) {
        if (size == 1) // default size
          ST7789V_drawPixel(x+i, y+j, bg);
        else {  // big size
          ST7789V_fillRect(x+i*size, y+j*size, size + x+i*size, size+1 + y+j*size, bg);
        }
      }
      line >>= 1;
    }
  }
}

void ST7789V_printText(unsigned char text[], uint16_t x, uint16_t y, uint16_t color, uint16_t bg, uint8_t size)
{
	int16_t offset;
	offset = size*6;
	for(uint16_t i=0; i<40 && text[i]!=NULL; i++)
	{
		ST7789V_drawChar(x+(offset*i), y, text[i],color,bg,size);
	}
}

void ST7789V_printImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data, uint32_t size)
{
	uint32_t n = size;
	ST7789V_setCursorPosition(x, y, w+x-1, h+y-1);
	for(uint32_t i=0; i<n ; i++)
	{
		ST7789V_SEND_DATA(data[i]);
	}
}



void ST7789V_setRotation(uint8_t rotate)
{
	switch(rotate) {
		case 1:
			rotationNum = 1;
			ST7789V_SEND_COMMAND(ST7789V_MADCTL);
			ST7789V_SEND_DATA(ST7789V_MADCTL_MY | ST7789V_MADCTL_BGR);
			break;
		case 2:
			rotationNum = 2;
			ST7789V_SEND_COMMAND(ST7789V_MADCTL);
			ST7789V_SEND_DATA(ST7789V_MADCTL_MV | ST7789V_MADCTL_BGR);
			break;
		case 3:
			rotationNum = 3;
			ST7789V_SEND_COMMAND(ST7789V_MADCTL);
			ST7789V_SEND_DATA(ST7789V_MADCTL_MX | ST7789V_MADCTL_BGR);
			break;
		case 4:
			rotationNum = 4;
			ST7789V_SEND_COMMAND(ST7789V_MADCTL);
			ST7789V_SEND_DATA(ST7789V_MADCTL_MX | ST7789V_MADCTL_MY | ST7789V_MADCTL_MV | ST7789V_MADCTL_BGR);
			break;
		default:
			rotationNum = 1;
			ST7789V_SEND_COMMAND(ST7789V_MADCTL);
			ST7789V_SEND_DATA(ST7789V_MADCTL_MY | ST7789V_MADCTL_BGR);
			break;
	}
}
