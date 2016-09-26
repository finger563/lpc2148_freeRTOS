/****************************************************************************
* FILENAME:      LPC2000_CAN.h                                              *
*                                                                           *
* VERSION:       V1.0                                                       *
*                                                                           *
* DESCRIPTION:   Header File for CAN Driver LPC2000_CAN_Driver.c            *  
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
 * CAN Controller Driver Header File
 */
#ifndef LPC2000CANDRIVER_H
#define LPC2000CANDRIVER_H

/*****************************************************************************/
/* Standard include files                                                    */
/*****************************************************************************/

/*****************************************************************************/
/* Project include files                                                     */
/*****************************************************************************/
//#include "..\Config.h"
#include "LPC2000_CAN_GAF.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* Types                                                                     */
/*****************************************************************************/

/**
 * Data structure for a CAN message to be transmitted
 */
typedef struct
{
   uint32_t   TFI;              //0x8xxxxxxx for 29bits length ID, 0x0xxxxxxx for 11 bits
   uint32_t   ID;
   uint32_t   DataField[2];

} lpc2000CANdriver_TXObj_t, *plpc2000CANdriver_TXObj_t;

/**
 * Data structure for a CAN message to be received
 */
typedef struct
{
   uint32_t   RFS;
   uint32_t   ID;
   uint32_t   DataField[2];

} lpc2000CANdriver_RXObj_t, *plpc2000CANdriver_RXObj_t;

typedef uint32_t CanStatusCode;
typedef uint32_t *pCanStatusCode;/**< General return code */

/*****************************************************************************/
/* Defines                                                                   */
/*****************************************************************************/

/**
 * Definition of return values
 */
#define LPC2000_CANDRIVER_INITIAL                                    0

#define LPC2000_CANDRIVER_OK                                         5   
#define LPC2000_CANDRIVER_ERR                                  		 10   

#define LPC2000_CANDRIVER_ERR_WRONG_CAN_CHANNEL_NUMBER               30
#define LPC2000_CANDRIVER_ERR_WRONG_CAN_TxBUFFER_NUMBER              31
#define LPC2000_CANDRIVER_ERR_TRANSMIT_BUFFER1_NOT_FREE              40
#define LPC2000_CANDRIVER_ERR_TRANSMIT_BUFFER2_NOT_FREE              50
#define LPC2000_CANDRIVER_ERR_TRANSMIT_BUFFER3_NOT_FREE              60

#define LPC2000_CANDRIVER_ERR_TABLE_ERROR_IN_STD_INDIVIDUAL_SECTION  90
#define LPC2000_CANDRIVER_ERR_TABLE_ERROR_IN_STD_GROUP_SECTION       100
#define LPC2000_CANDRIVER_ERR_TABLE_ERROR_IN_EXT_INDIVIDUAL_SECTION  110
#define LPC2000_CANDRIVER_ERR_TABLE_ERROR_IN_EXT_GROUP_SECTION       120
#define LPC2000_CANDRIVER_ERR_NOT_ENOUGH_MEMORY_SPACE                130


/**
 * Bit Timing Values for 16MHz clk frequency
 */
#define LPC2000_CANDRIVER_CANBITRATE100K16MHZ          0x001C0009
#define LPC2000_CANDRIVER_CANBITRATE125K16MHZ          0x001C0007
#define LPC2000_CANDRIVER_CANBITRATE250K16MHZ          0x001C0003
#define LPC2000_CANDRIVER_CANBITRATE500K16MHZ          0x001C0001
#define LPC2000_CANDRIVER_CANBITRATE1000K16MHZ         0x001C0000


/**
 * Bit Timing Values for 20MHz clk frequency
 */
#define LPC2000_CANDRIVER_CANBITRATE100K20MHZ          0x00140018
#define LPC2000_CANDRIVER_CANBITRATE125K20MHZ          0x002B0009
#define LPC2000_CANDRIVER_CANBITRATE250K20MHZ          0x002B0004
#define LPC2000_CANDRIVER_CANBITRATE500K20MHZ          0x00140004
#define LPC2000_CANDRIVER_CANBITRATE1000K20MHZ         0x00250001

/**
 * Bit Timing Values for 24MHz clk frequency
 */
#define LPC2000_CANDRIVER_CANBITRATE100K24MHZ          0x001C000E
#define LPC2000_CANDRIVER_CANBITRATE125K24MHZ          0x001C000B
#define LPC2000_CANDRIVER_CANBITRATE250K24MHZ          0x001C0005
#define LPC2000_CANDRIVER_CANBITRATE500K24MHZ          0x001C0002
#define LPC2000_CANDRIVER_CANBITRATE1000K24MHZ         0x00180001

/**
 * Bit Timing Values for 48MHz clk frequency
 */
#define LPC2000_CANDRIVER_CANBITRATE100K48MHZ          0x002B001D
#define LPC2000_CANDRIVER_CANBITRATE125K48MHZ          0x002B0017
#define LPC2000_CANDRIVER_CANBITRATE250K48MHZ          0x002B000B
#define LPC2000_CANDRIVER_CANBITRATE500K48MHZ          0x002B0005
#define LPC2000_CANDRIVER_CANBITRATE1000K48MHZ         0x002B0002

/**
 * Bit Timing Values for 60MHz clk frequency
 */
#define LPC2000_CANDRIVER_CANBITRATE100K60MHZ          0x00180031
#define LPC2000_CANDRIVER_CANBITRATE125K60MHZ          0x00180027
#define LPC2000_CANDRIVER_CANBITRATE250K60MHZ          0x00180013
#define LPC2000_CANDRIVER_CANBITRATE500K60MHZ          0x00180009
#define LPC2000_CANDRIVER_CANBITRATE1000K60MHZ         0x00180004


/**
 * Pre-defined CAN channel identifier
 */
#define LPC2000_CANCHANNEL_1            0x01
#define LPC2000_CANCHANNEL_2            0x02
#define LPC2000_CANCHANNEL_3            0x03
#define LPC2000_CANCHANNEL_4            0x04

/**
 * Pre-defined transmit buffer identifier
 */
#define LPC2000_TXB_1                   0x01
#define LPC2000_TXB_2                   0x02
#define LPC2000_TXB_3                   0x03

/**
 * Pre-defined Acceptance filter modes
 */
#define LPC2000_ACC_ON                  0x00
#define LPC2000_ACC_OFF                 0x01
#define LPC2000_ACC_BYPASS              0x03

/**
 * Pre-defined CAN controller operating modes
 */

#define CAN_RESET_MODE            		0x01
#define CAN_OPERATING_MODE                0x00
#define CAN_LISTENONLY_MODE               0x02
#define CAN_SELFTEST_MODE                 0x04


/*****************************************************************************/
/* External Global Variables                                                 */
/*****************************************************************************/
extern lpc2000CANdriver_TXObj_t CAN_Send_Data[];
extern lpc2000CANdriver_RXObj_t CAN_Rcv_Data[];


extern const lpc2000CANdriver_ACFilter_t  gklpc2000CANdriver_StdIndividualSection[];
extern const lpc2000CANdriver_ACFilter_t  gklpc2000CANdriver_StdGroupSection[];
extern const lpc2000CANdriver_ACFilterx_t  gklpc2000CANdriver_ExtIndividualSection[];
extern const lpc2000CANdriver_ACFilterx_t  gklpc2000CANdriver_ExtGroupSection[];

extern uint32_t CAN1Tx_Return_Message[5];
extern uint32_t CAN2Rx_Return_Message_Flag[10];
extern uint8_t CAN_Rcv_Data_Counter;
extern CanStatusCode Return_Value;
extern CanStatusCode CAN_Rcv_Status;


extern const uint8_t CAN_Error_10[];
extern const uint8_t CAN_Error_30[];
extern const uint8_t CAN_Error_31[];
extern const uint8_t CAN_Error_40[];
extern const uint8_t CAN_Error_50[];
extern const uint8_t CAN_Error_60[];
extern const uint8_t CAN_Error_90[];
extern const uint8_t CAN_Error_100[];
extern const uint8_t CAN_Error_110[];
extern const uint8_t CAN_Error_120[];
extern const uint8_t CAN_Error_130[];


extern const uint8_t CAN2Rx_Return_Message_EI[];	  
extern const uint8_t CAN2Rx_Return_Message_DOI[];	  
extern const uint8_t CAN2Rx_Return_Message_WUI[];	  
extern const uint8_t CAN2Rx_Return_Message_EPI[];	  
extern const uint8_t CAN2Rx_Return_Message_ALI[];	  
extern const uint8_t CAN2Rx_Return_Message_BEI[];

extern const uint8_t CAN_ERROR_STOP_DEMO[];
  
extern const uint8_t Demo_Start[];
extern const uint8_t Demo_Sent_Choice[];
extern const uint8_t Demo_Sent_Data[];
extern const uint8_t Data_Sent_Start_or_Not[];
extern const uint8_t Rcv_Data_OK[];
extern const uint8_t Acceptance_Filter_Tables[];



/*****************************************************************************/
/* External Functions                                                        */
/*****************************************************************************/

/**
 * Calling this function initializes the selected CAN Controller with the
 * specified Bit Timing and puts the CAN Controller into Operating Mode.
 *
 * @param[in]   canChannel      Channel number of CAN Controller
 * @param[in]   canBitrate      Pre-defined CAN Bit Timing value
 *
 * @return      The following error codes can be returned:
 *              - LPC2000_CANDRIVER_OK,
 *                on successful completion.
 */
CanStatusCode lpc2000CANdriver_CANInit(uint32_t canChannel,
                                  uint32_t canBitrate);


/**
 * Calling this function changes the CAN Controller mode.
 *
 * @param[in]   canChannel      Channel number of CAN Controller
 * @param[in]   CANMode         desired CAN controller mode
 *
 * @return      The following error codes can be returned;
 *              - LPC2000_CANDRIVER_OK,
 *                on successful completion.
 *              - LPC2000_CANDRIVER_ERR in case of an error.
 */
CanStatusCode lpc2000CANdriver_SetCANMode(uint32_t canChannel,
                                     uint8_t CANMode);

/**
 * Calling this function changes the Acceptance Filter mode.
 *
 * @param[in]   ACFMode         desired acceptance filter mode
 *
 * @return      The following error codes can be returned;
 *              - LPC2000_CANDRIVER_OK,
 *                on successful completion.
 *              - LPC2000_CANDRIVER_ERR in case of an error.
 */
CanStatusCode lpc2000CANdriver_SetACFMode(uint8_t ACFMode);

/**
 * Calling this function forces the CPU to copy transmit data from
 * User RAM into a certain transmit buffer. A 'buffer not free'
 * return code is given if a certain transmit buffers is occupied.
 *
 * @param[in]   canChannel      Channel number of CAN Controller
 * @param[in]   pTransmitBuf    pointer to a Transmit Buffer in User RAM
 * @param[in]   txbuffer        Transmit Buffer number: 1,2 or 3
 *
 * @return      The following error codes can be returned;
 *              - LPC2000_CANDRIVER_OK,
 *              and in case of errors:
 *              - LPC2000_CANDRIVER_ERR_WRONG_CAN_CHANNEL_NUMBER
 *              - LPC2000_CANDRIVER_ERR
 *              - LPC2000_CANDRIVER_ERR_TRANSMIT_BUFFER1_NOT_FREE
 *              - LPC2000_CANDRIVER_ERR_TRANSMIT_BUFFER2_NOT_FREE
 *              - LPC2000_CANDRIVER_ERR_TRANSMIT_BUFFER3_NOT_FREE
 */
CanStatusCode lpc2000CANdriver_CertainTxBufTransmitMessage(uint32_t canChannel,
                                                 plpc2000CANdriver_TXObj_t pTransmitBuf,
                                                 uint32_t txbuffer);


/**
 * Calling this function loads the Acceptance Filter Look_up table RAM with
 * pre-defined values stored in lpc2000_CANdriver_cfg.h. Depending on the number of
 * pre-defined identifiers the register start addresses for the Acceptance Filter
 * RAM are calculated and initialised automatically.
 *
 * @return      The following error codes can be returned;
 *              - LPC2000_CANDRIVER_OK,
 *              and in case of errors:
 *              - LPC2000_CANDRIVER_ERR
 */
CanStatusCode lpc2000CANdriver_LoadAcceptanceFilter(void);


/**
 * Calling this function copies the current Receive Buffer contents into the
 * specified Receive Buffer in User RAM. After copying the message data the
 * CAN Controllers receive buffer is released.
 *
 * @param[out]  pReceiveBuf     pointer to ReceiveBuffer in User RAM
 *
 * @return      The following error codes can be returned;
 *              - LPC2000_CANDRIVER_OK,
 *              and in case of errors:
 *              - LPC2000_CANDRIVER_ERR_WRONG_CAN_CHANNEL_NUMBER
 */
CanStatusCode lpc2000CANdriver_ReceiveMessageCh1 (plpc2000CANdriver_RXObj_t pReceiveBuf);


/**
 * Calling this function copies the current Receive Buffer contents into the
 * specified Receive Buffer in User RAM. After copying the message data the
 * CAN Controllers receive buffer is released.
 *
 * @param[out]  pReceiveBuf     pointer to ReceiveBuffer in User RAM
 *
 * @return      The following error codes can be returned;
 *              - LPC2000_CANDRIVER_OK,
 *              and in case of errors:
 *              - LPC2000_CANDRIVER_ERR_WRONG_CAN_CHANNEL_NUMBER
 */
CanStatusCode lpc2000CANdriver_ReceiveMessageCh2 (plpc2000CANdriver_RXObj_t pReceiveBuf);


/**
 * Calling this function copies the current Receive Buffer contents into the
 * specified Receive Buffer in User RAM. After copying the message data the
 * CAN Controllers receive buffer is released.
 *
 * @param[out]  pReceiveBuf     pointer to ReceiveBuffer in User RAM
 *
 * @return      The following error codes can be returned;
 *              - LPC2000_CANDRIVER_OK,
 *              and in case of errors:
 *              - LPC2000_CANDRIVER_ERR_WRONG_CAN_CHANNEL_NUMBER
 */
CanStatusCode lpc2000CANdriver_ReceiveMessageCh3 (plpc2000CANdriver_RXObj_t pReceiveBuf);


/**
 * Calling this function copies the current Receive Buffer contents into the
 * specified Receive Buffer in User RAM. After copying the message data the
 * CAN Controllers receive buffer is released.
 *
 * @param[out]  pReceiveBuf     pointer to ReceiveBuffer in User RAM
 *
 * @return      The following error codes can be returned;
 *              - LPC2000_CANDRIVER_OK,
 *              and in case of errors:
 *              - LPC2000_CANDRIVER_ERR_WRONG_CAN_CHANNEL_NUMBER
 */
CanStatusCode lpc2000CANdriver_ReceiveMessageCh4 (plpc2000CANdriver_RXObj_t pReceiveBuf);



/**
 * Calling this function display one frame of Received Data information
 * stored in User RAM to PC Screen by Uart0
 *
 * @param[in]  	pReceiveBuf     pointer to ReceiveBuffer in User RAM
 *
 * @return      
 */
uint32_t Rcv_Data_Output_to_Screen(plpc2000CANdriver_RXObj_t   pReceiveBuf);


/**
 * Calling this function Print Chars to Screen by Uart0
 *
 * @param[in]  	pscreen_print     pointer to Chars that be prepared to printed
 *
 * @return      
 */
uint32_t Print_Chars_to_Screen( uint8_t const * pscreen_print );


/**
 * Calling this function Print 4bits hex data to Screen by Uart0
 *
 * @param[in]  	Value_Hex_4bits      4bits HEX value that be prepared to printed
 *
 * @return      
 */
uint32_t Print_4bits_to_Screen( uint32_t Value_Hex_4bits);

void IRQ_CAN2Rx(void);

#ifdef __cplusplus
}
#endif

#endif /* LPC2000CANDRIVER_H */
