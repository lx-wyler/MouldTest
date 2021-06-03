#include "avr_compiler.h"
//#include "lcd_display.h"
//#include "color.h"

#define sign           0x10
#define nosign         0x00
#define result12right  0x00
#define result8right   0x04
#define result12left   0x06
#define _1V            0x00
#define _VCC           0x10
#define REFPORTA          0x20
#define REFPORTB          0x30
#define prescaler4     0x00
#define prescaler8     0x01
#define prescaler16    0x02
#define prescaler32    0x03
#define prescaler64    0x04
#define prescaler128   0x05
#define prescaler256   0x06
#define prescaler512   0x07
#define inputmod_int   0x00
#define inputmod_sin   0x01
#define inputmod_dif   0x02       //有符号时可选
#define inputmod_dif_gain  0x03   //有符号时可选
#define gain1          0x00
#define gain2          0x04
#define gain4          0x08
#define gain8          0x0c
#define gain16         0x10
#define gain32         0x14
#define gain64         0x18
#define temp           0x00       //模式设置为内部输入时有效
#define bandgap        0x08
#define scaledvcc      0x10
#define dac            0x18
#define PIN0_          0x00
#define PIN1_          0x08
#define PIN2_          0x10
#define PIN3_          0x18
#define PIN4_          0x20
#define PIN5_          0x28
#define PIN6_          0x30
#define PIN7_          0x38
#define _PIN0          0x00      //Differential without gain
#define _PIN1          0x01
#define _PIN2          0x02
#define _PIN3          0x03
#define _PIN4          0x00      //Differential with gain
#define _PIN5          0x01
#define _PIN6          0x02
#define _PIN7          0x03

#define COMMON_MODE_CYCLES  16

#define ADCA_Result_Config( _sign, _result )   ( ADCA_CTRLB |= (_sign) | (_result) )
#define ADCA_Reference_Config( _reference )    ( ADCA_REFCTRL |= (_reference) )
#define ADCA_Prescaler_Config( _prescaler )    ( ADCA_PRESCALER = (_prescaler) )
#define ADCA_Enable()   ( ADCA_CTRLA |= 0x01 )
#define ADCA_Disable()  ( ADCA_CTRLA &= ~0x01 )

#define ADCA_Ch0_InputMode_and_Gain_Config( _inputmod, _gain )     \
        ( ADCA_CH0_CTRL |= (_inputmod) | (_gain) )
#define ADCA_Ch0_InputMux_Config( _posInput, _negInput )      \
        ( ADCA_CH0_MUXCTRL = (_posInput) | (_negInput) )
#define ADCA_Ch0_Interrupts_Config( _interruptMode, _interruptLevel)  \
        ( ADCA_CH0_INTCTRL = (_interruptMode) | (_interruptLevel) )
#define ADCA_Ch0_Conversion_Start()     ( ADCA_CH0_CTRL |= 0x80 )
#define ADCA_Ch0_Conversion_Complete()  ( ADCA_CH0_INTFLAGS & 0x01 )
#define ADCA_Ch0_Clear_Flag()    ( ADCA_CH0_INTFLAGS = 0x01 )

#define ADCA_Ch1_InputMode_and_Gain_Config( _inputmod, _gain )     \
        ( ADCA_CH1_CTRL |= (_inputmod) | (_gain) )
#define ADCA_Ch1_InputMux_Config( _posInput, _negInput )      \
        ( ADCA_CH1_MUXCTRL = (_posInput) | (_negInput) )
#define ADCA_Ch1_Interrupts_Config( _interruptMode, _interruptLevel)  \
        ( ADCA_CH1_INTCTRL = (_interruptMode) | (_interruptLevel) )
#define ADCA_Ch1_Conversion_Start()     ( ADCA_CH1_CTRL |= 0x80 )
#define ADCA_Ch1_Conversion_Complete()  ( ADCA_CH1_INTFLAGS & 0x01 )
#define ADCA_Ch1_Clear_Flag()    ( ADCA_CH1_INTFLAGS = 0x01 )

#define ADCA_Ch2_InputMode_and_Gain_Config( _inputmod, _gain )     \
        ( ADCA_CH2_CTRL |= (_inputmod) | (_gain) )
#define ADCA_Ch2_InputMux_Config( _posInput, _negInput )      \
        ( ADCA_CH2_MUXCTRL = (_posInput) | (_negInput) )
#define ADCA_Ch2_Interrupts_Config( _interruptMode, _interruptLevel)  \
        ( ADCA_CH2_INTCTRL = (_interruptMode) | (_interruptLevel) )
#define ADCA_Ch2_Conversion_Start()     ( ADCA_CH2_CTRL |= 0x80 )
#define ADCA_Ch2_Conversion_Complete()  ( ADCA_CH2_INTFLAGS & 0x01 )
#define ADCA_Ch2_Clear_Flag()    ( ADCA_CH2_INTFLAGS = 0x01 )

#define ADCA_Ch3_InputMode_and_Gain_Config( _inputmod, _gain )     \
        ( ADCA_CH3_CTRL |= (_inputmod) | (_gain) )
#define ADCA_Ch3_InputMux_Config( _posInput, _negInput )      \
        ( ADCA_CH3_MUXCTRL = (_posInput) | (_negInput) )
#define ADCA_Ch3_Interrupts_Config( _interruptMode, _interruptLevel)  \
        ( ADCA_CH3_INTCTRL = (_interruptMode) | (_interruptLevel) )
#define ADCA_Ch3_Conversion_Start()     ( ADCA_CH3_CTRL |= 0x80 )
#define ADCA_Ch3_Conversion_Complete()  ( ADCA_CH3_INTFLAGS & 0x01 )
#define ADCA_Ch3_Clear_Flag()    ( ADCA_CH3_INTFLAGS = 0x01 )

#define ADCA_FreeRunning_Enable()    ( ADCA_CTRLB |= 0x08 )
#define ADCA_FreeRunning_Disable()   ( ADCA_CTRLB &= ~0x08 )
#define ADCA_TempReference_Enable()  ( ADCA_REFCTRL |= 0x01)
#define ADCA_TempReference_Disable() ( ADCA_REFCTRL &= ~0x01 )

static uint8_t SP_ReadCalibrationByte( uint8_t index )
{
	uint8_t result;
	NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc;
 	result = pgm_read_byte(index);
 	NVM_CMD = NVM_CMD_NO_OPERATION_gc;
	return result;
}
void Init_Adc(void)
{
    uint8_t calh;
	uint8_t call;
	calh = SP_ReadCalibrationByte( 0x21 );
	call = SP_ReadCalibrationByte( 0x20 );
    ADCA_CAL = calh << 8 | call ;                 //校准值
	ADCA_Prescaler_Config( prescaler32 );          //时钟分频
	//ADCA_Reference_Config( REFPORTB);                 //基准源  
//	ADCA_Reference_Config( _VCC );                 //基准源  
    //ADCA_REFCTRL|=0x01;
	ADCA_REFCTRL = 0x31;
//	ADCA_Result_Config( nosign, result8right );   //设置转换结果 
    ADCA_Result_Config( nosign, result12right );
	                  
	ADCA_Ch0_InputMode_and_Gain_Config( inputmod_sin, 0 );    //  单端输入
    ADCA_Ch0_InputMux_Config( PIN0_, 0);         //通道选择
//	ADCA_Ch0_Interrupts_Config( 0, 0x01 );      //中断使能

	ADCA_Ch1_InputMode_and_Gain_Config( inputmod_sin, 0 );    //  单端输入
    ADCA_Ch1_InputMux_Config( PIN1_, 0);         //通道选择
//	ADCA_Ch1_Interrupts_Config( 0, 0x01 );      //中断使能

	ADCA_Ch2_InputMode_and_Gain_Config( inputmod_sin, 0 );    //  单端输入
    ADCA_Ch2_InputMux_Config(PIN2_, 0);         //通道选择
	//ADCA_Ch2_Interrupts_Config( 0, 0x01 );      //中断使能

	ADCA_Ch3_InputMode_and_Gain_Config( inputmod_sin, 0 );    //  单端输入
    ADCA_Ch3_InputMux_Config( PIN3_, 0);         //通道选择
	//ADCA_Ch3_Interrupts_Config( 0, 0x01 );      //中断使能

	ADCA_Enable();                                 //ADC使能 
}
unsigned int Read_Adc( uint16_t channel )
{
    switch( channel )    
	{
	    case 1: ADCA_CH0_CTRL |= 0x80;
	            while(!ADCA_CH0_INTFLAGS);
	         //   ADCA_CH0_INTFLAGS = 0x01;
	            return ADCA_CH0_RES;
				break;
	    case 2: ADCA_CH1_CTRL |= 0x80;
	            while(!ADCA_CH1_INTFLAGS);
	           // ADCA_CH1_INTFLAGS = 0x01;
	            return ADCA_CH1_RES;
				break;
	    case 3: ADCA_CH2_CTRL |= 0x80;
	            while(!ADCA_CH2_INTFLAGS);
	        //    ADCA_CH2_INTFLAGS = 0x01;
	            return ADCA_CH2_RES;
				break;
	    case 4: ADCA_CH3_CTRL |= 0x80;
	            while(!ADCA_CH3_INTFLAGS);
	          //  ADCA_CH3_INTFLAGS = 0x01;
	            return ADCA_CH3_RES;
				break;
	}
}
uint8_t Read_Tem(void)
{
	return 0;
}
/*
void ADC_Display_Init(void)
{
    DisplayString_Char( ADD, "     ADC", 0, 0, WHITE, BLACK );
	DisplayString_Char( ADD, "Channel 1:", 0, 1, WHITE, BLACK );
	DisplayString_Char( ADD, "      -.---V", 0, 2, WHITE, BLACK );
	DisplayString_Char( ADD, "Channel 2:", 0, 3, WHITE, BLACK );
	DisplayString_Char( ADD, "      -.---V", 0, 4, WHITE, BLACK );
	DisplayString_Char( ADD, "CHannel 3:", 0, 5, WHITE, BLACK );
	DisplayString_Char( ADD, "      -.---V", 0, 6, WHITE, BLACK );
	DisplayString_Char( ADD, "Channel 4:", 0, 7, WHITE, BLACK );
	DisplayString_Char( ADD, "      -.---V", 0, 8, WHITE, BLACK );
}*/
