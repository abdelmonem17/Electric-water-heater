/****************************************************************************
 * @file  TMU_LCfg.h
 * @brief this file includes sos configurations
 *
 * @author Abdelmonem Mostafa
 * @date  OCT 2019
 *
 ****************************************************************************/


#ifndef TMU_LCONG_H_
#define TMU_LCONG_H_
#include "../../../MCAL/Timer/Inc/timer.h"
#include "../../../Infrastructure/Inc/types.h"



/************************************************************************/
/*			typedef for structs                                               */
/************************************************************************/
typedef struct  
{
	uint8 channel;
	uint16 resultion_ms;
}Sos_ConfigType;


extern Sos_ConfigType Sos_conf;




#endif /* TMU_LCONG_H_ */
