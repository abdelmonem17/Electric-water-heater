
#ifndef 	__BUTTON_H__
#define 	__BUTTON_H__
#include "../../../Infrastructure/Inc/types.h"


#define BtnState_t		uint8
#define	BTN_FALLING 	(BtnState_t)0
#define	BTN_RISING		(BtnState_t)1
#define	BTN_NO_CHANGE	(BtnState_t)2
#define	INVALIAD		(BtnState_t)3






BtnState_t btnGetState(uint8 port ,uint8 pin ,uint8 * lastBtnLvl);
void BtnsInit(uint8 port , uint8 pins);

#endif
