/*
 * MouldTest.c
 *
 * Created: 2021/5/20 23:16:53
 *  Author: liu77
 */ 


#include "avr_compiler.h"
#include "clksys_driver.h"

#include "oled.h"



void PLL_XOSC_Initial();



int main(void)
{
	int i=0;
	PLL_XOSC_Initial();
	
	InitOled();
	OLED_Clear();
	OLED_Displaynum6_8(0, 3, -32164.1);
	OLED_P6x8Str(0, 1, "This is my nth OLED program.");
	
	
    while(1)
    {
		OLED_Displaynum6_8(0, 0, i++);
        _delay_ms(1000);
		if(i == 11){
			i = 1;
			OLED_P6x8Str(0, 0, "  ");
		}
	}
}



void PLL_XOSC_Initial()
{
	unsigned char factor = 6;
	CLKSYS_XOSC_Config( OSC_FRQRANGE_2TO9_gc, false,OSC_XOSCSEL_XTAL_16KCLK_gc );//设置晶振范围 启动时间
	CLKSYS_Enable( OSC_XOSCEN_bm  );//使能外部振荡器
	do {} while ( CLKSYS_IsReady( OSC_XOSCRDY_bm  ) == 0 );//等待外部振荡器准备好
	CLKSYS_PLL_Config( OSC_PLLSRC_XOSC_gc, factor );//设置倍频因子并选择外部振荡器为PLL参考时钟
	CLKSYS_Enable( OSC_PLLEN_bm );//使能PLL电路
	do {} while ( CLKSYS_IsReady( OSC_PLLRDY_bm ) == 0 );//等待PLL准备好
	CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_PLL_gc);//选择系统时钟源
	CLKSYS_Prescalers_Config( CLK_PSADIV_1_gc, CLK_PSBCDIV_1_1_gc );//设置预分频器A,B,C的值
}