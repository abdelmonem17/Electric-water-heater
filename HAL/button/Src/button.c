
#include "../Inc/button.h"
#include "../../../MCAL/Gpio/Inc/gpio.h"

BtnState_t btnGetState(uint8 port ,uint8 pin ,uint8 *lastBtnLvl)
{
	BtnState_t currentButtonState;

	uint8 currentBtnLvl = GpioPinRead(port,pin);

	if((*lastBtnLvl == LOW)  && (currentBtnLvl == HIGH) )
	{
		 currentButtonState = BTN_RISING;
	}
	else if((*lastBtnLvl == HIGH)  && (currentBtnLvl == LOW) )
	{
		currentButtonState	= BTN_FALLING;
	}
	else
	{
		currentButtonState = BTN_NO_CHANGE;
	}
    *lastBtnLvl = currentBtnLvl;
	return currentButtonState;
}

void BtnsInit(uint8 port , uint8 pins)
{
	GpioPinDirection(port,pins,INPUT);
}
