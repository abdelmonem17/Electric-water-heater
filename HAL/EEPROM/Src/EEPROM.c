

#include "../Inc/EEPROM.h"
#include "../../../MCAL/I2c/I2c.h"



inline void EEPROM_init()
{
	I2c_master_initait();

}



I2cStatus_t EEPROM_write(uint8_t address,uint16 location,char data)
{
	static I2cStatus_t EEPROMWriteState = EEPROM_WRITING_START;

	switch(EEPROMWriteState)
	{
	case EEPROM_WRITING_COMPETED :
	case EEPROM_WRITING_START    :
		if( I2c_start_condition() == I2C_SENT)
		{
			EEPROMWriteState = EEPROM_WRINING_ADDRESS_TO_WRITE;
		}
		else
		{
			EEPROMWriteState = EEPROM_WRITING_START;
		}
		break;
	case EEPROM_WRINING_ADDRESS_TO_WRITE :
		if( I2c_sending( (address << 1) )  == I2C_SENT)
		{
			EEPROMWriteState = EEPROM_WRINING_LOCATION;
		}
		else
		{

		}
		break;
	case EEPROM_WRINING_LOCATION :
		if( I2c_sending( location ) == I2C_SENT)
		{
			EEPROMWriteState = EEPROM_WRINING_BYTE;
		}
		else
		{

		}
		break;
	case EEPROM_WRINING_BYTE :
		if( I2c_sending(data) == I2C_SENT)
		{
			EEPROMWriteState = EEPROM_WRITING_COMPETED;
			I2c_stop_condition();


		}
		else
		{

		}
		break;
	}



	//TWI_sending( (location>>8) );


	return EEPROMWriteState;


}

I2cStatus_t EEPROM_read(uint8_t address, uint16_t location, uint8 *pByte)
{


	static I2cStatus_t EEPROMReadingState = EEPROM_WRITING_START;

	switch(EEPROMReadingState)
	{
	case EEPROM_WRITING_COMPETED :
	case EEPROM_WRITING_START    :
		if( I2c_start_condition() == I2C_SENT)
		{
			EEPROMReadingState = EEPROM_WRINING_ADDRESS_TO_WRITE;
		}
		else
		{
			EEPROMReadingState = EEPROM_WRITING_START;
		}
		break;
	case EEPROM_WRINING_ADDRESS_TO_WRITE :
		if( I2c_sending( (address << 1) )  == I2C_SENT)
		{
			EEPROMReadingState = EEPROM_WRINING_LOCATION;
		}
		else
		{

		}
		break;
	case EEPROM_WRINING_LOCATION :
		if( I2c_sending( location ) == I2C_SENT)
		{
			EEPROMReadingState = EEPROM_WRITING_REP_START;
		}
		else
		{

		}
		break;
	case EEPROM_WRITING_REP_START  :
		if( I2c_start_condition() == I2C_SENT)
		{
			EEPROMReadingState = EEPROM_WRINING_ADDRESS_TO_READ;
		}
		else
		{

		}
		break;
	case EEPROM_WRINING_ADDRESS_TO_READ :
		if( I2c_sending( (  (address<<1) | 0x01)  ) == I2C_SENT)
		{
			EEPROMReadingState = EEPROM_READING_BYTE;

		}
		else
		{

		}
		break;
	case EEPROM_READING_BYTE :
		if(  I2c_receive(NOT_ACK,pByte)   == I2C_RECEIVED)
		{
			EEPROMReadingState = EEPROM_READING_COMPETED;
			//I2c_stop_condition();

		}
		else
		{

		}
		break;
	}



	//TWI_sending( (location>>8) );










	return EEPROMReadingState;
}


