#ifndef  __driverprodemo_H_
#define __driverprodemo_H_

#include "stdint.h"
#define WRITE_CMD	0xF8
#define WRITE_DAT	0xFA

#define CS_Pin   GPIO_PIN_2
#define CS_Port  GPIOD

#define SID_Pin  GPIO_PIN_5
#define SID_Port GPIOB

#define CLK_Pin  GPIO_PIN_3
#define CLK_Port GPIOB

#define RST_Pin  GPIO_PIN_9
#define RST_Port GPIOB

#define BLK_Pin  GPIO_PIN_8
#define BLK_Port GPIOB

#define LCD_SID_H  HAL_GPIO_WritePin(SID_Port, SID_Pin, GPIO_PIN_SET)
#define LCD_SID_L  HAL_GPIO_WritePin(SID_Port, SID_Pin, GPIO_PIN_RESET)

#define LCD_CLK_H  HAL_GPIO_WritePin(CLK_Port, CLK_Pin, GPIO_PIN_SET)
#define LCD_CLK_L  HAL_GPIO_WritePin(CLK_Port, CLK_Pin, GPIO_PIN_RESET)

#define LCD_CS_H   HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_SET)
#define LCD_CS_L   HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_RESET)

#define LCD_RST_H  HAL_GPIO_WritePin(RST_Port, RST_Pin, GPIO_PIN_SET)
#define LCD_RST_L  HAL_GPIO_WritePin(RST_Port, RST_Pin, GPIO_PIN_RESET)

#define LCD_BLK_H  HAL_GPIO_WritePin(BLK_Port, BLK_Pin, GPIO_PIN_SET)
#define LCD_BLK_L  HAL_GPIO_WritePin(BLK_Port, BLK_Pin, GPIO_PIN_RESET)

#define uchar unsigned char

void LCD_Init(void);

void Delay_us(uint32_t us);

void Delay_ms(uint32_t ms);

void LCD_WriteCmd(uint8_t Cmd );

void LCD_WriteDat(uint8_t Dat );

uchar LCD_ReadData();

void LCD_BufferCheck(void);

void LCD_Display_Nums(uint8_t x, uint8_t y, uint8_t num);

void LCD_Display_String(uint8_t x,uint8_t y,uint8_t*str);

void LCD_Display_Imag(const uchar* ptr);

void LCD_Clear(void);

//void LCD_SetLocation(uint8_t x,uint8_t y,uint8_t*data)
void LCD_Display_Stringg(uint8_t x,uint8_t y,uint8_t*str);


#endif