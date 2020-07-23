#include "../Inc/seven_segement.h"
#include "../../../MCAL/Gpio/Inc/gpio.h"




void sevenSegementInit(SevenSegementCfg_Str *cfg)
{
	GpioPortDirection(cfg->segementPort,OUTPUT);
	GpioPinDirection(cfg->ctlPort,cfg->ctlPin,OUTPUT);
}


void sevenSegementsDisplay(SevenSegementData_Str *displayData)
{
	char BCD[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};
	switch(displayData->anodeOrCathod)
	{
	case ANODE :
		switch(displayData->state)
		{
		case CTL_ON :
			GpioPortWrite(displayData->segementPort , BCD[displayData->diplayedNum] );
			/*set the ctl pin*/
			GpioPinWrite_Same_Value(displayData->ctlPort , displayData->ctlPin , HIGH);


			break;
		default :
			GpioPinWrite_Same_Value(displayData->ctlPort , displayData->ctlPin , LOW);
			GpioPortWrite(displayData->segementPort , LOW );
			break;
		}
		break;

		default :

			switch(displayData->state)
			{
			case CTL_ON :
				GpioPortWrite(displayData->segementPort , ( ~ BCD[displayData->diplayedNum] )  );
				/*clear the ctl pin*/
				GpioPinWrite_Same_Value(displayData->ctlPort , (displayData->ctlPin) , LOW);
				break;
			default :
				GpioPinWrite_Same_Value(displayData->ctlPort , (displayData->ctlPin ) , HIGH);
				break;
			}
			break;
	}

}
