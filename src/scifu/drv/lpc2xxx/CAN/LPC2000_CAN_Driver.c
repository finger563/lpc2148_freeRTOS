/**************************************************************************
* FILENAME:      LPC2000_CAN_Driver.c                                     *         
*                                                                         *  
* VERSION:       V1.0                                                     *  
*                                                                         *  
* DESCRIPTION:   Philips LPC2000 CAN Controller Driver with SJA1000       *                         
*				 Demo by UART0 with KEIL MCB2100 Evaluation Board		  *
*                                                                         *  
* TOOLS:         KEIL uVision3                                     		  *
*                ARM Development Suite ADS1.2 							  *
*                KEIL MCB2100 Evaluation Board               			  *
*                                                                         *  
* REVISION HISTORY                                                        *  
* Version  Author           Date        Remarks							  *
* 1.0      Anthony Xie      10/10/2005    - -                             *         
***************************************************************************/

/**
 * @file
 * These CAN Controller driver routines are designed to provide the
 * application programmer with a higher level interface for the communication
 * with each CAN Controller module.
 * The CAN Controller Driver routines perform the following functions:
 *    Initialization of the CAN Controller
 *    Configuration of the CAN Controller for various baud rates
 *    Preparation and transmission of CAN messages
 *    Store received CAN messages in appropriate message buffers
 *    Providing pre-defined values for a set of bit-rates for various
 *        clock frequencies
 *    Mode switching of the CAN Controller
 *    Easy read/write access to CAN Controller registers
 */

// **************************************************************************
// Standard include files                                                    
// **************************************************************************

// **************************************************************************
// Project include files                                                     
// **************************************************************************

#include  <targets/lpc2000.h>
#include  "LPC2000_CAN.h"
#include  "LPC2000_CAN_GAF.h"   // Includes Global Acceptance Filter   


// **************************************************************************
// Types                                                                     
// **************************************************************************

// **************************************************************************
// Defines                                                                   
// **************************************************************************


//#define SEMAPHORE_00                    0x00000000
//#define SEMAPHORE_01                    0x01000000
//#define SEMAPHORE_11                    0x03000000


// --------------------------------------------------------------------------
// ---------------------------   hwAccess Layer -----------------------------
// --------------------------------------------------------------------------

#define    OUTB(x,y)    (*((volatile UInt8 *)(x)) = y)
#define    INB(x)       (*((volatile UInt8 *)(x)))

#define    OUTW(x,y)    (*((volatile UInt32 *)(x)) = y)
#define    INW(x)       (*((volatile UInt32 *)(x)))

// **************************************************************************
// External Functions                                                        
// **************************************************************************

// **************************************************************************
// External Variables                                                        
// **************************************************************************

// **************************************************************************
// Global Variables                                                          
// **************************************************************************
lpc2000CANdriver_TXObj_t CAN_Send_Data[] =
 { {0x00080000,0x00000001,0x04030201,0x08070605},
   {0x00080000,0x000001AC,0x14131211,0x18171615},
   {0x00080000,0x00000310,0x24232221,0x28272625},
   {0x80080000,0x00003377,0x34333231,0x38373635},
   {0x80080000,0x00007755,0x44434241,0x48474645} };

lpc2000CANdriver_RXObj_t CAN_Rcv_Data[5];

/*---------------------------------------------------------------------------*/
/* Defines for Global Acceptance Filter                                      */                             
/*---------------------------------------------------------------------------*/

//Size of Acceptance Filter Sections

#define LPC2000_CANDRIVER_SIZE_OF_ACFMEMORY   2048 /* Size of SJA1000 Look_up Table RAM    */

// Pre-defined CAN Acceptance Filter RAM Address

#define CAFMEM                                0xE0038000


/*---------------------------------------------------------------------------*/
/*Sections can be enabled or disabled to the application specific needs		 */
/*---------------------------------------------------------------------------*/
 #define LPC2000_CANDRIVER_STD_INDIVIDUAL
 #define LPC2000_CANDRIVER_STD_GROUP      
 #define LPC2000_CANDRIVER_EXT_INDIVIDUAL 
 #define LPC2000_CANDRIVER_EXT_GROUP      

/*---------------------------------------------------------------------------*/
/* Acceptance Filter Configuration Table Constants                           */
/*---------------------------------------------------------------------------*/

#define LPC2000_CANDRIVER_SCC_1                     1	// SCC: Source CAN Cannel
#define LPC2000_CANDRIVER_SCC_2                     2
#define LPC2000_CANDRIVER_SCC_3                     3
#define LPC2000_CANDRIVER_SCC_4                     4


#ifdef LPC2000_CANDRIVER_STD_INDIVIDUAL
/* Please, add or change the list of pre-defined   */
/* entries to the application specific needs       */
const lpc2000CANdriver_ACFilter_t  gklpc2000CANdriver_StdIndividualSection[] =
   {
     /* Channel(1-4) ,              11-bit Identifier ( <7FF Double than ram size ) */
     {LPC2000_CANDRIVER_SCC_2,       0x0010},
     {LPC2000_CANDRIVER_SCC_2,       0x01AC},
     {LPC2000_CANDRIVER_SCC_2,       0x0245},
     {LPC2000_CANDRIVER_SCC_2,       0x025F}
   };								   

        
#define LPC2000_CANDRIVER_NUMBER_OF_STD_INDIVIDUAL_IDS (\
        sizeof gklpc2000CANdriver_StdIndividualSection /\
        sizeof gklpc2000CANdriver_StdIndividualSection[0])

#else
#define LPC2000_CANDRIVER_NUMBER_OF_STD_INDIVIDUAL_IDS  0
#endif


#ifdef LPC2000_CANDRIVER_STD_GROUP 
/* Please, add or change the list of pre-defined   */
/* entries to the application specific needs       */
const lpc2000CANdriver_ACFilter_t  gklpc2000CANdriver_StdGroupSection[] =
   {
     /* Channel                11-bit Identifier */
     {LPC2000_CANDRIVER_SCC_2,       0x0300},        // lower bound, Group 1 
     {LPC2000_CANDRIVER_SCC_2,       0x037F},        // upper bound, Group 1 
	 {LPC2000_CANDRIVER_SCC_2,       0x0400},        // lower bound, Group 2 
	 {LPC2000_CANDRIVER_SCC_2,       0x047F}         // upper bound, Group 1 
   };
#define LPC2000_CANDRIVER_NUMBER_OF_STD_GROUP_IDS (\
        sizeof gklpc2000CANdriver_StdGroupSection /\
        sizeof gklpc2000CANdriver_StdGroupSection[0])

#else
#define LPC2000_CANDRIVER_NUMBER_OF_STD_GROUP_IDS  0
#endif

#ifdef LPC2000_CANDRIVER_EXT_INDIVIDUAL
/* Please, add or change the list of pre-defined   */
/* entries to the application specific needs       */
const lpc2000CANdriver_ACFilterx_t  gklpc2000CANdriver_ExtIndividualSection[] =
   {
     /* Channel                29-bit Identifier   ( =< 0x1FFFFFFF) */
     {LPC2000_CANDRIVER_SCC_2,       0x00002288},
     {LPC2000_CANDRIVER_SCC_2,       0x00003377},
	 {LPC2000_CANDRIVER_SCC_2,       0x00005566},
	 {LPC2000_CANDRIVER_SCC_2,       0x00006677}
   };
#define LPC2000_CANDRIVER_NUMBER_OF_EXT_INDIVIDUAL_IDS (\
        sizeof gklpc2000CANdriver_ExtIndividualSection /\
        sizeof gklpc2000CANdriver_ExtIndividualSection[0])

#else
#define LPC2000_CANDRIVER_NUMBER_OF_EXT_INDIVIDUAL_IDS 0
#endif


#ifdef LPC2000_CANDRIVER_EXT_GROUP
/* Please, add or change the list of pre-defined   */
/* entries to the application specific needs       */
const lpc2000CANdriver_ACFilterx_t  gklpc2000CANdriver_ExtGroupSection[] =
   {
     /* Channel                29-bit Identifier ( =< 0x1FFFFFFF) */
     {LPC2000_CANDRIVER_SCC_2,       0x00007700},     // lower bound, Group 1 
     {LPC2000_CANDRIVER_SCC_2,       0x000077FF},     // upper bound, Group 1
     {LPC2000_CANDRIVER_SCC_2,       0x000085F7},     // lower bound, Group 2 
     {LPC2000_CANDRIVER_SCC_2,       0x00008802}      // upper bound, Group 2

   };
#define LPC2000_CANDRIVER_NUMBER_OF_EXT_GROUP_IDS (\
        sizeof gklpc2000CANdriver_ExtGroupSection /\
        sizeof gklpc2000CANdriver_ExtGroupSection[0])

#else
#define LPC2000_CANDRIVER_NUMBER_OF_EXT_GROUP_IDS      0
#endif


/*---------------------------------------------------------------------------*/
/* CAN Global Variable                                                       */
/*---------------------------------------------------------------------------*/
UInt32 CAN1Tx_Return_Message[5] = {0,0,0,0,0};
UInt32 CAN2Rx_Return_Message_Flag[10] = {0,0,0,0,0,0,0,0,0,0};
UInt8 CAN_Rcv_Data_Counter=0;
CanStatusCode Return_Value = LPC2000_CANDRIVER_INITIAL;
CanStatusCode CAN_Rcv_Status = 0;

/*---------------------------------------------------------------------------*/
/* CAN Uart Output char define                                                       */
/*---------------------------------------------------------------------------*/
const UInt8 CAN_Error_10[] = "\n\r\
*** Error: LPC2000_CANDRIVER_ERR\n\r\
\n\r";

const UInt8 CAN_Error_30[] = "\n\r\
*** Error: LPC2000_CANDRIVER_ERR_WRONG_CAN_CHANNEL_NUMBER\n\r\
\n\r";

const UInt8 CAN_Error_31[] = "\n\r\
*** Error: LPC2000_CANDRIVER_ERR_WRONG_CAN_TxBUFFER_NUMBER\n\r\
\n\r";

const UInt8 CAN_Error_40[] = "\n\r\
*** Error: LPC2000_CANDRIVER_ERR_TRANSMIT_BUFFER1_NOT_FREE\n\r\
\n\r";

const UInt8 CAN_Error_50[] = "\n\r\
*** Error: LPC2000_CANDRIVER_ERR_TRANSMIT_BUFFER2_NOT_FREE\n\r\
\n\r";

const UInt8 CAN_Error_60[] = "\n\r\
*** Error: LPC2000_CANDRIVER_ERR_TRANSMIT_BUFFER3_NOT_FREE\n\r\
\n\r";

const UInt8 CAN_Error_90[] = "\n\r\
*** Error: LPC2000_CANDRIVER_ERR_TABLE_ERROR_IN_STD_INDIVIDUAL_SECTION\n\r\
\n\r";

const UInt8 CAN_Error_100[] = "\n\r\
*** Error: LPC2000_CANDRIVER_ERR_TABLE_ERROR_IN_STD_GROUP_SECTION\n\r\
\n\r";

const UInt8 CAN_Error_110[] = "\n\r\
*** Error: LPC2000_CANDRIVER_ERR_TABLE_ERROR_IN_EXT_INDIVIDUAL_SECTION\n\r\
\n\r";

const UInt8 CAN_Error_120[] = "\n\r\
*** Error: LPC2000_CANDRIVER_ERR_TABLE_ERROR_IN_EXT_GROUP_SECTION\n\r\
\n\r";

const UInt8 CAN_Error_130[] = "\n\r\
*** Error: LPC2000_CANDRIVER_ERR_NOT_ENOUGH_MEMORY_SPACE\n\r\
\n\r";



const UInt8 CAN2Rx_Return_Message_EI[] = "\n\r\
*** Error: Error Warning Interrupt-EI bit set\n\r\
\n\r";	  

const UInt8 CAN2Rx_Return_Message_DOI[] = "\n\r\
*** Error: Data Overrun Interrupt-DOI bit set\n\r\
\n\r";	  

const UInt8 CAN2Rx_Return_Message_WUI[] = "\n\r\
*** Error: Wake-Up Interrupt-WUI bit set\n\r\
\n\r";	  

const UInt8 CAN2Rx_Return_Message_EPI[] = "\n\r\
*** Error: Error Passive Interrupt-EPI bit set\n\r\
\n\r";	  

const UInt8 CAN2Rx_Return_Message_ALI[] = "\n\r\
*** Error: Arbitration Lost Interrupt-ALI bit set\n\r\
\n\r";	  

const UInt8 CAN2Rx_Return_Message_BEI[] = "\n\r\
*** Error: Bus Error Interrupt-BEI bit set\n\r\
\n\r";	  

const UInt8 CAN_ERROR_STOP_DEMO[] = "\
*** Pls. STOP demo.  Check board setting, wire connection. And restart demo.\
\n\r";	  


const UInt8 Demo_Start[] = "\f\
$$$$$$$ PHILIPS LPC2000 CAN BUS DRIVER DEMO - with SJA1000 $$$$$$$\n\n\r\
***Demo Files:    LPC2000_CAN_SYS.C\n\r\
***               LPC2000_CAN_Driver.c\n\n\r\
***Demo Tools:    Keil MCB2100 Demo Board ( CAN1-Trans, CAN2-Rcv )\n\n\r\
***Demo Function: Press 1 - simple communication ( GLOBAL ACC FILTER Bypass )\n\r\
***               Press 2 - communication with GLOBAL ACC FILTER\n\r\
\n\r";

const UInt8 Demo_Sent_Choice[]="\n\r\
***If you want demo again \n\r\
***Enter your choice: 1 for ACC Bypass, 2 for ACC On\n\r";

const UInt8 Demo_Sent_Data[] = "\n\r\
***Total 40 bytes data (using 5 frames) will be transmitted by CAN1 to CAN BUS\n\r\
***Frame1: Data - 01,02,03,04,05,06,07,08 with Standard 11bits ID:0x001;\n\r\
***Frame2: Data - 11,12,13,14,15,16,17,18 with Standard 11bits ID:0x1AC;\n\r\
***Frame3: Data - 21,22,23,24,25,26,27,28 with Standard 11bits ID:0x310;\n\r\
***Frame4: Data - 31,32,33,34,35,36,37,38 with Extended 29bits ID:0x3377;\n\r\
***Frame5: Data - 41,42,43,44,45,46,47,48 with Extended 29bits ID:0x7755;\n\r\
\n\r";

const UInt8 Data_Sent_Start_or_Not[] = "\
***Start transmit now? Press y or Y - start\n\r\
***                    Press n or N - Not start\
\n\r";


const UInt8 Rcv_Data_OK[] = "\n\r\
*** Communication Successful!  data Rcved OK!   From CAN BUS by CAN2: \
\n\r";

const UInt8 Acceptance_Filter_Tables[] = "\
*** Acceptance_Filter_Tables:\n\r\
*** Idx  Table      SCC#  Dis/En  ID\n\r\
*** 0    Standard    2      0     0010H\n\r\
*** 1    Standard    2      0     01ACH\n\r\
*** 2    Standard    2      0     0245H\n\r\
*** 3    Standard    2      0     025FH\n\r\
*** 4    Std.Group   2      0     0300H-037FH\n\r\
*** 5    Std.Group   2      0     0400H-047FH\n\r\
*** 6    Extended    2      0     00002288H\n\r\
*** 7    Extended    2      0     00003377H\n\r\
*** 8    Extended    2      0     00005566H\n\r\
*** 9    Extended    2      0     00006677H\n\r\
*** 10   Ext.Group   2      0     00007700H-000077FFH\n\r\
*** 11   Ext.Group   2      0     000085F7H-00008802H\n\r\
\n\r";






// **************************************************************************
// Local Functions Prototypes                                                
// **************************************************************************

// **************************************************************************
//                                                                           
// Implementation                                                            
//                                                                           
// **************************************************************************


// **************************************************************************
// FUNCTION:    lpc2000CANdriver_CANInit                                          
//                                                                           
// DESCRIPTION: Initialises the CAN Controller for channel 1,2,3,4          
//              - Enter the Reset Mode of the selected CAN Controller Unit   
//              - CAN Bit Timing                                             
//              - Enter Normal Operating Mode of the CAN Controller          
//                                                                           
// GLOBAL VARIABLES USED:                                                    
//                                                                           
// PARAMETERS:  canChannel: 1,2,3,4                                     
//              canBitrate: Register value, see also global.h for defines    
//                                                                           
// RETURN:      CanStatusCode: Status of operation                           
//                                                                           
//              LPC2000_CANDRIVER_OK - successful                                 
//                                                                           
// **************************************************************************




CanStatusCode
lpc2000CANdriver_CANInit ( UInt32   canChannel,
                      UInt32   canBitrate )
{

   switch (canChannel)  // CAN Channel 
   {
      case 1: // Enter Reset Mode 
			  CAN1MOD = CAN_RESET_MODE;
              // Write into Bus Timing Register 
			  CAN1BTR = canBitrate;
              // Enter Operating Mode 
			  CAN1MOD = CAN_OPERATING_MODE;
              break;

      case 2: // Enter Reset Mode 
			  CAN2MOD = CAN_RESET_MODE;
              // Write into Bus Timing Register 
			  CAN2BTR = canBitrate;
              // Enter Operating Mode 
			  CAN2MOD = CAN_OPERATING_MODE;
              break;
/*
      case 3: // Enter Reset Mode 
			  CAN3MOD = CAN_RESET_MODE;
              // Write into Bus Timing Register 
			  CAN3BTR = canBitrate;
              // Enter Operating Mode 
			  CAN3MOD = CAN_OPERATING_MODE;
              break;

      case 4: // Enter Reset Mode 
			  CAN4MOD = CAN_RESET_MODE;
              // Write into Bus Timing Register 
			  CAN4BTR = canBitrate;
              // Enter Operating Mode 
 			  CAN4MOD = CAN_OPERATING_MODE;
              break;
*/
      default:// wrong channel number 
              return (LPC2000_CANDRIVER_ERR_WRONG_CAN_CHANNEL_NUMBER);

   }

   return (LPC2000_CANDRIVER_OK);


} // lpc2000CANdriver_CANInit () 




// **************************************************************************
// FUNCTION:    lpc2000CANdriver_SetACFMode                                       
//                                                                           
// DESCRIPTION: This function sets the Acceptance Filter mode                
//                                                                           
// GLOBAL VARIABLES USED: - -                                                
//                                                                           
// PARAMETERS:  ACFMode:  0 = Acceptance Filter On                           
//                        1 = Acceptance Filter Off  (reset status)                        
//                        3 = Acceptance Filter Bypass                       
//                        4 = Acceptance Filter On + FullCAN Mode enabled    
//                                                                           
// RETURN:      CanStatusCode: Status of operation                           
//                                                                           
//              LPC2000_CANDRIVER_OK     - successful                             
//                                                                           
//              LPC2000_CANDRIVER_ERR    - this mode is not supported             
//                                                                           
// **************************************************************************

CanStatusCode
lpc2000CANdriver_SetACFMode( UInt8    ACFMode )
{

   switch (ACFMode)
   {
      case 0: // Acceptance Filter On 
			  AFMR = 0x00000000;
              break;
      case 1: // Acceptance Filter Off 
			  AFMR = 0x00000001;
              break;
      case 3: // Acceptance Filter Bypass 
 			  AFMR = 0x00000003;
              break;
      case 4: // Acceptance Filter FullCAN 
			  AFMR = 0x00000004;
              break;

      default:// Not supported 
              return (LPC2000_CANDRIVER_ERR);

   }
   return (LPC2000_CANDRIVER_OK);

} // lpc2000CANdriver_SetACFMode() 

// **************************************************************************
// FUNCTION:    lpc2000CANdriver_SetCANMode                                       
//                                                                           
// DESCRIPTION: This function sets the CAN Controller mode                   
//                                                                           
// GLOBAL VARIABLES USED:                                                    
//                                                                           
// PARAMETERS:  canChannel: 1,2,3,4                                      
//              CANMode   : 0 = Operating Mode                               
//                          1 = Reset Mode                                   
//                          2 = Listen Only Mode                             
//                          4 = Self Test Mode                               
//                                                                           
// RETURN:      CanStatusCode: Status of operation                           
//                                                                           
//              LPC2000_CANDRIVER_OK       - successful                           
//                                                                           
//              LPC2000_CANDRIVER_ERR      - this mode is not supported           
//                                                                           
// **************************************************************************

CanStatusCode
lpc2000CANdriver_SetCANMode(UInt32   canChannel,
                       UInt8    CANMode )
{
   UInt32 offset;

   offset = (canChannel-1) * 0x00004000;

   if ( (canChannel==0) || (canChannel>4))
   {
      return (LPC2000_CANDRIVER_ERR_WRONG_CAN_CHANNEL_NUMBER);
   }

   switch (CANMode)
   {
      case 0: // Operating Mode 
              OUTW((UInt32)&CAN1MOD + offset, CAN_OPERATING_MODE);
              break;
      case 1: // Reset Mode 
              OUTW((UInt32)&CAN1MOD + offset, CAN_RESET_MODE);
              break;
      case 2: // Listen Only Mode 
              OUTW((UInt32)&CAN1MOD + offset, CAN_RESET_MODE);
              OUTW((UInt32)&CAN1MOD + offset, CAN_LISTENONLY_MODE);
              break;
      case 4: // Self Test Mode 
              OUTW((UInt32)&CAN1MOD + offset, CAN_RESET_MODE);
              OUTW((UInt32)&CAN1MOD + offset, CAN_SELFTEST_MODE);
              break;

      default:// Not supported 
              return (LPC2000_CANDRIVER_ERR);

   }
   return (LPC2000_CANDRIVER_OK);

} // lpc2000CANdriver_SetACFMode() 



// **************************************************************************
// FUNCTION:    lpc2000CANdriver_PrepareTransmitMessage                           
//                                                                           
// DESCRIPTION: This function loads a certain Transmit Buffer with message   
//              data                                                         
//                                                                           
// GLOBAL VARIABLES USED:                                                    
//                                                                           
// PARAMETERS:                                                               
//                                                                           
// RETURN:      CanStatusCode: Status of operation                           
//                                                                           
//              LPC2000_CANDRIVER_OK        - successful                          
//                                                                           
//              LPC2000_CANDRIVER_ERR       - transmit buffer not free            
//                                          - wrong channel                       
//                                                                           
// **************************************************************************

CanStatusCode
lpc2000CANdriver_CertainTxBufTransmitMessage (UInt32                 canChannel,
                                         plpc2000CANdriver_TXObj_t   pTransmitBuf,
                                         UInt32                      txbuffer)
{
   UInt32   TransmitBufferStatus;
   UInt32   Address_TFI_ID_DATA;
   UInt32   Address_CMR;
   UInt32   Address_GSR;
   UInt32   TXB_Offset;
   UInt32   TransmitBufferFree;
   UInt8    Transmit_TxBuf_Start;
//   UInt32   Temp_Transmit;
//   pUInt32  pTemp_Transmit;

   // Calculate address offset depending on selected tx buffer 
   TXB_Offset = ((txbuffer-1) * 0x10);
   
   // Transmit start command for different txbuffer
   switch (txbuffer)
   {
      case 1: 
	          Transmit_TxBuf_Start = 0x21;   // Transmit by Tx1, mornal mode
//			  Transmit_TxBuf_Start = 0x23;	 // Single transmit in spite of successful
//			  Transmit_TxBuf_Start = 0x30;	 // Self-Testing mode, CAN1MOD:STM should be "1" 
//			  Transmit_TxBuf_Start = 0x32;	 // Single Self-Testing mode
	          break;
	  case 2: Transmit_TxBuf_Start = 0x41;   // Transmit by Tx2
//			  Transmit_TxBuf_Start = 0x43;	 // Single transmit in spite of successful
//			  Transmit_TxBuf_Start = 0x50;	 // Self-Testing mode, CAN1MOD:STM should be "1" 
//			  Transmit_TxBuf_Start = 0x52;	 // Single Self-Testing mode
	          break;
	  case 3: Transmit_TxBuf_Start = 0x81;	 // Transmit by Tx3
//			  Transmit_TxBuf_Start = 0x83;	 // Single transmit in spite of successful
//			  Transmit_TxBuf_Start = 0x90;	 // Self-Testing mode, CAN1MOD:STM should be "1" 
//			  Transmit_TxBuf_Start = 0x92;	 // Single Self-Testing mode
	          break;

	  default:return (LPC2000_CANDRIVER_ERR_WRONG_CAN_TxBUFFER_NUMBER);
	    break;
   } 

   // Read Global Status Register 

   switch (canChannel)
   {
      case 1: TransmitBufferStatus = CAN1SR;
              break;
      case 2: TransmitBufferStatus = CAN2SR;
              break;
      /*case 3: TransmitBufferStatus = CAN3SR;
              break;
      case 4: TransmitBufferStatus = CAN4SR;
              break;
      */
      default:return (LPC2000_CANDRIVER_ERR_WRONG_CAN_CHANNEL_NUMBER);

   }

   switch (txbuffer)
   {
      case 1: TransmitBufferFree = TransmitBufferStatus & 0x0000000C; 
              if (TransmitBufferFree == 0)	  // one of Two(TBS,TCS) fits will pass
              {
                 return (LPC2000_CANDRIVER_ERR_TRANSMIT_BUFFER1_NOT_FREE);
              }
              break;
      case 2: TransmitBufferFree = TransmitBufferStatus & 0x00000C00;
              if (TransmitBufferFree == 0)
              {
                 return (LPC2000_CANDRIVER_ERR_TRANSMIT_BUFFER2_NOT_FREE);
              }
              break;
      case 3: TransmitBufferFree = TransmitBufferStatus & 0x000C0000;
              if (TransmitBufferFree == 0)
              {
                 return (LPC2000_CANDRIVER_ERR_TRANSMIT_BUFFER3_NOT_FREE);
              }
              break;
      default:// wrong txbuffer number 
              return (LPC2000_CANDRIVER_ERR);

   }

   switch (canChannel)  // CAN Channel 
   {
      
      // CAN Channel 1 
	  case 1:   Address_TFI_ID_DATA = (UInt32)&CAN1TFI1 + TXB_Offset;
	            Address_CMR = (UInt32)&CAN1CMR;
				Address_GSR = (UInt32)&CAN1GSR;
                break;

      // CAN Channel 2 
      case 2:   Address_TFI_ID_DATA = (UInt32)&CAN1TFI1 + 0x00004000 + TXB_Offset;
	            Address_CMR = (UInt32)&CAN1CMR + 0x00004000;
				Address_GSR = (UInt32)&CAN1GSR + 0x00004000;
                break;

      // CAN Channel 3 
      case 3:   Address_TFI_ID_DATA = (UInt32)&CAN1TFI1 + 0x00008000 + TXB_Offset;
				Address_CMR = (UInt32)&CAN1CMR + 0x00008000;
				Address_GSR = (UInt32)&CAN1GSR + 0x00008000;
                break;

      // CAN Channel 4 
      case 4:   Address_TFI_ID_DATA = (UInt32)&CAN1TFI1 + 0x0000C000 + TXB_Offset;
	            Address_CMR = (UInt32)&CAN1CMR + 0x0000C000;
				Address_GSR = (UInt32)&CAN1GSR + 0x0000C000;
                break;

      default:  // wrong channel number 
                return (LPC2000_CANDRIVER_ERR_WRONG_CAN_CHANNEL_NUMBER);

   }

   // Copy message contents into local variable(s) 

   OUTW( Address_TFI_ID_DATA + 0, pTransmitBuf -> TFI);
   OUTW( Address_TFI_ID_DATA + 4, pTransmitBuf -> ID);
   OUTW( Address_TFI_ID_DATA + 8, pTransmitBuf -> DataField[0]);
   OUTW( Address_TFI_ID_DATA +12, pTransmitBuf -> DataField[1]);

   OUTB( Address_CMR, Transmit_TxBuf_Start );

   while ( (INW(Address_GSR) & 0x0048) == 0x0000 );  // Wait Trans successed or error counter reach limit

   if( (INW(Address_GSR) & 0x0040) == 0x0040 ) 		 // if error, stop transmit
   {												 // for demo used, stop screen roll
   OUTB( Address_CMR, 0x02 );                        
	}												 

   return (LPC2000_CANDRIVER_OK);

}  // lpc2000CANdriver_PrepareTransmitMessage; 




// **************************************************************************
// FUNCTION:    lpc2000CANdriver_ReceiveMessageCh1                                
//                                                                           
// DESCRIPTION: This function reads the received CAN message and stores      
//              the data in the CPU User RAM                                 
//                                                                           
// GLOBAL VARIABLES USED:                                                    
//                                                                           
// PARAMETERS:                                                               
//                                                                           
// RETURN:      CanStatusCode: Status of operation                           
//                                                                           
//              LPC2000_CANDRIVER_OK - successful                                 
//                                                                           
// **************************************************************************

CanStatusCode
lpc2000CANdriver_ReceiveMessageCh1 (plpc2000CANdriver_RXObj_t   pReceiveBuf)
{

   // CAN Channel 1 

   pReceiveBuf -> RFS = CAN1RFS;
   pReceiveBuf -> ID = CAN1RID;

   pReceiveBuf -> DataField[0] = CAN1RDA;
   pReceiveBuf -> DataField[1] = CAN1RDB;

   // Release Receive Buffer 

   CAN1CMR = 0x04;                 // Release Rx Buffer


   return (1);
}   // lpc2000CANdriver_ReceiveMessageCh0; 



// **************************************************************************
// FUNCTION:    lpc2000CANdriver_ReceiveMessageCh2                                
//                                                                           
// DESCRIPTION: This function reads the received CAN message and stores      
//              the data in the CPU User RAM                                 
//                                                                           
// GLOBAL VARIABLES USED:                                                    
//                                                                           
// PARAMETERS:                                                               
//                                                                           
// RETURN:      CanStatusCode: Status of operation                           
//                                                                           
//              LPC2000_CANDRIVER_OK - successful                                 
//                                                                           
// **************************************************************************

CanStatusCode
lpc2000CANdriver_ReceiveMessageCh2 (plpc2000CANdriver_RXObj_t   pReceiveBuf)
{
   // CAN Channel 2 

   pReceiveBuf -> RFS = CAN2RFS;
   pReceiveBuf -> ID = CAN2RID;

   pReceiveBuf -> DataField[0] = CAN2RDA;
   pReceiveBuf -> DataField[1] = CAN2RDB;

   // Release Receive Buffer 

   CAN2CMR = 0x04;      // Release Rx Buffer


   return (1);
}   // lpc2000CANdriver_ReceiveMessageCh2; 

// **************************************************************************
// FUNCTION:    lpc2000CANdriver_ReceiveMessageCh3                                
//                                                                           
// DESCRIPTION: This function reads the received CAN message and stores      
//              the data in the CPU User RAM                                 
//                                                                           
// GLOBAL VARIABLES USED:                                                    
//                                                                           
// PARAMETERS:                                                               
//                                                                           
// RETURN:      CanStatusCode: Status of operation                           
//                                                                           
//              LPC2000_CANDRIVER_OK - successful                                 
//                                                                           
// **************************************************************************

CanStatusCode
lpc2000CANdriver_ReceiveMessageCh3 (plpc2000CANdriver_RXObj_t   pReceiveBuf)
{

   // CAN Channel 3 

   /*pReceiveBuf -> RFS = CAN3RFS;
   pReceiveBuf -> ID = CAN3RID;

   pReceiveBuf -> DataField[0] = CAN3RDA;
   pReceiveBuf -> DataField[1] = CAN3RDB;

   // Release Receive Buffer 

   CAN3CMR = 0x04;                 // Release Rx Buffer
*/

   return (1);
}   // lpc2000CANdriver_ReceiveMessageCh3; 

// **************************************************************************
// FUNCTION:    lpc2000CANdriver_ReceiveMessageCh4                                
//                                                                           
// DESCRIPTION: This function reads the received CAN message and stores      
//              the data in the CPU User RAM                                 
//                                                                           
// GLOBAL VARIABLES USED:                                                    
//                                                                           
// PARAMETERS:                                                               
//                                                                           
// RETURN:      CanStatusCode: Status of operation                           
//                                                                           
//              LPC2000_CANDRIVER_OK - successful                                 
//                                                                           
// **************************************************************************

CanStatusCode
lpc2000CANdriver_ReceiveMessageCh4 (plpc2000CANdriver_RXObj_t   pReceiveBuf)
{

   // CAN Channel 4 
/*
   pReceiveBuf -> RFS = CAN4RFS;
   pReceiveBuf -> ID = CAN4RID;

   pReceiveBuf -> DataField[0] = CAN4RDA;
   pReceiveBuf -> DataField[1] = CAN4RDB;

   // Release Receive Buffer 

   CAN4CMR = 0x04;          // Release Rx Buffer

*/
   return (1);
}   // lpc2000CANdriver_ReceiveMessageCh4; 





// **************************************************************************
// FUNCTION:    lpc2000CANdriver_LoadAcceptanceFilter                             
//                                                                           
// DESCRIPTION: The function lpc2000hwCANdriver_LoadAcceptanceFilter configures   
//              the Acceptance Filter Memory. Filter constants which are     
//              pre-defined in the configuration file lpc2000CANdriver_cfg.h      
//              are used for programming.                                    
//              In addition to the filter memory configuration, this         
//              function also configures the acceptance filter start address 
//              registers according to the number of CAN identifiers.        
//                                                                           
// GLOBAL VARIABLES USED:                                                    
//                                                                           
// PARAMETERS:                                                               
//                                                                           
// RETURN:      CanStatusCode: Status of operation                           
//                                                                           
//              LPC2000_CANDRIVER_OK     - successful                             
//                                                                           
//              LPC2000_CANDRIVER_ERR    - not successful -> table error          
// **************************************************************************

CanStatusCode
lpc2000CANdriver_LoadAcceptanceFilter (void)
{
   UInt32 address;
   UInt32 memory_address;
   UInt32 IDcount;
   UInt32 i;
   UInt32 acfword_pre;
   UInt32 acfword_upper;
   UInt32 acfword_lower;


   address = 0;

   // Set Standard Frame Individual Startaddress 
   SFF_sa = address;

   // ----------------------------------------------------------
   // ----------   Fill Standard Individual Section ------------
   // ----------------------------------------------------------

   #ifdef LPC2000_CANDRIVER_STD_INDIVIDUAL

   IDcount = LPC2000_CANDRIVER_NUMBER_OF_STD_INDIVIDUAL_IDS;
   acfword_pre = 0;
   i = 0;

   while (IDcount != 0)
   {
      acfword_lower = (gklpc2000CANdriver_StdIndividualSection[i].ID << 16)
                    + (gklpc2000CANdriver_StdIndividualSection[i].Channel << 29);

     IDcount--;
      if (IDcount == 0)
      {
         // odd number of identifiers -> disable ID ! 
         acfword_upper = 0x00001FFF
                       + (gklpc2000CANdriver_StdIndividualSection[i].Channel << 13);
      }
      else
      {
         acfword_upper = (gklpc2000CANdriver_StdIndividualSection[i+1].ID)
                       + (gklpc2000CANdriver_StdIndividualSection[i+1].Channel << 13);
         IDcount--;
      }

      // Check ascending numerical order 
      if ((acfword_lower >> 16) >= acfword_upper)
      {
         // table error - violation of ascending numerical order 
         return(LPC2000_CANDRIVER_ERR_TABLE_ERROR_IN_STD_INDIVIDUAL_SECTION);
      }
      else
      {
         if (acfword_pre >= (acfword_lower >> 16))
         {
            // table error - violation of ascending numerical order 
            return(LPC2000_CANDRIVER_ERR_TABLE_ERROR_IN_STD_INDIVIDUAL_SECTION);
         }
         else
         {
            // write configuration into Acceptance Filter Memeory 
            OUTW( CAFMEM + address, acfword_lower + acfword_upper);
            acfword_pre = acfword_upper;
         }
      }

      // increment configuration table index 
      i = i + 2;
      // next Acceptance Filter Memory address 
      address = address + 4;
   };

   #endif

   // Set Standard Frame Group Startaddress 
   SFF_GRP_sa = address;

   // ----------------------------------------------------------
   // ----------   Fill Standard Group Section -----------------
   // ----------------------------------------------------------

   #ifdef LPC2000_CANDRIVER_STD_GROUP

   IDcount = LPC2000_CANDRIVER_NUMBER_OF_STD_GROUP_IDS;
   acfword_pre = 0;
   i = 0;

   while (IDcount != 0)
   {
      acfword_lower = (gklpc2000CANdriver_StdGroupSection[i].ID << 16)
                    + (gklpc2000CANdriver_StdGroupSection[i].Channel << 29);

      IDcount--;
      if (IDcount == 0)
      {
         // table error - odd number of identifiers 
         // not allowed for group definitions 
         return (LPC2000_CANDRIVER_ERR_TABLE_ERROR_IN_STD_GROUP_SECTION);
      }
      else
      {
         acfword_upper = (gklpc2000CANdriver_StdGroupSection[i+1].ID)
                       + (gklpc2000CANdriver_StdGroupSection[i+1].Channel << 13);
         IDcount--;
      }

      // Check ascending numerical order 
      if ((acfword_lower >> 16) >= acfword_upper)
      {
         // table error - violation of ascending numerical order 
         return(LPC2000_CANDRIVER_ERR_TABLE_ERROR_IN_STD_GROUP_SECTION);
      }
      else
      {
         if (acfword_pre >= (acfword_lower >> 16))
         {
            // table error - violation of ascending numerical order 
            return(LPC2000_CANDRIVER_ERR_TABLE_ERROR_IN_STD_GROUP_SECTION);
         }
         else
         {
            // write configuration into Acceptance Filter Memory 
            OUTW(CAFMEM + address, acfword_lower + acfword_upper);
            acfword_pre = acfword_upper;
         }
      }

     // increment configuration table index 
     i = i + 2;
     // next Acceptance Filter Memory address 
     address = address + 4;
   };

   #endif

   // Set Extended Frame Individual Startaddress 
   EFF_sa = address;

   // ----------------------------------------------------------
   // ----------   Fill Extended Individual Section ------------
   // ----------------------------------------------------------

   #ifdef LPC2000_CANDRIVER_EXT_INDIVIDUAL

   IDcount = LPC2000_CANDRIVER_NUMBER_OF_EXT_INDIVIDUAL_IDS;
   i = 0;
   acfword_pre = 0;

   while (IDcount != 0)
   {
      acfword_lower = (gklpc2000CANdriver_ExtIndividualSection[i].IDx)
                    + (gklpc2000CANdriver_ExtIndividualSection[i].Channel << 29);

      IDcount--;

      // Check ascending numerical order with previous table entity 
      if ((i > 0) && (acfword_pre >= acfword_lower))
      {
         // table error - violation of ascending numerical order 
         return(LPC2000_CANDRIVER_ERR_TABLE_ERROR_IN_EXT_INDIVIDUAL_SECTION);
      }
      else
      {
         OUTW(CAFMEM + address, acfword_lower);
         // next Acceptance Filter Memory address 
         address = address + 4;

         // increment configuration table index 
         i = i + 1;
         acfword_pre = acfword_lower;

      }
   };

   #endif

   // Set Extended Frame Group Startaddress 
   EFF_GRP_sa = address;

   // ----------------------------------------------------------
   // ----------   Fill Extended Group Section -----------------
   // ----------------------------------------------------------

   #ifdef LPC2000_CANDRIVER_EXT_GROUP

   IDcount = LPC2000_CANDRIVER_NUMBER_OF_EXT_GROUP_IDS;
   i = 0;
   acfword_pre = 0;

   while (IDcount != 0)
   {
      acfword_lower = (gklpc2000CANdriver_ExtGroupSection[i].IDx)
                    + (gklpc2000CANdriver_ExtGroupSection[i].Channel << 29);

      // Check ascending numerical order with previous group 
      if ((i > 0) && (acfword_pre >= acfword_lower))
      {
         // table error - violation of ascending numerical order 
         return (LPC2000_CANDRIVER_ERR_TABLE_ERROR_IN_EXT_GROUP_SECTION);
      }
      else
      {
         IDcount--;

         if (IDcount == 0)
         {
            // table error 'odd number of entities' 
            return (LPC2000_CANDRIVER_ERR_TABLE_ERROR_IN_EXT_GROUP_SECTION);
         }
         else
         {
            acfword_upper = (gklpc2000CANdriver_ExtGroupSection[i+1].IDx)
                          + (gklpc2000CANdriver_ExtGroupSection[i].Channel << 29);

         // Check ascending numerical order 
         if (acfword_lower >= acfword_upper)
         {
            // table error - violation of ascending numerical order 
            return (LPC2000_CANDRIVER_ERR_TABLE_ERROR_IN_EXT_GROUP_SECTION);
         }
         else
         {
            // write configuration into Acceptance Filter Memeory 
            OUTW(CAFMEM + address, acfword_lower);

            // next Acceptance Filter Memory address 
            address = address + 4;
            // write configuration into Acceptance Filter Memory 
            OUTW(CAFMEM + address, acfword_upper);

            // next Acceptance Filter Memory address 
            address = address + 4;

            acfword_pre = acfword_upper;
         }
       }
   }

     // increment configuration table index 
     i = i + 2;
     IDcount--;
   };

   #endif

   // Check Acceptance Filter Memory Space 
   memory_address = address ;

   if (memory_address > LPC2000_CANDRIVER_SIZE_OF_ACFMEMORY)
   {
      // not enough memory space for CAN Identifiers 
      return (LPC2000_CANDRIVER_ERR_NOT_ENOUGH_MEMORY_SPACE);
   }
   else
   {
      // Set End of Table 
      ENDofTable = address;
   }

   return (LPC2000_CANDRIVER_OK);
}


// **************************************************************************
// FUNCTION:    Rcv_Data_Output_to_Screen                             
//                                                                           
// DESCRIPTION: Print Data to PC Screen by Uart0     
//                                                                           
// GLOBAL VARIABLES USED:                                                    
//                                                                           
// PARAMETERS:                                                               
//                                                                           
// RETURN:           
// **************************************************************************
UInt32 Rcv_Data_Output_to_Screen(plpc2000CANdriver_RXObj_t   pReceiveBuf)
{
  UInt32 Temp_Data_Output[2], i;


  //Uart Output for CAN data define
  UInt8 const Rcv_Data_To_Screen_1[] = "***Rcv-Frame:Data-";
  UInt8 const Rcv_Data_To_Screen_2[] = " ID:";
  UInt8 const Rcv_Data_To_Screen_3[] = "  BP-Bit:";
  UInt8 const Rcv_Data_To_Screen_4[] = "  ID-Index:";
  UInt8 const Rcv_Data_To_Screen_5[] = "\n\r";


  Print_Chars_to_Screen( Rcv_Data_To_Screen_1 );
  Temp_Data_Output[0] = pReceiveBuf -> DataField[0];
  Temp_Data_Output[1] = pReceiveBuf -> DataField[1];
  pReceiveBuf -> DataField[0] = 0;                   // clear Data0 in Rcv data Buffer
  pReceiveBuf -> DataField[1] = 0;					 // clear Data1 in Rcv data Buffer
  for(i=0;i<2;i++)																
    {
	  Print_4bits_to_Screen( (Temp_Data_Output[i] & 0x000000F0)>>4 ); // print 2nd 4bits
	  Print_4bits_to_Screen( (Temp_Data_Output[i] & 0x0000000F) );    // print 1st 4bits
	  Print_4bits_to_Screen( 44 );   								  // print "," 
	  Print_4bits_to_Screen( (Temp_Data_Output[i] & 0x0000F000)>>12 );// print 4th 4bits
	  Print_4bits_to_Screen( (Temp_Data_Output[i] & 0x00000F00)>>8 ); // print 3rd 4bits
	  Print_4bits_to_Screen( 44 );   								  // print ","
	  Print_4bits_to_Screen( (Temp_Data_Output[i] & 0x00F00000)>>20 );// print 6th 4bits
	  Print_4bits_to_Screen( (Temp_Data_Output[i] & 0x000F0000)>>16 );// print 5th 4bits
	  Print_4bits_to_Screen( 44 );   								  // print ","
	  Print_4bits_to_Screen( (Temp_Data_Output[i] & 0xF0000000)>>28 );// print 8th 4bits
	  Print_4bits_to_Screen( (Temp_Data_Output[i] & 0x0F000000)>>24 );// print 7th 4bits
	  Print_4bits_to_Screen( 44 );									  // print ","
	     								  
	}

  Print_Chars_to_Screen( Rcv_Data_To_Screen_2 );  
  Temp_Data_Output[0] = pReceiveBuf -> ID;		  // clear ID in Rcv data Buffer
  pReceiveBuf -> ID = 0;
  Print_4bits_to_Screen( (Temp_Data_Output[0] & 0xF0000000)>>28 ); // print ID 8th 4bits
  Print_4bits_to_Screen( (Temp_Data_Output[0] & 0x0F000000)>>24 ); // print ID 7th 4bits
  Print_4bits_to_Screen( (Temp_Data_Output[0] & 0x00F00000)>>20 ); // print ID 6th 4bits
  Print_4bits_to_Screen( (Temp_Data_Output[0] & 0x000F0000)>>16 ); // print ID 5th 4bits
  Print_4bits_to_Screen( (Temp_Data_Output[0] & 0x0000F000)>>12 ); // print ID 4th 4bits
  Print_4bits_to_Screen( (Temp_Data_Output[0] & 0x00000F00)>>8 );  // print ID 3rd 4bits 
  Print_4bits_to_Screen( (Temp_Data_Output[0] & 0x000000F0)>>4 );  // print ID 2nd 4bits 
  Print_4bits_to_Screen( (Temp_Data_Output[0] & 0x0000000F) );     // print ID 1st 4bits 

  Print_Chars_to_Screen( Rcv_Data_To_Screen_3 );
  Temp_Data_Output[0] = pReceiveBuf -> RFS;		 // clear RFS in Rcv data Buffer
  pReceiveBuf -> RFS = 0;
  i = (Temp_Data_Output[0] & 0x00000400)>>10;					   // save BP bit
  Print_4bits_to_Screen( (Temp_Data_Output[0] & 0x00000400)>>10 ); // print BP bit

  Print_Chars_to_Screen( Rcv_Data_To_Screen_4 );
  if( i == 0)
    {
      Temp_Data_Output[0] &= 0x000003FF;
	  Temp_Data_Output[1] = Temp_Data_Output[0] / 1000;
      Print_4bits_to_Screen( Temp_Data_Output[1] );      // print ID Index thousand digit
	  Temp_Data_Output[0] %= 1000;
      Temp_Data_Output[1] = Temp_Data_Output[0] / 100;
      Print_4bits_to_Screen( Temp_Data_Output[1] );      // print ID Index centi-digit
	  Temp_Data_Output[0] %= 100;
      Temp_Data_Output[1] = Temp_Data_Output[0] / 10;
      Print_4bits_to_Screen( Temp_Data_Output[1] );      // print ID Index deca-digit
	  Temp_Data_Output[0] %= 10;
      Print_4bits_to_Screen( Temp_Data_Output[0] );      // print ID Index below ten digit
    }
  else
    { for(i=0;i<4;i++) Print_4bits_to_Screen( 45 );}	 // print 4 "-"

  Print_Chars_to_Screen( Rcv_Data_To_Screen_5 );		 // change line + line feed

  return(0);
}

// **************************************************************************
// FUNCTION:    Print_Chars_to_Screen                             
//                                                                           
// DESCRIPTION: Print Chars to Screen by Uart0     
//                                                                           
// GLOBAL VARIABLES USED:                                                    
//                                                                           
// PARAMETERS:                                                               
//                                                                           
// RETURN:           
// **************************************************************************
UInt32 Print_Chars_to_Screen(  UInt8 const * pscreen_print )
{
  while( *pscreen_print != '\0' )
    {
	  U0THR = *pscreen_print;
	  while( (U0LSR & 0x20) == 0 );
	  pscreen_print++;
	}
 return(0);
}

// **************************************************************************
// FUNCTION:    Print_4bits_to_Screen                             
//                                                                           
// DESCRIPTION: Print 4bits hex data to Screen by Uart0     
//                                                                           
// GLOBAL VARIABLES USED:                                                    
//                                                                           
// PARAMETERS:                                                               
//                                                                           
// RETURN:           
// **************************************************************************
UInt32 Print_4bits_to_Screen( UInt32 Value_Hex_4bits)
{
  switch(Value_Hex_4bits)
    {
	  case 0:
	  case 1:
	  case 2:
	  case 3:
	  case 4:
	  case 5:
	  case 6:
	  case 7:
	  case 8:
	  case 9: U0THR = Value_Hex_4bits + 48;  // print "0 - 9"
	    break;
	  case 10:
	  case 11:
	  case 12:
	  case 13:
	  case 14:
	  case 15: U0THR = Value_Hex_4bits + 55; // print "A - F"
	    break;
      case 44:                               // print ","
      case 45: U0THR = Value_Hex_4bits;      // print "-"
		break;
	  default:	    
	    break;
	}
  while( (U0LSR & 0x20) == 0 );
  return(0);
}