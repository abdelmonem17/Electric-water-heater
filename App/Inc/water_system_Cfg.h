#ifndef	 __WATER_SYSTEM_CFG_H__
#define  __WATER_SYSTEM_CFG_H__




#if defined (COOLER_C)
/*cooler cfg*/
#define  COOLING_ELEMENT_LED_PORT			PORT_A
#define  COOLING_ELEMENT_LED_PIN			BIT4


#define  FAN_PORT							PORT_A
#define  FAN_PIN							BIT5




#elif  (HEATER_C)
/* heater cfg */
#define  HEATER_ELEMEENT_LED_PORT			PORT_A
#define  HEATER_ELEMEENT_LED_PIN			BIT7


#define  HEATER_PORT						PORT_A
#define  HEATER_PIN							BIT6




#elif  (WATER_SYSTEM_C)

/* up down power CFG */
#define BTNS_UP_DOWN_PORT						PORT_A
#define BTN_ON_OFF_PORT							PORT_A


#define Btn_t		   							 EN_bits
#define	BTN_UP    								 (Btn_t)BIT0
#define	BTN_DOWN  								 (Btn_t)BIT2
#define	BTN_ON_OFF 								 (Btn_t)BIT1





#define SEVEN_SEGEMENT_LEFT_DATA_PORT	    PORT_B
#define SEVEN_SEGEMENT_LEFT_CTL_PORT	    PORT_C
#define SEVEN_SEGEMENT_LEFT_CTL_PIN  	    BIT6

#define SEVEN_SEGEMENT_RIGHT_DATA_PORT	    PORT_B
#define SEVEN_SEGEMENT_RIGHT_CTL_PORT	    PORT_C
#define SEVEN_SEGEMENT_RIGHT_CTL_PIN  	    BIT7


#define EEPROM_SLAVE_ADDRESS				0b1010000
#define EEPROM_TEMP_LOCATION				0x16


#define TEMP_MIN							35
#define TEMP_MAX							75
#define TEMP_INC_DEC_STEP					5

#define TEMP_CURRENT_INIT					60


#define TEMP_READING_COUNTS 				10
#define TEMP_MIN_DIFFERENCE					5

#define SEVEN_SEG_DISPLAYING_MODE_RES 		1
#define SEVEN_SEG_SETTING_MODE_RES 			(1000/TASK_7SEGEMENT_PERIODICITY)
#define SEVEN_SEG_DISPLAY_OFF_MODE_RES 		0

#define TASK_UP_DOWN_PERIODICITY			151
#define	TASK_UP_DOWN_PRIORITY				4

#define TASK_ON_OFF_PERIODICITY				201
#define	TASK_ON_OFF_PRIORITY				1

#define TASK_TEMP_PERIODICITY				100
#define	TASK_TEMP_PRIORITY					2

#define TASK_7SEGEMENT_PERIODICITY			20
#define	TASK_7SEGEMENT_PRIORITY				6

#define TASK_EEPROM_PERIODICITY				5
#define	TASK_EEPROM_PRIORITY				2

#define TASK_UP_DOWN_5SEC_COUNTS			(5000/TASK_UP_DOWN_PERIODICITY)



#define	BtnEdgeStatus_t		 			 BtnState_t
#define	PRESSED 			 			(BtnEdgeStatus_t)BTN_RISING
#define	RELEASED  			 			(BtnEdgeStatus_t)BTN_FALLING


#define TEMP_SIMULATION 				0
#define TEMP_REAL_HARDWARE				1
#define TEMP_STATUS  					TEMP_SIMULATION




#define   BTN_ON_OFF_INT_EXT_TYPE	INT_EXT0
#define   BTN_ON_OFF_INT_EXT_EDGE	INT_EXT0_FALLING_EDGE


#endif




#endif
