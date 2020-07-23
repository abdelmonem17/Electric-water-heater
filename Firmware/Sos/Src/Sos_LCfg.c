/*
 * TMU_LCfg.c
 *
 * Created: 11/16/2019 9:46:58 PM
 *  Author: Eng Abdo
 */ 
#include "../Inc/Sos_LCfg.h"
#include "../../../MCAL/Timer/Inc/timer.h"


/************************************************************************/
/*   TMU configuration parameters                                                                     */
/************************************************************************/
#define MS_TO_US			1000
#define RESLUTION_MS		1
#define TMU_TIMER_CHANNEL	CH0


Sos_ConfigType Sos_conf =	{TMU_TIMER_CHANNEL,(RESLUTION_MS * MS_TO_US) };

