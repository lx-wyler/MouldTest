/*
 * oled.h
 *
 * Created: 2021/5/20 23:20:18
 *  Author: liu77
 */ 


#ifndef __OLED_H__
#define __OLED_H__

// 引脚定义操作
#define CLK_H PORTC_OUTSET = 0x01 << 0
#define CLK_L PORTC_OUTCLR = 0x01 << 0
#define SDA_H PORTC_OUTSET = 0x01 << 1
#define SDA_L PORTC_OUTCLR = 0x01 << 1
#define RES_H PORTC_OUTSET = 0x01 << 2
#define RES_L PORTC_OUTCLR = 0x01 << 2
#define DC_D  PORTC_OUTSET = 0x01 << 3
#define DC_C  PORTC_OUTCLR = 0x01 << 3
#define CS_H  PORTC_OUTSET = 0x01 << 4
#define CS_L  PORTC_OUTCLR = 0x01 << 4



void InitOled();
void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
void OLED_Displaynum6_8(const unsigned int x, const unsigned char y, double num);  //??????×?
void OLED_Clear(void)  ;


#endif /* OLED_H_ */