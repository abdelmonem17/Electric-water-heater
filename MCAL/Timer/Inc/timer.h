 


#ifndef TIMER_H_
#define TIMER_H_
#include "../../Gpio/Inc/gpio.h"
#include "timer_cfg.h"
#include "timer_lcfg.h"


#define TIMER0_COMP_ISR_INDEX	0

typedef	void(*pFun)(void);
extern volatile uint16 timer0_ticks;



/****************************************************************************
*
*   @brief function inits timer
*   this function takes  timer config
*   @params  name : timer_conf
*			 type :Timer_cfg_S
*
*   @return uint8
****************************************************************************/
uint8 Timer_Init(Timer_cfg_S *);

/****************************************************************************
*
*   @brief function starts timer
*   this function takes  timer channel
*   @params  name : ch
*			 type :Timer_ch_E
*
*   @return uint8
****************************************************************************/
void Timer_Start(Timer_ch_E);

/****************************************************************************
*
*   @brief function stops timer
*   this function takes  timer channel
*   @params  name : ch
*			 type :Timer_ch_E
*
*   @return uint8
****************************************************************************/
void Timer_Stop(Timer_ch_E);

/****************************************************************************
*
*   @brief function sets timer interrupt time
*   this function takes  timer channel and timer channel
*   @params  name : ch
*			 type :	Timer_ch_E
*   @params  name : counts_us
*			 type :	uint16
*
*   @return uint8
****************************************************************************/
uint8 Timer_set_counter(Timer_ch_E,uint16);

/****************************************************************************
*
*   @brief function gets timer status
*   this function takes  timer channel
*   @params  name : ch
*			 type :Timer_ch_E
*
*   @return BOOL_E
****************************************************************************/
BOOL_E Timer_GetStatus(Timer_ch_E);

static inline __attribute__((always_inline)) uint8 timerGetChannel(Timer_cfg_S *cfg)
{
	return cfg->ch ;
}
static inline __attribute__((always_inline)) void timerSetChannel(Timer_cfg_S *cfg,uint8 ch)
{
	cfg->ch = ch;
}
static inline __attribute__((always_inline)) void timerSetResolution(Timer_cfg_S *cfg,uint16 resolution_us)
{
	cfg->resolution_us = resolution_us;
}

void timer_SetCallBack(Timer_ch_E ch, pFun pf );




#endif /* TIMER_H_ */
