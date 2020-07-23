#include "../Inc/temp_Sensor.h"

static uint8  gsu8_temp = 60;

uint8 getTemperture(void)
{
	return gsu8_temp;
}
void  setTemperature_Stub(uint8 temp)
{
	gsu8_temp = temp;
}

