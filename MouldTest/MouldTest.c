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
	CLKSYS_XOSC_Config( OSC_FRQRANGE_2TO9_gc, false,OSC_XOSCSEL_XTAL_16KCLK_gc );//���þ���Χ ����ʱ��
	CLKSYS_Enable( OSC_XOSCEN_bm  );//ʹ���ⲿ����
	do {} while ( CLKSYS_IsReady( OSC_XOSCRDY_bm  ) == 0 );//�ȴ��ⲿ����׼����
	CLKSYS_PLL_Config( OSC_PLLSRC_XOSC_gc, factor );//���ñ�Ƶ���Ӳ�ѡ���ⲿ����ΪPLL�ο�ʱ��
	CLKSYS_Enable( OSC_PLLEN_bm );//ʹ��PLL��·
	do {} while ( CLKSYS_IsReady( OSC_PLLRDY_bm ) == 0 );//�ȴ�PLL׼����
	CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_PLL_gc);//ѡ��ϵͳʱ��Դ
	CLKSYS_Prescalers_Config( CLK_PSADIV_1_gc, CLK_PSBCDIV_1_1_gc );//����Ԥ��Ƶ��A,B,C��ֵ
}