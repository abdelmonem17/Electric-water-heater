/****************************************************************************
 * @file  Sos.c
 * @brief this file includes some functions implementation of OS
 *
 * @author Abdelmonem Mostafa
 * @date  NOV 25 2019
 *
 ****************************************************************************/

#include "../Inc/Sos.h"
#include "../Inc/Sos_Cfg.h"
#include "../../../MCAL/Timer/Inc/timer.h"
#include "../../../Infrastructure/Inc/mega32_registers.h"
#include "../../../MCAL/Internal/Inc/internal_peripherals.h"


/*- LOCAL MACROS ------------------------------------------*/

#define STATE_DORMANT			0
#define	STATE_NOTDORMANT		1

typedef struct
{

	uint32					next_Excution_Time;
	EnmTMUPeriodicity_t		        shots;
	Sos_Task_t				task;
	uint16 *				id_address;
	uint16					priodicity;
	uint16					priority;
	uint8					state;	

}strTaskProfile_t;

/*- GLOBAL STATIC VARIABLES -------------------------------*/
static uint16 gu16_TaskRepoSize;
static	strTaskProfile_t gStrTaskProfile_TasksRepo	[MAX_TASKS_COUNTS];
static volatile uint32 gu32_Sos_Ticks;
volatile static uint8 gu8_TickFlag;
volatile static uint8 gu8_Sos_TicksOverflowFlag=FALSE;



/*- APIs IMPLEMENTATION -----------------------------------*/

/****************************************************************************
 *
 *   @brief function called in ISR of the timer
 *   this function  counts system ticks and weaks the system from sleep
 *   @params  void
 *   @return void
 ****************************************************************************/



static void Sos_CallTimerBack(void)
{
	if (gu32_Sos_Ticks > (uint16)(gu32_Sos_Ticks + 1)  )
	{
		gu8_Sos_TicksOverflowFlag = TRUE;
		gu32_Sos_Ticks = 0;
	}
	else
	{
		gu32_Sos_Ticks++;
	}

	/*	weak the processor up	*/

}
/****************************************************************************
 *
 *   @brief function initiates Sos
 *   this function  takes Sos configuration and return the enum for error
 *   @params  name : ConfigPtr
 *			 type :	const TMU_ConfigType *
 *   @return EnmTMUError_t
 ****************************************************************************/

EnmSosError_t Sos_Init (const Sos_ConfigType * ConfigPtr )
{
	EnmSosError_t aEnmSosError_Sos_InitStatus = SOS_NO_ERROR;
	gu32_Sos_Ticks= 0;
	gu16_TaskRepoSize=0;
	gu8_TickFlag=0;
	//timer0_cfg.ch= ConfigPtr->channel;
	//timer0_cfg.resolution_us=ConfigPtr->resultion_ms * MS_TO_US;
	timerSetChannel(&timer0_cfg,ConfigPtr->channel);
	timerSetResolution(&timer0_cfg,ConfigPtr->resultion_ms);

	if (Timer_Init(&timer0_cfg) == NOT_OK)
	{
		aEnmSosError_Sos_InitStatus = SOS_ERROR_UNSUPPORTED_TIMER_RESOLUTION;
	}
	else
	{
		Timer_Start(ConfigPtr->channel);


	}
	switch(ConfigPtr->channel)
	{
	/*		assign function to count system ticks for TMU		*/
	case CH0:

		timer_SetCallBack(ConfigPtr->channel, Sos_CallTimerBack );
		break;
	default :
		aEnmSosError_Sos_InitStatus = SOS_ERROR_CHANNEL_UNAVIABLE;
	}

	return aEnmSosError_Sos_InitStatus ;
}

/****************************************************************************
 *
 *   @brief function start calculating delay for a specific functionality
 *   this function  takes Sos configuration and return the enum for error
 *   @params  name : Sos_Job
 *			 type :	Sos_Job_t
 *   @params  name : pTask_Id
 *			 type :	uint16
 *   @params  name : delay_ms
 *			 type :	uint16
 *   @params  name : EnmTMUPeriodicity_t
 *			 type :	periodicity
 *   @return EnmTMUError_t
 ****************************************************************************/
EnmSosError_t Sos_Create_Task(Sos_Task_t SOS_Task ,uint16 *pTask_Id ,uint16 priority,uint16 priodicity,EnmTMUPeriodicity_t shots_state)
{
	EnmSosError_t aEnmSosError_taskCreationStatus = SOS_NO_ERROR;

	if (gu16_TaskRepoSize != MAX_TASKS_COUNTS )
	{


		if (pTask_Id !=NULL && SOS_Task !=NULL)
		{

			*pTask_Id = gu16_TaskRepoSize;
			gStrTaskProfile_TasksRepo[gu16_TaskRepoSize].id_address = pTask_Id;
			gStrTaskProfile_TasksRepo[gu16_TaskRepoSize].task = SOS_Task;
			gStrTaskProfile_TasksRepo[gu16_TaskRepoSize].priority = priority;
			gStrTaskProfile_TasksRepo[gu16_TaskRepoSize].priodicity = priodicity;
			gStrTaskProfile_TasksRepo[gu16_TaskRepoSize].shots = shots_state;
			gStrTaskProfile_TasksRepo[gu16_TaskRepoSize].next_Excution_Time = gu32_Sos_Ticks;  /*make task ready*/
			gStrTaskProfile_TasksRepo[gu16_TaskRepoSize].state	=STATE_NOTDORMANT;
			++gu16_TaskRepoSize;
		}
		else if(pTask_Id == NULL)
		{
			aEnmSosError_taskCreationStatus = TMU_ErrorNullIdPointer;
		}
		else
		{
			aEnmSosError_taskCreationStatus = SOS_ERROR_NULL_TASK_POINTER;
		}
	}
	else
	{
		aEnmSosError_taskCreationStatus = SOS_ERROR_TASK_REPO_FULL;
	}
	return aEnmSosError_taskCreationStatus ;		
}


/****************************************************************************
 *
 *   @brief function initiates Sos
 *   this function  takes job id and stop the timer for this task and return error type
 *   @params  name : au16_job_Id
 *			 type :	uint16
 *   @return EnmTMUError_t
 ****************************************************************************/
EnmSosError_t Sos_Delete_Task(uint16 au16_job_Id )
{
	EnmSosError_t aEnmSosError_taskDeletionStatus = SOS_NO_ERROR;
	/*    overwrite on the job that will be deleted by the last job in the list  */
	if ( (au16_job_Id >=0) && (au16_job_Id < gu16_TaskRepoSize) )
	{
		gStrTaskProfile_TasksRepo[au16_job_Id]=gStrTaskProfile_TasksRepo[--gu16_TaskRepoSize];

		if(gStrTaskProfile_TasksRepo[au16_job_Id].id_address !=NULL)
		{
			*(gStrTaskProfile_TasksRepo[au16_job_Id].id_address) = au16_job_Id;
		}
		else
		{
			/*	unhandled null reference     */
		}


	}
	else
	{
		aEnmSosError_taskDeletionStatus = SOS_ERROR_TASK_NOT_FOUND;
	}
	return aEnmSosError_taskDeletionStatus;
}

EnmSosError_t Sos_SetTaskDormant(uint16 au16_job_Id )
{
	EnmSosError_t aEnmSosError_taskSetDormant = SOS_NO_ERROR;
	if ( (au16_job_Id >=0) && (au16_job_Id < gu16_TaskRepoSize) )
	{
		gStrTaskProfile_TasksRepo[au16_job_Id].state =STATE_DORMANT;


	}
	else
	{
		aEnmSosError_taskSetDormant = SOS_ERROR_TASK_NOT_FOUND;
	}
	return aEnmSosError_taskSetDormant;
}

EnmSosError_t Sos_SetTaskNotDormant(uint16 au16_job_Id )
{
	EnmSosError_t aEnmSosError_taskSetNotDormant = SOS_NO_ERROR;
	if ( (au16_job_Id >=0) && (au16_job_Id < gu16_TaskRepoSize) )
	{
		gStrTaskProfile_TasksRepo[au16_job_Id].state =STATE_NOTDORMANT;

		gStrTaskProfile_TasksRepo[au16_job_Id].next_Excution_Time = gu32_Sos_Ticks	+	gStrTaskProfile_TasksRepo[au16_job_Id].priodicity;
	}
	else
	{
		aEnmSosError_taskSetNotDormant = SOS_ERROR_TASK_NOT_FOUND;
	}
	return aEnmSosError_taskSetNotDormant;
}

/****************************************************************************
 *
 *   @brief function monitor tasks
 *   this function  takes void and return void
 *   @params  void
 *   @return void
 ****************************************************************************/
void Sos_Run(void)
{
	uint8 au8_TaskReadyFlag=FALSE;
	uint16 au16_highestPriorityIndex;
	while (1)
	{

		/*	check in task repo for the highest priority task  */
		for (uint16 au8_TaskIndex=0 ; au8_TaskIndex < gu16_TaskRepoSize ; au8_TaskIndex++ )
		{
			/*		to handle Sos ticks  overflow		*/
			if (gu8_Sos_TicksOverflowFlag == TRUE)
			{
				gStrTaskProfile_TasksRepo[au8_TaskIndex].next_Excution_Time=(uint16)gStrTaskProfile_TasksRepo[au8_TaskIndex].next_Excution_Time;

			}
			else
			{

			}

			/* compare the current task with highest priorty task */
			if (  (gStrTaskProfile_TasksRepo[au8_TaskIndex].next_Excution_Time <=  gu32_Sos_Ticks  ) && (gStrTaskProfile_TasksRepo[au8_TaskIndex].state	== STATE_NOTDORMANT))
			{

				/*check if first task ready */
				if (au8_TaskReadyFlag == FALSE)
				{
					au16_highestPriorityIndex = au8_TaskIndex;
					au8_TaskReadyFlag =TRUE;
				}
				else
				{
					/* check for higher priority task that ready*/
					au16_highestPriorityIndex = gStrTaskProfile_TasksRepo[au16_highestPriorityIndex].priority > gStrTaskProfile_TasksRepo[au8_TaskIndex].priority ?  au8_TaskIndex : au16_highestPriorityIndex ;

				}

			}
			else
			{

				/*			noting			*/
			}

		}

		/* execute the highest priority task that ready */
		if (au8_TaskReadyFlag == TRUE)
		{
			gStrTaskProfile_TasksRepo[au16_highestPriorityIndex].task();

			/*	check if task one shot	*/
			if(gStrTaskProfile_TasksRepo[au16_highestPriorityIndex].shots == ONE_SHOT)
			{
				Sos_Delete_Task(au16_highestPriorityIndex);
			}
			else
			{
				/*  calculate the next time for the task to be executed */
				gStrTaskProfile_TasksRepo[au16_highestPriorityIndex].next_Excution_Time += gStrTaskProfile_TasksRepo[au16_highestPriorityIndex].priodicity;
			}
			au8_TaskReadyFlag = FALSE;
			gu8_Sos_TicksOverflowFlag = FALSE;
		}
		else
		{
			/* no task ready */
		}

		/*	sleep until next tick	*/

		Sos_IdleTask_Hock();

	}


}


/****************************************************************************
 *
 *   @brief function execute sleep in idle mode
 *   this function  takes void and return void
 *   @params  void
 *   @return void
 ****************************************************************************/
void Sos_IdleTask_Hock(void)
{
	/*	sleep until next tick	*/
	CPU_sleep();
}

void Sos_SchedulerPause(void)
{
	Timer_Stop(timerGetChannel(&timer0_cfg) );
}

/****************************************************************************
 *
 *   @brief function deinitiates Sos
 *   this function  takes void and return void
 *   @params  void
 *   @return void
 ****************************************************************************/
void Sos_Dinit (void )
{
	/*    clear timer configurations */
	Timer_Stop(CH0);
	timer0_cfg.ch= NUMBER_MINUS_ONE;
	timer0_cfg.resolution_us=NUMBER_ZERO;


	/*		clear task rep			*/
	gu16_TaskRepoSize=NUMBER_ZERO;

}
