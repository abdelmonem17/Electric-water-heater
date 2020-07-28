
#include "../../Firmware/Sos/Inc/Sos.h"
#include "../Inc/water_system.h"
#include "../../MCAL/Interrupt/Inc/interrupt.h"




int main(void)
{

	Sos_Init(&Sos_conf);
	systemInit();
	INT_GLOBAL_FLAG_ENABLE();
	Sos_Run();
	return OK;


}



