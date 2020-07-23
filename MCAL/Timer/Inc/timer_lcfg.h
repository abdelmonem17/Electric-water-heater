
#ifndef TIMER_LCFG_H_
#define TIMER_LCFG_H_
#include "../../../Infrastructure/Inc/types.h"



typedef enum
{
	false,true
}BOOL_E;


typedef enum
{
	CH0,CH1,CH2
}Timer_ch_E;

typedef enum {
	TIMER,COUNTER_RISING,COUNTER_FALLING
	
}Timer_Mode_E;

typedef enum {
	POLLING,INTERRUPT
	
}Timer_Interrupt_E;
typedef struct
{
	Timer_ch_E ch;
	Timer_Mode_E mode;
	uint32  resolution_us;
	uint32  CPU_clk;
	Timer_Interrupt_E interrupt;
}Timer_cfg_S;



extern Timer_cfg_S timer0_cfg;

#endif /* TIMER_LCFG_H_ */
