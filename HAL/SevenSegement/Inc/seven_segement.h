#ifndef     __SEVEN_SEGMENT_H__
#define		__SEVEN_SEGMENT_H__
#include "../../../Infrastructure/Inc/types.h"

#define 	ctlType	 uint8
#define	 	ANODE	 (ctlType)1
#define		CATHOD   (ctlType)0

#define     CtlState uint8
#define     CTL_ON	 (CtlState)1
#define     CTL_OFF  (CtlState)0


typedef struct
{
	uint8 segementPort		: 3	;
	uint8 ctlPort  			: 3;
	uint8 ctlPin   			   ;
	ctlType anodeOrCathod 	: 1;
	uint8 diplayedNum		: 4;
	CtlState 	state		: 1;
}SevenSegementData_Str;

typedef struct
{
	uint8 segementPort  : 3  ;
	uint8 ctlPort  		: 3;
	uint8 ctlPin   		 ;


}SevenSegementCfg_Str;

void sevenSegementsDisplay(SevenSegementData_Str *displayData);
void sevenSegementInit(SevenSegementCfg_Str *cfg);


#endif
