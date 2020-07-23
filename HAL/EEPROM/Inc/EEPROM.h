#ifndef _EEPROM_H_
#define _EEPROM_H_
#include "../../../Infrastructure/Inc/types.h"


#define I2cStatus_t							uint8
#define EEPROM_WRITING_COMPETED				0
#define	EEPROM_WRITING_START				1
#define EEPROM_WRINING_ADDRESS_TO_WRITE		2
#define EEPROM_WRINING_LOCATION				3
#define EEPROM_WRINING_BYTE					4


#define EEPROM_READING_COMPETED		   		0
#define	EEPROM_WRITING_REP_START			4
#define EEPROM_WRINING_ADDRESS_TO_READ		5
#define EEPROM_READING_BYTE			    	6



void EEPROM_init();
I2cStatus_t EEPROM_read(uint8 address, uint16 location, uint8 *pByte);
I2cStatus_t EEPROM_write(uint8 address,uint16 location,char data);


#endif
