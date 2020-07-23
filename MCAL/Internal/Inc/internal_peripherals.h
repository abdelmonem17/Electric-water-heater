#ifndef		 __INTERNAL_PERIPHERALS_h__
#define      __INTERNAL_PERIPHERALS_h__

#include "../../../Infrastructure/Inc/mega32_registers.h"

static inline __attribute__((always_inline)) void CPU_sleep(void)
{

	MCU_CTL |=SE_PIN;
	__asm__ __volatile__ ( "sleep" "\n\t" :: );
	MCU_CTL &= (~SE_PIN);
}


#endif
