
#include "../Inc/heater.h"
#include  "../../../MCAL/Gpio/Inc/gpio.h"
#include "../../TempSensor/Inc/temp_Sensor.h"

#define	HEATER_C	1
#include "../../../App/Inc/water_system_Cfg.h"



void heaterInit(void)
{
	GpioPinDirection(HEATER_ELEMEENT_LED_PORT , HEATER_ELEMEENT_LED_PIN,OUTPUT);
	GpioPinDirection(HEATER_PORT, HEATER_PIN,OUTPUT);
}
void heaterOn(void)
{

	GpioPinWrite_Same_Value(HEATER_PORT, HEATER_PIN,HIGH);
	setTemperature_Stub(getTemperture() + 1);
}
void heaterOff(void)
{
	GpioPinWrite_Same_Value(HEATER_ELEMEENT_LED_PORT , HEATER_ELEMEENT_LED_PIN,LOW);
	GpioPinWrite_Same_Value(HEATER_PORT, HEATER_PIN,LOW);
}

void heaterELementLed(void)
{
	GpioPinToggle(HEATER_ELEMEENT_LED_PORT , HEATER_ELEMEENT_LED_PIN);

}
