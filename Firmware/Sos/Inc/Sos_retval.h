


#ifndef SOS_RETVAL_H_
#define SOS_RETVAL_H_


/**
*   @brief:  This return value indicate that the function could
*             run successfully and it returns no errors.
*/
#define   TMU_SUCCESS                            ( 0 )


/**
*   @brief:  This return value indicate that the selected resource
*           not found in the platform.
*/
#define   TMU_ERROR_RESOURCE_NOT_FOUND           ( -1 )
/**
*   @brief:  This return value indicate that the function tries
*             to use an uninitialized module.
*/
#define   TMU_ERROR_NOT_INITIALIZED              ( -2 )



#endif /* SOS_RETVAL_H_ */
