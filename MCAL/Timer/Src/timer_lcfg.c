#include "../Inc/timer_lcfg.h"
#include "../../../Infrastructure/Inc/general_cfg.h"


#define INTERRUPT_TIME_US		1000UL


Timer_cfg_S timer0_cfg={CH0,TIMER,INTERRUPT_TIME_US,FR_CPU,INTERRUPT};

