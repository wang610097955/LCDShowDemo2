#include "gpio.h"
#include "stdint.h"
#include "driverprodemo.h"
void write_LCD1602_DB(uint8_t data)
{
	GPIO_Write(GPIOG,(uint16_t)data);
}
 
uint8_t read_LCD1602_DB(void)
{
	uint8_t value;
	value=(uint8_t)GPIO_ReadInputData(GPIOG);
	return value;
}
 
/* 等待液晶准备好 */
void LcdWaitReady()
{
    unsigned char sta;
		delay_us(1);
 
    LCD1602_RS = 0;
		delay_us(1);
 
    LCD1602_RW = 1;
		delay_us(1);
 
    do {
        LCD1602_E = 1;
		delay_us(1);
 
		LCD1602_DB_IO_inputMode();
		sta = read_LCD1602_DB();
		LCD1602_DB_IO_outputMode();	
		delay_us(1);
 
        LCD1602_E = 0;
		delay_us(1);
 
    } while (sta & 0x80); 	//bit7等于1表示液晶正忙，重复检测直到其等于0为止
}
/* 向LCD1602液晶写入一字节命令，cmd-待写入命令值 */
void LcdWriteCmd(unsigned char cmd)
{
		delay_us(1);
 
    LcdWaitReady();
		delay_us(1);
 
    LCD1602_RS = 0;
		delay_us(1);
 
    LCD1602_RW = 0;
		delay_us(1);
 
    write_LCD1602_DB(cmd);
		delay_us(1);
 
    LCD1602_E  = 1;
		delay_us(1);
 
    LCD1602_E  = 0;
		delay_us(1);
 
}
 
/* 向LCD1602液晶写入一字节数据，dat-待写入数据值 */
void LcdWriteDat(unsigned char dat)
{
		delay_us(1);
 
    LcdWaitReady();
		delay_us(1);
 
    LCD1602_RS = 1;
		delay_us(1);
 
    LCD1602_RW = 0;
		delay_us(1);
 
    write_LCD1602_DB(dat);
		delay_us(1);
 
    LCD1602_E  = 1;
		delay_us(1);
 
    LCD1602_E  = 0;
		delay_us(1);
 
}
/* 设置显示RAM起始地址，亦即光标位置，(x,y)-对应屏幕上的字符坐标 */
void LcdSetCursor(unsigned char x, unsigned char y)
{
    unsigned char addr;
    
	if (y >= 2)	   						//说明我们想显示在右半屏
    {
        y -= 2;
        x += 8;							//那么x就得往右挪动8个汉字了
    }
    addr = y*16 + x;					//现在这个是以一个汉字为单位，这里所有所说的汉字为单位的意思是16*16
										//由起始DDRAM地址连续写入字符串				  
										//addr的值是所有的汉字后面的地址，也是汉字为单位，即ddram也是以汉字为单位的
    LcdWriteCmd(0x30); 					//启动DDRAM操作
    LcdWriteCmd(0x80|addr);				//设置ram地址
}
/* 在液晶上显示字符串，(x,y)-对应屏幕上的起始坐标，str-字符串指针 */
void LcdShowStr(unsigned char row, unsigned char column, unsigned char *str)
{
	unsigned char n=0,a;
	unsigned char *fore_str;
    LcdSetCursor(column, row);   		//设置起始地址
    while (*str != '\0')  				//连续写入字符串数据，直到检测到结束符
    {
		
		if((n+1)%2==0)					//奇数地址
		{
			fore_str=str-1;
			if(*fore_str>0x80)
			{
				LcdWriteDat(*str++);
			}
			else if(*str>0x80)
				LcdWriteDat(' ');
			else if(*str<=0x80)
				LcdWriteDat(*str++);
		}			
		else
			LcdWriteDat(*str++);  		//先取str指向的数据，然后str自加1
		n++;
 
		if((n+2*column)%16==0)
		{
			a=(n+2*column)/16;
			if(row==0)
			{
			if(a==1)
				LcdWriteCmd(0x90);
			if(a==2)
				LcdWriteCmd(0x88);
			if(a==3)
				LcdWriteCmd(0x98);
			if(a==4)
				LcdWriteCmd(0x80);
			}
 
			if(row==1)
			{
			if(a==1)
				LcdWriteCmd(0x88);
			if(a==2)
				LcdWriteCmd(0x98);
			if(a==3)
				LcdWriteCmd(0x80);
			if(a==4)
				LcdWriteCmd(0x90);
			}	
 
			if(row==2)
			{
			if(a==1)
				LcdWriteCmd(0x98);
			if(a==2)
				LcdWriteCmd(0x80);
			if(a==3)
				LcdWriteCmd(0x90);
			if(a==4)
				LcdWriteCmd(0x88);
			}	
 
			if(row==3)
			{
			if(a==1)
				LcdWriteCmd(0x80);
			if(a==2)
				LcdWriteCmd(0x90);
			if(a==3)
				LcdWriteCmd(0x88);
			if(a==4)
				LcdWriteCmd(0x98);
			}	
 
 
	
		}
    }
}/* 初始化1602液晶 */
void InitLcd1602()
{
    LcdWriteCmd(0x38);  //16*2显示，5*7点阵，8位数据接口
    LcdWriteCmd(0x0C);  //显示器开，光标关闭
    LcdWriteCmd(0x06);  //文字不动，地址自动+1
    LcdWriteCmd(0x01);  //清屏
}
 
 
 
 
/*
* 函数名：LcdShowImage
* 描  述：在显示屏上显示一幅图像
* 输  入：img - 待显示图像指针
*         x - 屏幕显示横坐标(以像素为单位)
*         y - 屏幕显示纵坐标(以像素为单位)
*         w - 图像宽度(以像素为单位)
*         h - 图像高度(以像素为单位)
* 输  出：无
* 备  注：x与w必须是16的整数倍，因CGRAM最小寻址单位为2字节；y与h可为0-63的任意值。
*/
 
//经过我的不断测试，y和h的值也是可以取到64，而且显示汉字，也得取到16而不是15，那么来说，是多少像素高度，这里就填多少
void LcdShowImage(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t *img)
{
    uint16_t i;
    uint8_t xi,yi;
    uint8_t xt,yt;
 
    x >>= 4;
    w >>= 3;
    i = 0;
    LcdWriteCmd(0x36); //启动CGRAM操作
	
    for (yi=0; yi<h; yi++)
    {
        yt = y+yi;
        xt = x;
        if (yt >= 32)
        {
            yt -= 32;
            xt += 8;
        }
        LcdWriteCmd(0x80|yt);
        LcdWriteCmd(0x80|xt);
        for (xi=0; xi<w; xi++)
        {
            LcdWriteDat(img[i++]);
        }
    }
}
 
/*
* 函数名：LcdClearArea
* 描  述：清除屏幕上的一块图形显示区域
* 输  入：x - 区域起始横坐标(以像素为单位)
*         y - 区域起始纵坐标(以像素为单位)
*         w - 区域宽度(以像素为单位)
*         h - 区域高度(以像素为单位)
* 输  出：无
* 备  注：x与w必须是16的整数倍，因CGRAM最小寻址单位为2字节；y与h可为0-63的任意值。
*/
 
 
//记住，绘图模式时候屏幕默认有乱点显示的数据的，需要我们先调用这个函数后面才能正常显示
 
void LcdClearArea(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    uint8_t xi,yi;
    uint8_t xt,yt;
 
    x >>= 4;
    w >>= 3;
    LcdWriteCmd(0x36); 			//启动CGRAM操作
    for (yi=0; yi<h; yi++)
    {
        yt = y+yi;
        xt = x;
        if (yt >= 32)
        {
            yt -= 32;
            xt += 8;
        }
        LcdWriteCmd(0x80|yt);
        LcdWriteCmd(0x80|xt);
        for (xi=0; xi<w; xi++)
        {
            LcdWriteDat(0x00); 	//写入0x00以清除显示
        }
    }
}
