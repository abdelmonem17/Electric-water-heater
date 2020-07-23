
#include "../Inc/cooler.h"
#include "../../TempSensor/Inc/temp_Sensor.h"
#include  "../../../MCAL/Gpio/Inc/gpio.h"


#define COOLER_C	1
#include "../../../App/Inc/water_system_Cfg.h"



void CoolerInit(void)
{
	GpioPinDirection(COOLING_ELEMENT_LED_PORT , COOLING_ELEMENT_LED_PIN,OUTPUT);
	GpioPinDirection(FAN_PORT, FAN_PIN,OUTPUT);
}
void coolerOn(void)
{
	GpioPinWrite_Same_Value(COOLING_ELEMENT_LED_PORT , COOLING_ELEMENT_LED_PIN,HIGH);
	GpioPinWrite_Same_Value(FAN_PORT, FAN_PIN,HIGH);
	setTemperature_Stub(getTemperture() - 1);
}
void coolerOff(void)
{
	GpioPinWrite_Same_Value(COOLING_ELEMENT_LED_PORT , COOLING_ELEMENT_LED_PIN,LOW);
	GpioPinWrite_Same_Value(FAN_PORT, FAN_PIN,LOW);
}
