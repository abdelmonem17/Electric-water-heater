#include "I2c.h"
void I2c_master_initait()
{
	TWSR=0;
	TWBR=0x47;            //SCL =50Khz
	TWCR=(1<<TWEN);


}

uint8 I2c_start_condition(void)
{


	static uint8 state = I2C_SENT;
	if(state == I2C_SENT)
	{
		TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTA);
		state = I2C_WAITING;
	}
	else if ( ( TWCR & (1<<TWINT) ) ) //waiting until data sent
	{
		state = I2C_SENT;
	}
	return state ;
	return 1;
}

uint8 I2c_sending(uint8_t data)
{
	static uint8 state = I2C_SENT;
	if(state == I2C_SENT)
	{
		TWDR=data;
		TWCR=(1<<TWINT)|(1<<TWEN);     //to start sending data
		state = I2C_WAITING;
	}
	else if ( ( TWCR & (1<<TWINT) ) ) //waiting until data sent
	{
		state = I2C_SENT;
	}
	return state ;

}


uint8_t I2c_receive(uint8_t ackStatus, uint8 *pdata)
{
	static uint8 state = I2C_RECEIVED ;

	if(state == I2C_RECEIVED)
	{
		if(ackStatus == NOT_ACK)                   //read only one byte
			TWCR=(1<<TWINT)|(1<<TWEN);
		else                         //read more than one byte
		{			        //start receiving and after that send ACK
			TWCR=(1<<TWEA)|(TWINT);
			TWCR|=(1<<TWINT);
		}
		state = I2C_WAITING;
	}

    if( (state == I2C_WAITING) && (  (TWCR & (1<<TWINT) ) != 0 ) )           //wait for complete
	{
		*pdata = TWDR;
		state = I2C_RECEIVED;
	}

	return state;

}
void I2c_stop_condition()
{
	TWCR=TWCR=(1<<TWINT)|(1<<TWEN)| (1<<TWSTO);     //start send stop condition

}

void I2c_slave_initiat(uint8_t slaveAdress)
{
	slaveAdress&=~(1<<0);
	TWCR=1<<TWEN;
	TWAR=slaveAdress|isGeneralCalling ;

	TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWEA);

}
void I2c_slave_listen()
{
	while( !( TWCR & (1<<TWINT) ) );

}
