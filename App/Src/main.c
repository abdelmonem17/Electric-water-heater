
#include "../../Firmware/Sos/Inc/Sos.h"
#include "../Inc/water_system.h"




int main(void)
{

	Sos_Init(&Sos_conf);
	systemInit();
	Sos_Run();
	return 1;


}



