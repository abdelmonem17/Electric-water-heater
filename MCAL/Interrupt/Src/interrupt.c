/*
 * interrupt.c
 *
 *  Created on: Jul 17, 2020
 *      Author: Eng Abdo
 */


#include "../Inc/interrupt.h"
#include "../../../Infrastructure/Inc/mega32_registers.h"
#include "../../../MCAL/Gpio/Inc/gpio.h"

static IntExt_Fp IntExtPFun[3];

void IntExt_Iint(IntExt_t ExtPin,IntExtEdge_t edge,IntExt_Fp pf)
{
	switch(ExtPin)
	{
	case INT_EXT0:
		GpioPinDirection(INT_EXT0_PORT,INT_EXT0_PIN,INPUT);
		MCU_CTL=( ( MCU_CTL& 0xfC ) |  edge);
		INT_GCTL  |= INT_EXT0_PIN;
		IntExtPFun[0] = pf;
		break;
	default:
		break;
	}
}

INT_HANDLER(INT0_VECT)
{
	IntExtPFun[0]();
}
