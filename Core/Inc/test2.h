#ifndef __LCD12864_H
#define __LCD12864_H	 

 
void LCD12864IO_Init(void);
 
void LCD1602_DB_IO_outputMode(void);
void LCD1602_DB_IO_inputMode(void);
 
void write_LCD1602_DB(uint8_t data);
uint8_t read_LCD1602_DB(void);
 
 
void InitLcd1602(void);
void LcdShowStr(unsigned char row, unsigned char column, unsigned char *str);
void LcdWriteCmd(unsigned char cmd);
 
void LcdShowImage(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t *img);
void LcdClearArea(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
 
extern uint8_t HE[];
extern uint8_t QQ[];
extern uint8_t myVX[];
 

 
#endif