/*
 * TWI.h
 *
 * Created: 1/7/2019 7:43:24 PM
 *  Author: Eng Abdo
 */ 
#ifndef TWI_H_
#define TWI_H_
#include "../../Infrastructure/Inc/types.h"
#include <avr/io.h>

#define  I2C_SENT			1
#define  I2C_WAITING		0

#define I2C_RECEIVED		1


#define 	ACK			0
#define 	NOT_ACK		1

#define isGeneralCalling 1
#define osc 16000000
void I2c_master_initait();
uint8 I2c_start_condition();
void I2c_stop_condition();
void I2c_slave_listen();
void I2c_slave_initiat(uint8_t slaveAdress);

uint8 I2c_sending(uint8 data);
uint8 I2c_receive(uint8 ack, uint8 *pdata);

#endif
