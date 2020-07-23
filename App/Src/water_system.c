#include "../../Infrastructure/Inc/types.h"
#include "../../HAL/TempSensor/Inc/temp_Sensor.h"
#include "../../HAL/SevenSegement/Inc/seven_segement.h"
#include "../../Firmware/Sos/Inc/Sos.h"
#include "../../HAL/button/Inc/button.h"
#include "../../HAL/cooler/Inc/cooler.h"
#include "../../HAL/heater/Inc/heater.h"
#include "../../HAL/EEPROM/Inc/EEPROM.h"
#include "../Inc/water_system.h"

#include "../../MCAL/Interrupt/Inc/interrupt.h"

#define WATER_SYSTEM_C	1
#include "../Inc/water_system_Cfg.h"





/* local macros*/
#define SystemWorkingMode_t 	uint8
#define	SETTING_MODE			(SystemWorkingMode_t)0
#define	DISPLAYING_MODE			(SystemWorkingMode_t)1
#define	DISPLAY_OFF_MODE    	(SystemWorkingMode_t)2




#define	OperatingMode_t			 	 uint8
#define	OFF_DISPLAY					(OperatingMode_t)0
#define	ON_DISPLAY					(OperatingMode_t)1
#define	POWERING_DISPLAY			(OperatingMode_t)2



#define SevenSegmentType_t		 uint8
#define	SEVEN_SEGMENT_RIGHT		(SevenSegmentType_t)0
#define	SEVEN_SEGMENT_LEFT		(SevenSegmentType_t)1
#define	SEVEN_SEGMENT_OFF		(SevenSegmentType_t)2



#define BtnPoweringStatus_enm				bitLevel_t
#define	BTN_GROUNDED 	 					(BtnPoweringStatus_enm)LOW
#define	BTN_POWERED 	 					(BtnPoweringStatus_enm)HIGH




#define HeatingCoolingStatus_t			uint8
#define	HEATING 		 				(HeatingCoolingStatus_t)1
#define	COOLING  						(HeatingCoolingStatus_t)2
#define	HEATING_OFF_COLLING_OFF 		(HeatingCoolingStatus_t)0



/*global static variables*/
static uint8 gsu8_CurrentTemp;
static SystemWorkingMode_t gsSystemModeEnm_DisplayingMode  ;

static uint8  gsu8_RequiredTemp ;
static OperatingMode_t gsOperatingModeEnm_BtnPowerState ;
static HeatingCoolingStatus_t gsHeatingCoolingStatusEnm_Status ;

static uint16 gsu16_BtnsupDownTaskId;
static uint16 gsu16_BtnOnOffTaskId;
static uint16 gsu16_7segTaskId;
static uint16 gsu16_TempMonitoringTaskId;
static uint16 gsu16_EEPROMTaskId;




#ifdef INTERRUPT
void BtnOnOff_CallBack(void)
{

	switch(gsOperatingModeEnm_BtnPowerState)
	{
	case ON_DISPLAY :
		gsOperatingModeEnm_BtnPowerState = OFF_DISPLAY;
		break;
	case OFF_DISPLAY :
		gsOperatingModeEnm_BtnPowerState = ON_DISPLAY;
		gsSystemModeEnm_DisplayingMode = DISPLAYING_MODE;
		break;
	default :
		break;
	}



}
#endif




/****************************************************************************
 *
 *   @brief function initialize the system
 *   @params  void
 *   @return  void
 ****************************************************************************/
void systemInit(void)
{


	/*global variable init*/
	gsSystemModeEnm_DisplayingMode = DISPLAY_OFF_MODE ;
	gsOperatingModeEnm_BtnPowerState = OFF_DISPLAY;
	gsHeatingCoolingStatusEnm_Status = HEATING_OFF_COLLING_OFF;

	/*hardware componant init*/
	CoolerInit();
	heaterInit();
	EEPROM_init();
	BtnsInit(BTNS_UP_DOWN_PORT, BTN_UP | BTN_DOWN );
	BtnsInit(BTN_ON_OFF_PORT, BTN_ON_OFF );


	SevenSegementCfg_Str leftSegementCfg = {segementPort:SEVEN_SEGEMENT_LEFT_DATA_PORT , ctlPort:SEVEN_SEGEMENT_LEFT_CTL_PORT ,ctlPin : SEVEN_SEGEMENT_LEFT_CTL_PIN };
	sevenSegementInit(& leftSegementCfg);

	SevenSegementCfg_Str rightSegementCfg = {segementPort:SEVEN_SEGEMENT_RIGHT_DATA_PORT , ctlPort:SEVEN_SEGEMENT_RIGHT_CTL_PORT ,ctlPin : SEVEN_SEGEMENT_RIGHT_CTL_PIN };
	sevenSegementInit(& rightSegementCfg);

	uint8 au8_RequiredTemp;
	while( EEPROM_read(EEPROM_SLAVE_ADDRESS,EEPROM_TEMP_LOCATION , &au8_RequiredTemp) != EEPROM_READING_COMPETED );


	/*check the stored set temp and accordingly can check if first time to use EEPROM or not*/
	if( (au8_RequiredTemp > TEMP_MAX ) || (au8_RequiredTemp < TEMP_MIN ) )
	{
		gsu8_RequiredTemp = TEMP_CURRENT_INIT;
	}
	else
	{
		gsu8_RequiredTemp = au8_RequiredTemp;
	}

	/*tasks creation*/
	Sos_Create_Task(btnsUpDownMonitoring_Task,&gsu16_BtnsupDownTaskId,TASK_UP_DOWN_PRIORITY,TASK_UP_DOWN_PERIODICITY,PERIODIC);
	Sos_Create_Task(btnOnOffMonitoring_Task,&gsu16_BtnOnOffTaskId, TASK_ON_OFF_PRIORITY ,TASK_ON_OFF_PERIODICITY,PERIODIC);
	Sos_Create_Task(tempertureMonitoring_Task,&gsu16_TempMonitoringTaskId,TASK_TEMP_PRIORITY, TASK_TEMP_PERIODICITY ,PERIODIC);
	Sos_Create_Task(SevenSegmentDisplay_Task,&gsu16_7segTaskId, TASK_7SEGEMENT_PRIORITY ,TASK_7SEGEMENT_PERIODICITY,PERIODIC);
	Sos_Create_Task(EEPROM_Task,&gsu16_EEPROMTaskId, TASK_EEPROM_PRIORITY ,TASK_EEPROM_PERIODICITY,PERIODIC);


	/*block eeprom task ,waiting to be unblocked*/
	Sos_SetTaskDormant(gsu16_EEPROMTaskId);
}




/****************************************************************************
 *
 *   @brief function acts as task and polls the status of the ON/OFF display button
 *   @params  void
 *
 *
 *   @return  void
 ****************************************************************************/
void btnOnOffMonitoring_Task(void)
{
	static BtnPoweringStatus_enm  sBtnPoweringStatusEnm_status= BTN_GROUNDED;

	/*get button edge */
	BtnEdgeStatus_t aBtnEdgeStatus_status = btnGetState(BTN_ON_OFF_PORT,BTN_ON_OFF ,
			&sBtnPoweringStatusEnm_status);

	/*check for Releasing of the button*/
	if( aBtnEdgeStatus_status == RELEASED)
	{
		/* toggle heating status */
		gsHeatingCoolingStatusEnm_Status = (gsHeatingCoolingStatusEnm_Status != HEATING) ? HEATING : HEATING_OFF_COLLING_OFF;

		/*changing display status*/
		switch(gsOperatingModeEnm_BtnPowerState)
		{
		case ON_DISPLAY :
			gsOperatingModeEnm_BtnPowerState = OFF_DISPLAY;
			break;
		case OFF_DISPLAY :
			gsOperatingModeEnm_BtnPowerState = ON_DISPLAY;
			gsSystemModeEnm_DisplayingMode = DISPLAYING_MODE;


			break;
		default :
			break;
		}


	}
}




/****************************************************************************
 *
 *   @brief function polls up and down buttons to set temperture
 *   @params  void
 *
 *
 *   @return  void
 ****************************************************************************/
void btnsUpDownMonitoring_Task(void)
{


	static uint8 su8_counter = 0;
	static BtnPoweringStatus_enm BtnPoweringStatusEnum_UpBtn = BTN_GROUNDED;
	static BtnPoweringStatus_enm BtnPoweringStatusEnum_DownBtn = BTN_GROUNDED;
	BtnEdgeStatus_t aBtnEdgeStatusEnm_UpBtn = RELEASED;
	static uint8 su8_RequiredTemp;
	BtnState_t aBtnEdgeStatus_DownBtn = RELEASED;


	aBtnEdgeStatusEnm_UpBtn = btnGetState(BTNS_UP_DOWN_PORT, BTN_UP , &BtnPoweringStatusEnum_UpBtn);
	aBtnEdgeStatus_DownBtn = btnGetState(BTNS_UP_DOWN_PORT, BTN_DOWN, &BtnPoweringStatusEnum_DownBtn);




	switch(gsSystemModeEnm_DisplayingMode)
	{
	case DISPLAYING_MODE:

		/*	check for button pressing to go into setting mode	*/
		if( (aBtnEdgeStatusEnm_UpBtn == PRESSED) || (aBtnEdgeStatus_DownBtn == PRESSED))
		{
			gsSystemModeEnm_DisplayingMode = SETTING_MODE;
			su8_counter = 0;

			su8_RequiredTemp = gsu8_RequiredTemp;
			gsu8_RequiredTemp = gsu8_CurrentTemp;

		}
		else
		{

		}
		break;
	case SETTING_MODE:

		/* check for up button press to increment the required temperture */
		if( aBtnEdgeStatusEnm_UpBtn == PRESSED)
		{


			if(gsu8_RequiredTemp <= (TEMP_MAX - TEMP_INC_DEC_STEP) )
			{

				gsu8_RequiredTemp += TEMP_INC_DEC_STEP;

			}
			else
			{

				gsu8_RequiredTemp = TEMP_MAX;
			}
			su8_counter = 0;

		}
		/* check for down button press to increment the required temperture */
		else if ( aBtnEdgeStatus_DownBtn == PRESSED)
		{

			if(gsu8_RequiredTemp >= (TEMP_MIN + TEMP_INC_DEC_STEP) )
			{
				gsu8_RequiredTemp -= TEMP_INC_DEC_STEP;
			}
			else
			{
				/*do nothing*/
				gsu8_RequiredTemp = TEMP_MIN;

			}
			su8_counter = 0;

		}
		else
		{
			/* TASK_UP_DOWN_5SEC_COUNTS   *  current_task_periodicity  = 5 second */
			if(su8_counter < TASK_UP_DOWN_5SEC_COUNTS)
			{
				su8_counter++;
			}
			else
			{
				gsSystemModeEnm_DisplayingMode = DISPLAYING_MODE;

				if(su8_RequiredTemp != gsu8_RequiredTemp)
				{
					/* set request to the eeprom task */
					Sos_SetTaskNotDormant(gsu16_EEPROMTaskId);
				}
				else
				{

				}
			}
		}

		break;
	default:
		/*error option*/

		break;


	}


}



/****************************************************************************
 *
 *   @brief function checks cooling and heating status and accordingly turn the heater
 *    or cooler on or off
 *   @params  void
 *
 *
 *   @return  void
 ****************************************************************************/
/*100ms * 10  = 1s*/
static inline  void __attribute__((always_inline)) CoolingHeatingMonitoring(void)
																																		{
	switch(gsHeatingCoolingStatusEnm_Status)
	{
	case HEATING :
		coolerOff();
		heaterOn();
		heaterELementLed();
		break;
	case COOLING :
		heaterOff();
		coolerOn();
		break;
	default :
		coolerOff();
		heaterOff();
		break;

	}
																																		}



/****************************************************************************
 *
 *   @brief function acts as function polls the state of current temperature
 *   comparing to required one
 *   @params  void
 *
 *   @return  void
 ****************************************************************************/
void tempertureMonitoring_Task(void)
{
	static uint16 su16_TempAvg = 0;
	static uint8 su8_TempCounter = 0;
	static uint8 su8_RequiredTemp = TEMP_CURRENT_INIT;


#if (TEMP_STATUS	== TEMP_REAL_HARDWARE)
	HeatingCoolingStatus_t aheatingCoolingStatusEnm_LastStatus = gsHeatingCoolingStatusEnm_Status;
#endif


	/* to take 10 temp readings */
	if(su8_TempCounter++ < TEMP_READING_COUNTS )
	{
		gsu8_CurrentTemp = getTemperture();
		su16_TempAvg += gsu8_CurrentTemp;
	}
	else
	{
		su16_TempAvg /= TEMP_READING_COUNTS;

		/*detect temp setting request*/
		if( (su8_RequiredTemp != gsu8_RequiredTemp)  && (gsSystemModeEnm_DisplayingMode != SETTING_MODE) )
		{
			su8_RequiredTemp = gsu8_RequiredTemp;
		}
		else
		{
			/*noting*/
		}

		if(su16_TempAvg <= (su8_RequiredTemp - TEMP_MIN_DIFFERENCE) )
		{
			gsHeatingCoolingStatusEnm_Status = HEATING;

		}
		else if (su16_TempAvg >= (su8_RequiredTemp + TEMP_MIN_DIFFERENCE) )
		{
			/* turn cooler on */

			gsHeatingCoolingStatusEnm_Status = COOLING;
		}
		else if(su16_TempAvg == su8_RequiredTemp)
		{
			/*turn off coller and heater*/

			setTemperature_Stub( su8_RequiredTemp );
			gsHeatingCoolingStatusEnm_Status = HEATING_OFF_COLLING_OFF;

		}
		else
		{
			/*nothing*/
		}

#if (TEMP_STATUS	== TEMP_REAL_HARDWARE)
		/*check for any heating cooling change*/
		if(gsHeatingCoolingStatusEnm_Status != aheatingCoolingStatusEnm_LastStatus )
		{
			/*fire request to heater and cooler*/
			//Sos_SetTaskNotDormant(gsu16_CoolingHeatingTaskId);
			aheatingCoolingStatusEnm_LastStatus = gsHeatingCoolingStatusEnm_Status ;
			CoolingHeatingMonitoring();


		}
#else

		CoolingHeatingMonitoring();
#endif
		su16_TempAvg = 0;
		su8_TempCounter = 0;
	}




}


/****************************************************************************
 *
 *   @brief function acts as a task and polls the status of seven segements
 *   @params  void
 *
 *
 *   @return  void
 ****************************************************************************/
void SevenSegmentDisplay_Task(void)
{

	static SevenSegmentType_t sSevenSegementTypeEnm_Display = SEVEN_SEGMENT_OFF;
	static uint8 su8_DisplayingCounter  ;

	static uint8 su8_DisplayingResultion = SEVEN_SEG_DISPLAY_OFF_MODE_RES;
	static SystemWorkingMode_t sSystemModeEnm_LastSystemMode = DISPLAY_OFF_MODE;

	static SevenSegementData_Str segementLeftData = {segementPort:SEVEN_SEGEMENT_LEFT_DATA_PORT ,
			ctlPort:SEVEN_SEGEMENT_LEFT_CTL_PORT,
			ctlPin :SEVEN_SEGEMENT_LEFT_CTL_PIN, anodeOrCathod:ANODE,
	};
	static SevenSegementData_Str segementRightData = {segementPort:SEVEN_SEGEMENT_RIGHT_DATA_PORT ,
			ctlPort:SEVEN_SEGEMENT_RIGHT_CTL_PORT,ctlPin : SEVEN_SEGEMENT_RIGHT_CTL_PIN
			,anodeOrCathod:ANODE
	};

	if(gsOperatingModeEnm_BtnPowerState == ON_DISPLAY)
	{


		switch(gsSystemModeEnm_DisplayingMode)
		{
		case DISPLAYING_MODE :
			segementLeftData.diplayedNum = (uint8)(gsu8_CurrentTemp/10);
			segementRightData.diplayedNum = (uint8)(gsu8_CurrentTemp%10);
			/*check if any change in mode*/
			if(gsSystemModeEnm_DisplayingMode != sSystemModeEnm_LastSystemMode )
			{
				su8_DisplayingResultion = SEVEN_SEG_DISPLAYING_MODE_RES ;
				sSevenSegementTypeEnm_Display = SEVEN_SEGMENT_RIGHT;
				su8_DisplayingCounter = su8_DisplayingResultion;
			}
			else
			{

			}
			break;
		case SETTING_MODE :
			segementLeftData.diplayedNum = (uint8)(gsu8_RequiredTemp/10);
			segementRightData.diplayedNum = (uint8)(gsu8_RequiredTemp%10);
			/*check if any change in mode*/
			if(gsSystemModeEnm_DisplayingMode != sSystemModeEnm_LastSystemMode )
			{

				su8_DisplayingResultion = SEVEN_SEG_SETTING_MODE_RES;
				su8_DisplayingCounter = su8_DisplayingResultion;
			}
			else
			{

			}


			break;
		default:
			//au8_DisplayingResultion = SEVEN_SEG_DISPLAY_OFF_MODE_RES;
			//sSevenSegementTypeEnm_Display = SEVEN_SEGMENT_OFF;
			break;

		}
		sSystemModeEnm_LastSystemMode = gsSystemModeEnm_DisplayingMode;

		/*change senven sgements right and left status according the upove*/
		switch(sSevenSegementTypeEnm_Display )
		{
		case SEVEN_SEGMENT_RIGHT:

			if(su8_DisplayingCounter == su8_DisplayingResultion)
			{
				segementLeftData.state = CTL_OFF;
				segementRightData.state = CTL_ON;
				sevenSegementsDisplay(&segementLeftData);
				sevenSegementsDisplay(&segementRightData);

			}
			else
			{

			}

			if(su8_DisplayingCounter > 0)
			{

				su8_DisplayingCounter--;

			}
			else
			{


				sSevenSegementTypeEnm_Display = SEVEN_SEGMENT_LEFT;
				su8_DisplayingCounter = su8_DisplayingResultion;

			}

			break;
		case  SEVEN_SEGMENT_LEFT:
			if(su8_DisplayingCounter == su8_DisplayingResultion)
			{
				segementRightData.state = CTL_OFF;
				segementLeftData.state = CTL_ON;
				sevenSegementsDisplay(&segementRightData);
				sevenSegementsDisplay(&segementLeftData);

			}
			else
			{

			}

			if(su8_DisplayingCounter > 0)
			{

				su8_DisplayingCounter--;

			}
			else
			{
				sSevenSegementTypeEnm_Display = SEVEN_SEGMENT_RIGHT;
				su8_DisplayingCounter = su8_DisplayingResultion;
			}
			break;
		default:

			break;
		}
	}
	else
	{
		/* turn seven segement display off */
		segementRightData.state = CTL_OFF;
		segementLeftData.state = CTL_OFF;
		sevenSegementsDisplay(&segementRightData);
		sevenSegementsDisplay(&segementLeftData);

	}

}



/****************************************************************************
 *
 *   @brief function acts as a task and polls the eeprom to write on it
 *   @params  void
 *
 *
 *   @return  void
 ****************************************************************************/
void EEPROM_Task(void)
{

	if (EEPROM_write(EEPROM_SLAVE_ADDRESS,EEPROM_TEMP_LOCATION,gsu8_RequiredTemp) == EEPROM_WRITING_COMPETED)
	{
		/* set the current task waiting for anther request for the eeprom*/
		Sos_SetTaskDormant(gsu16_EEPROMTaskId);
	}
	else
	{
		/*nothing*/
	}
}
