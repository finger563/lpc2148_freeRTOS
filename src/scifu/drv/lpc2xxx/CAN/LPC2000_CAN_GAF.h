/****************************************************************************
* FILENAME:      LPC2000_CAN_GAF.h                                          *
*                                                                           *
* VERSION:       V1.0                                                       *
*                                                                           *
* DESCRIPTION:   Header File for CAN Driver LPC2000_CAN_Driver.c            *  
*				 CAN Global Acceptance Filter Configuration File     	    *
*                                                                          	*
* TOOLS:         KEIL uVision3                                          	*
*                ARM Development Suite ADS1.2                               *
*                                                                           *
*                                                                           *
* REVISION HISTORY:                                                         *
* Version  Author          Date          Remarks                            *
* 1.0      Anthony Xie     10/10/2005    - -                                *
*****************************************************************************/

/**
 * @file
 * CAN Controller Acceptance Filter Configuration
 *
 * This header file configures the acceptance filter, i.e. defines Identifier
 * ranges for all sorts of message filtering desired for a certain CAN network
 * This file contains some certain definitions (actually an example) and can
 * be used as a template for further modifications
 */
#ifndef LPC2000CANDRIVER_GAF_H
#define LPC2000CANDRIVER_GAF_H

#include <inttypes.h>

/*****************************************************************************/
/* Standard include files                                                    */
/*****************************************************************************/

/*****************************************************************************/
/* Project include files                                                     */
/*****************************************************************************/

/*****************************************************************************/
/* Types                                                                     */
/*****************************************************************************/

/**
 * Structure used for 11-bit CAN Identifiers
 */
typedef struct
{
  uint8_t    Channel;
  uint16_t   ID;

} lpc2000CANdriver_ACFilter_t;

/**
 * Structure used for 29-bit CAN Identifiers
 */
typedef struct
{
  uint8_t    Channel;
  uint32_t   IDx;

} lpc2000CANdriver_ACFilterx_t;



#endif /* LPC2000CANDRIVER_GAF_H */
