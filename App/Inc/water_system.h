#ifndef __MYSYSTEM_H__
#define __MYSYSTEM_H__


/****************************************************************************
 *
 *   @brief function initialize the system
 *   @params  void
 *   @return  void
 ****************************************************************************/
void systemInit(void);



/****************************************************************************
 *
 *   @brief function acts as task and polls the status of the ON/OFF display button
 *   @params  void
 *
 *
 *   @return  void
 ****************************************************************************/
void btnOnOffMonitoring_Task(void);





/****************************************************************************
 *
 *   @brief function polls up and down buttons to set temperture
 *   @params  void
 *
 *
 *   @return  void
 ****************************************************************************/
void btnsUpDownMonitoring_Task(void);


/****************************************************************************
 *
 *   @brief function checks cooling and heating status and accordingly turn the heater
 *    or cooler on or off
 *   @params  void
 *
 *
 *   @return  void
 ****************************************************************************/
void btnOnOffMonitoring_Task(void);

/****************************************************************************
 *
 *   @brief function acts as function polls the state of current temperature
 *   comparing to required one
 *   @params  void
 *
 *   @return  void
 ****************************************************************************/
void tempertureMonitoring_Task(void);


/****************************************************************************
 *
 *   @brief function acts as a task and polls the status of seven segements
 *   @params  void
 *
 *
 *   @return  void
 ****************************************************************************/
void SevenSegmentDisplay_Task(void);

/****************************************************************************
 *
 *   @brief function acts as a task and polls the eeprom to write on it
 *   @params  void
 *
 *
 *   @return  void
 ****************************************************************************/
void EEPROM_Task(void);

#endif
