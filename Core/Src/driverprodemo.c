#include "gpio.h"
#include "stdint.h"
#include "driverprodemo.h"
#include "gpio.h"
#include "lcdfont.h"


#define WRITE_CMD	0xF8
#define WRITE_DATA	0xFA
#define READ_DATA       0xFE

void LCD_RST();
void SendByte(uint8_t byte);
void LCD_DisplayFont(uchar *CGRAM_ZIKU,uint8_t t);
void disp_CGRAM(uchar x,uchar y,uchar num);
void LCD_DisplayFont2(uint8_t t);

//LCD Driver Demo
/********************************************************************************
*** Function  Name	：void LCD_Init(void);
*** Function		：GPIO Initialization
*** Input  parameter：Null
*** Ouput  parameter：Null
*** Return parameter：Null
***
*** description:
********************************************************************************/

void LCD_Init(void)
{
  MX_GPIO_Init();
  LCD_RST(); 
  LCD_WriteCmd(0x30);         //一次送八位数据
  LCD_WriteCmd(0x0c);         //整体显示，游标关
  LCD_WriteCmd(0x01);         // clear
/*  LCD_WriteCmd(0x30);         //一次送八位数据
  LCD_WriteCmd(0x0c);         //整体显示，游标关
  LCD_WriteCmd(0x01);         // clear
  LCD_WriteCmd(0x02);         //DRAM 地址归位
  LCD_WriteCmd(0x80);         //设定DRAM 7位地址000 0000到地址计数器AC
  LCD_WriteCmd(0x04);         //点设定，显示字符/光标从左到右移位，DRAM地址+1
//  LCD_WriteCmd(0x0f);         //显示设定，开显示，显示光标，当前显示位反白闪动*/
}

/********************************************************************************
*** Function  Name	：void Delay_us(uint32_t us);
*** Function		：Delay_Time
*** Input  parameter：ms:time
*** Ouput  parameter：Null
*** Return parameter：Null
***
*** description:
********************************************************************************/
void Delay_us(uint32_t us){
  while(us--);
}

/********************************************************************************
*** Function  Name	：void Delay_ms(uint32_t ms);
*** Function		：Delay_Time
*** Input  parameter：ms:time
*** Ouput  parameter：Null
*** Return parameter：Null
***
*** description:
********************************************************************************/
void Delay_ms(uint32_t ms)
{
  ms = 1000*ms;
  while(ms--);
}

/********************************************************************************
*** Function  Name  	 ：void LCD_WriteCmd(uint8_t Cmd);
*** Function		 ：Control the commend  input
*** Input  parameter：Cmd:Command Input 8 bit
*** Ouput  parameter：Null
*** Return parameter：Null
***
*** description:
********************************************************************************/
void LCD_WriteCmd(uint8_t Cmd)
{ 
  LCD_CS_H;
  SendByte(WRITE_CMD);
  SendByte(0xf0&Cmd);
  SendByte((Cmd<<4)&0xf0);
  LCD_CS_H;
}

/********************************************************************************
*** Function  Name  ：void LCD_WriteData(uint8_t data);
*** Function        ：Control the Data  input
*** Input  parameter：Dat:Data Input 8 bit
*** Ouput  parameter：Null
*** Return parameter：Null
***
*** description:
********************************************************************************/
void LCD_WriteDat(uint8_t Dat)
{
  LCD_CS_H;
  SendByte(WRITE_DATA); 
  SendByte(0xf0&Dat);
  SendByte((Dat<<4)&0xf0);
  LCD_CS_H;
}

/********************************************************************************
*** Function  Name  ：uchar LCD_ReadData();
*** Function		: Read  Data 
*** Input  parameter: Null
*** Ouput  parameter：Null
*** Return parameter：LCD_Data:the return LCD_Data
*					  LCD_Status:the registe Status
***
*** description:
********************************************************************************/
uchar LCD_ReadData()
{
  LCD_CS_H;
  uchar LCDData;
  LCD_WriteCmd(READ_DATA);
  LCD_CLK_H;
  HAL_Delay(100);
  LCD_CLK_L;
  HAL_Delay(100);
  return 0;
}

/********************************************************************************
*** Function  Name  ：uchar LCD_Buffer();
*** Function	    : Read the System Buffer status
*** Input  parameter: Null
*** Ouput  parameter：Null
*** Return parameter: LCD_Status:the Buffer status
*				
***
*** description:
********************************************************************************/
void LCD_BufferCheck(void)
{
  uint16_t DataPort = 0xFF;
  LCD_CS_L;
  LCD_SID_H;
  LCD_CLK_H;
  while(DataPort&0x80);
  LCD_CLK_L;
}

/********************************************************************************
*** Function  Name	：void LCD_Display_Nums(uint8_t x, uint8_t y, uint8_t num);
*** Function		：LCD display Numbers
*** Input  parameter: x:the Abscissa
*		      y:the Ordinate
*		      num:display number
*** Ouput  parameter：LCD display number
*** Return parameter：Null
***
*** description:
********************************************************************************/
void LCD_Display_Nums(uint8_t x, uint8_t y, uint8_t num)
{
  uint8_t i = num+0x30;
  LCD_WriteCmd(LCD_addr[x][y]); 
  LCD_WriteDat(i);
}

/********************************************************************************
*** Function  Name	：void LCD_Display_String(uint8_t x, uint8_t y, uint8_t* str);
*** Function		：LCD display String
*** Input  parameter: x:the Abscissa
*		       y:the Ordinate
*					str:display
*** Ouput  parameter：LCD display string
*** Return parameter：Null
***
*** description:
********************************************************************************/
void LCD_Display_String(uint8_t x,uint8_t y,uint8_t*str)
{ 
  LCD_WriteCmd(LCD_addr[x][y]); //写初始光标位置
  while(*str>0)
  { 
    LCD_WriteDat(*str);    //写数据
    str++;     
  }
}

/********************************************************************************
*** Function  Name	：void LCD_Display_Imag(const uchar* ptr);
*** Function		：LCD display Images
*** Input  parameter: ptr:the pictures parameters
*** Ouput  parameter：LCD display image
*** Return parameter：Null
***
*** description:
********************************************************************************/
void LCD_Display_Imag(const uchar* ptr)
{
  unsigned int i,j,k;
  if(ptr==(void*)0)   //强制转换成空字符NULL
  LCD_Clear();
  LCD_WriteCmd(0x34);      //打开扩展指令集
  for(i=0x80,j=0;j<32;j++)
  {
    LCD_WriteCmd(i++);
    LCD_WriteCmd(0x80);
    for(k=0;k<16;k++)
    {
      if(ptr==(void*)0)
        LCD_WriteDat(*ptr++);
      else
        LCD_WriteCmd(0x00);
    }
  }
  for(i=0x80,j=0;j<32;j++)
  {
    LCD_WriteCmd(i++);
    LCD_WriteCmd(0x88);
    for(k=0;k<16;k++)
    {
      if(ptr==(void*)0)
        LCD_WriteDat(*ptr++);
      else
        LCD_WriteCmd(0x00);
    }
  }
  LCD_WriteCmd(0x36); //打开扩充指令集动作，打开绘图显示Off
  LCD_WriteCmd(0x30); //回到基本指令集
}

/********************************************************************************
*** Function  Name	：void LCD_Clear(void)
*** Function		：Clear the LCD display
*** Input  parameter: Null
*** Ouput  parameter：Null
*** Return parameter：Null
***
*** description:
********************************************************************************/
void LCD_Clear(void)
{
  LCD_WriteCmd(0x01);
  HAL_Delay(10);
}

/********************************************************************************
*** Function  Name	：void LCD_DisplayFont12(uchar x,uchar y,uint8_t t)
*** Function		：Display the Chinese Fontzise of 12
*** Input  parameter: x:the Abscissa
		      y:the Ordinate
                      t:the Chinese font in lcdfont place
*** Ouput  parameter：Null
*** Return parameter：Null
***
*** description:but the function only to make 4 fonts,and you need to make up
                the 32 hexs in the last,add 0x00
********************************************************************************/
void LCD_DisplayFont12(uchar x,uchar y,uint8_t t)
{
  uchar *CGRAM_ZIKU;
  switch(t)
  {
  case 0:CGRAM_ZIKU = CGRAM_ZIKU12;break;
  case 1:CGRAM_ZIKU = CGRAM_ZIKU13;break;
  case 2:CGRAM_ZIKU = CGRAM_ZIKU14;break;
  case 3:CGRAM_ZIKU = CGRAM_ZIKU15;break;
  }
  LCD_DisplayFont(CGRAM_ZIKU,t);
  disp_CGRAM(x,y,t);
}

void LCD_DrawPoiot(uchar dat)
{
   uchar i,j,k;
   uchar GDRAM_X=0X80;
   uchar GDRAM_Y=0x80;
   for(i=0;i<2;i++)
   {
     for(j=0;j<32;j++)
     {
       for(k=0;k<8;k++)
       {
         LCD_WriteCmd(0x34);
         LCD_WriteCmd(GDRAM_X+k);
         LCD_WriteCmd(GDRAM_Y+j);
         LCD_WriteDat(dat);
         LCD_WriteDat(dat);
       }
     }
     GDRAM_X = 0x88;
   }
   LCD_WriteCmd(0x36);
   LCD_WriteCmd(0x30);  
}




void set_Cursor(uchar x,uchar y)
{
  uchar i;
  switch(x)
  {
  case 0x00:i = 0x80;break;
  case 0x01:i = 0x90;break;
  case 0x02:i = 0x88;break;
  case 0x03:i = 0x98;break;
  default:break;
  }
  i = y+i;
  LCD_WriteCmd(i);
}

void LCD_DisplayFont(uchar *CGRAM_ZIKU,uint8_t t)
{
  uchar i,add;
  LCD_WriteCmd(0x34);
  LCD_WriteCmd(0x02);
  LCD_WriteCmd(0x30);
  add=(0x40+(t<<4));
  for(i=0;i<16;i++)
  {
    LCD_WriteCmd(add+i);
    LCD_WriteDat(CGRAM_ZIKU[i*2]);
    LCD_WriteDat(CGRAM_ZIKU[i*2+1]);
  }

}


void disp_CGRAM(uchar x,uchar y,uchar num)
{       
  set_Cursor(x,y);
  LCD_WriteDat(0x00);
  LCD_WriteDat(num*2);
}



void LCD_RST()
{
  LCD_RST_L;
  HAL_Delay(5);
  LCD_RST_H;
  HAL_Delay(5);
}

void SendByte(uint8_t byte)
{  
  uint8_t i;
  for (i = 0; i < 8; i++)
  {
    if ((byte<<i)&0x80)
    {
      LCD_SID_H;
    }
    else
    {
      LCD_SID_L;
    }
    LCD_CLK_H;
    Delay_us(100);
    LCD_CLK_L;
    Delay_us(100);
  }
}


