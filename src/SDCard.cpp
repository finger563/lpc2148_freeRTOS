////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE NAME:     $Source:$
//                $Date:$
//                $Revision:$
//
// AUTHOR:        Jean-Sebastien Stoezel (js.stoezel@gmail.com)
//
// HISTORY:       $Log:$
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include "SDCard.hpp"
#include "CScifuSpiDevice.hpp"
#include "CScifu.hpp"
#include "LPC214x.h"
#include "CLog.hpp"


#define SD_CARD_CMD_GO_IDLE_STATE       (0)
#define SD_CARD_CMD_SEND_OP_COND        (1)
#define SD_CARD_CMD_SET_BLOCKLEN        (16)
#define SD_CARD_CMD_READ_SINGLE_BLOCK   (17)
#define SD_CARD_CMD_WRITE_SINGLE_BLOCK  (24)
#define SD_CARD_CMD_READ_OCR            (58)

typedef enum
{
   SD_CARD_RESPONSE_R1,
   SD_CARD_RESPONSE_R1B,
   SD_CARD_RESPONSE_R2,
   SD_CARD_RESPONSE_R3
} eSdCardResponseType_t;

CScifuSpiDevice * p_SpiDev = NULL;
/*
********************************************************************************
*
* FUNCTION NAME:  SdCardGoIdleState
*
* DESCRIPTION:    
*
* ASSUMPTION:
*
********************************************************************************
*/
int32_t SdCardGoIdleState(void);

/*
********************************************************************************
*
* FUNCTION NAME:  SdCardSetBlockLen
*
* DESCRIPTION:    
*
* ASSUMPTION:
*
********************************************************************************
*/
int32_t SdCardSetBlockLen(
   const uint32_t slot,
   uint32_t     blockLen
);

/*
********************************************************************************
*
* FUNCTION NAME:  SdCardSendOpCond
*
* DESCRIPTION:    
*
* ASSUMPTION:
*
********************************************************************************
*/
int32_t SdCardSendOpCond(void);

/*
********************************************************************************
*
* FUNCTION NAME:  SdCardRespReceive
*
* DESCRIPTION:    
*
* ASSUMPTION:
*
********************************************************************************
*/
int32_t SdCardRespReceive(
   const eSdCardResponseType_t  cmdType,
   const uint8_t                expectedResp
);

/*
********************************************************************************
*
* FUNCTION NAME:  SdCardCmdSend
*
* DESCRIPTION:    
*
* ASSUMPTION:
*
********************************************************************************
*/
int32_t SdCardCmdSend(
   const uint8_t              cmd,
  uint8_t const * const arg
);

/*
********************************************************************************
*
* FUNCTION NAME:  SdCardResp1Get
*
* DESCRIPTION:    
*
* ASSUMPTION:
*
********************************************************************************
*/
int32_t SdCardRespGet(
   const uint32_t                slot,     /* in */
   const uint8_t               cmd,
    uint8_t const * const arg
);

/*
********************************************************************************
*
* FUNCTION NAME:  SdCardSlotInit
*
********************************************************************************
*/
int32_t SdCardSlotInit(void)
{
   int32_t result = -1;
   uint32_t cnt = 0xFF;
   
   LOG_TRACE("SdCardSlotInit");

   /* TODO: Should we scan all spi devices for a SD card? */
   if(0 == CScifu::GetDevice(&p_SpiDev, "spi0"))
   {
      CScifu_Info("SPI0 found");
   }
   else
   {
      CScifu_Error("SPI0 not found");
   }
   
   if(0 == p_SpiDev->SetBaudRate(600*1000))
   {
      //rprintf("\r\nSPI0 setbaud rate");
	   /* For all devices */
	   /*
	   --------------
	   Reset Sequence
	   --------------
	   The microSD Card requires a defined reset sequence. After power-on reset or
	   software reset (CMD0), the card enters an idle state. In this state, the
	   only legal host commands are CMD1 (SEND_OP_COND), ACMD41 (SD_SEND_OP_COND),
	   CMD59 (CRC_ON_OFF) and CMD58 (READ_OCR).
	   The host must poll the card by repeatedly sending CMD1 until the ?in-idle-state?
	   bit in the card response indicates, by being set to 0, that the card completed
	   its initialization processes and is ready for the next command.
	   However, in SPI mode, CMD1 has no operands and does not return the contents of
	   the OCR Register. Instead, the host can use CMD58 (SPI Mode Only) to read the
	   register. It is the responsibility of the host to refrain from accessing cards
	   that do not support its voltage range.
	   The use of CMD58 is not restricted to the initialization phase only, but can be
	   issued at any time. The host must poll the card by repeatedly sending CMD1
	   until the ?in-idle-state? bit in the card response indicates, by being set to 0,
	   that the card has completed its initialization process and is ready for the next
	   command.
	   */
	
	   //rprintf("\r\nSPI0 before idle state");
	   
	   
	   while(0 != SdCardGoIdleState())
	   {}
	   
	   //rprintf("\r\nSPI0 after idle state");
	
	   
	      /* Poll the card until it's done initializing. */
	      cnt = 0xFF;
	      while(0 != cnt)
	      {
	         if(0 == SdCardSendOpCond())
	         {
			    //rprintf("\r\nSD card initialized");
	            result = 0;
	            break;
	         }
	         cnt--;
	      } /* while(0 != cnt) */ 
	
	/*      SdCardReadOcr(0, &dummy);*/
	
	/*      if(0 != cnt)
	      {
	         cnt = 0xFF;
	         while(0 != cnt)
	         {
	            // Set the SD card block lenght
	            while(-1 == SdCardSetBlockLen(0, 512))
	            {
	               blockLen <<= 1;
	               SysIoPrintf("\r\nBlockLen = %u", blockLen);
	            }
	            if(0 == result)
	            {
	               break;
	            }
	            cnt--;
	         }
	      }
	*/
	//      if(0 != cnt)
	//      {
	//         /* Switch to a higher baudrate */
	//         //SpiDevInit(0, 25000000, SPI_MODE_POLL, SPI_OPERATION_MASTER);
	//      }
	   //} /* if(0 == SdCardGoIdleState(0)) */
   }
   else
   {
      CScifu_Error("SPI0 setbaud rate failed");
   }
   
   p_SpiDev->SetBaudRate(600*1000);
    
   return result;
} /* SdCardInit */

/*
********************************************************************************
*
* FUNCTION NAME:  SdCardRead
*
********************************************************************************
*/
int32_t SdCardRead(
  const uint32_t          	sector,
  uint8_t * const   		p_Data,   /* out */
  const uint32_t          	nBytes
)
{
   int32_t result = -1;
   uint32_t crc;
   uint32_t         addr = 0;
   uint8_t arg[4] = {0, 0, 0, 0};
   
   LOG_TRACE("SdCardRead");
   
//   rprintf("\r\nSdCardRead");

   addr = sector * 512; /* Replace by sector size */

   /* Block len, MSB first */
    arg[0] = (uint8_t)((addr & 0xFF000000) >> 24);
    arg[1] = (uint8_t)((addr & 0x00FF0000) >> 16);
    arg[2] = (uint8_t)((addr & 0x0000FF00) >> 8);
    arg[3] = (uint8_t)((addr & 0x000000FF));
     
     /* Send the send_op_cond */
     //SysIoPrintf("\r\nSD_CARD_CMD_READ_SINGLE_BLOCK");
     SdCardCmdSend(SD_CARD_CMD_READ_SINGLE_BLOCK, arg);
     //if(arg < 0x40000000ul)
     //{
     //   arg+=2;
     //}
     //else
     //{
     //   arg = 0;
     //}
     //SdCardCmdSend(slot, SD_CARD_CMD_READ_SINGLE_BLOCK, (uint8_t*)(&arg));
     if(0 == SdCardRespReceive(SD_CARD_RESPONSE_R1, 0x00))
     {
        /* Now receive the data bytes */
        /* DATA TOKEN */
        if(0 == SdCardRespReceive(SD_CARD_RESPONSE_R1, 0xFE))
        {
//		   rprintf("\r\nSdCardRead success");
           result = 0;
           /* Read the data */
           p_SpiDev->Read(p_Data, nBytes);
           /* Read the CRC */
           p_SpiDev->Read((uint8_t *)&crc, 2);
        }
        
     }
     //*arg++;
   return result;
} /* SdCardRead */

/*
********************************************************************************
*
* FUNCTION NAME:  SdCardWrite
*
********************************************************************************
*/
int32_t SdCardWrite(
  const uint32_t          sector,
  const uint8_t * const   p_Data,   /* in */
  const uint32_t          nBytes
)
{
   int32_t result = -1;
   uint32_t         addr = 0;
   uint8_t arg[4] = {0, 0, 0, 0};
   uint32_t cnt = 0;
   
   LOG_TRACE("SdCardWrite");

   addr = sector * 512; /* Replace by sector size */

   /* Block len, MSB first */
    arg[0] = (uint8_t)((addr & 0xFF000000) >> 24);
    arg[1] = (uint8_t)((addr & 0x00FF0000) >> 16);
    arg[2] = (uint8_t)((addr & 0x0000FF00) >> 8);
    arg[3] = (uint8_t)((addr & 0x000000FF));
     
     /* Send the send_op_cond */
     //SysIoPrintf("\r\nSD_CARD_CMD_READ_SINGLE_BLOCK");
     SdCardCmdSend(SD_CARD_CMD_WRITE_SINGLE_BLOCK, arg);
     //if(arg < 0x40000000ul)
     //{
     //   arg+=2;
     //}
     //else
     //{
     //   arg = 0;
     //}
     //SdCardCmdSend(slot, SD_CARD_CMD_READ_SINGLE_BLOCK, (uint8_t*)(&arg));
     if(0 == SdCardRespReceive(SD_CARD_RESPONSE_R1, 0x00))
     {
        /* Set bit 0 to 0 which indicates the beginning of the data block */
        arg[0] = 0xFE;
        p_SpiDev->Write(arg, 1);

        /* Send the data */
        p_SpiDev->Write(p_Data, nBytes); /*TODO: Change this hardcoded value */

        /* Send dummy checksum */
        arg[0] = 0xFF;
        arg[1] = 0xFF;
        p_SpiDev->Write(arg, 2);

        p_SpiDev->Read(arg, 1);
        if((arg[0] & 0x0F) != 0x05)
        {
           
        } /* if((arg[0] & 0xFF) != 0x05) */
        else
        {
          /* If the status is already zero, the write hasn't finished yet and card is busy */
          cnt = 0xFFFF;
          arg[0] = 0;
          while((cnt != 0) &&
                (0 == arg[0])
                )
          {
             p_SpiDev->Read(arg, 1);
             cnt--;
          }

           result = 0;
        }
     }
     //*arg++;
   return result;
} /* SdCardWrite */

/*
********************************************************************************
*
* FUNCTION NAME:  SdCardCmdSend
*
********************************************************************************
*/
int32_t SdCardCmdSend(
   const uint8_t                cmd,
   uint8_t const * const  arg
)
{
   int32_t    result = -1;
   uint8_t  buffer[8];
   
   LOG_TRACE("SdCardCmdSend");

   buffer[0] = 0xFF;
   buffer[1] = 0x40 | (cmd & 0x3F);
   memcpy(buffer + 2, arg, 4);
   buffer[6] = 0x95;
   buffer[7] = 0xFF;

   result = p_SpiDev->Write(buffer, 8);
   /* Generate 8 more cycle to allow completion of the command */
   

   return result;
} /* SdCardCmdSend */

/*
********************************************************************************
*
* FUNCTION NAME:  SdCardRespReceive
*
********************************************************************************
*/
int32_t SdCardRespReceive(
   const eSdCardResponseType_t  cmdType,
   const uint8_t                expectedResp
)
{
   int32_t    result  = -1;
   uint32_t   cnt     = 0xFF;
   uint8_t  dummy    = 0xFF;
   //SysIoPrintf("\r\nRESP:");
   
   LOG_TRACE("SdCardRespReceive");
   

     switch(cmdType)
     {
        case SD_CARD_RESPONSE_R1:
        case SD_CARD_RESPONSE_R1B:
        {
           while(0 != cnt)
           {
             cnt--;
        
             /* Read byte */
             p_SpiDev->Read(&dummy, 1);
			 //rprintf("\r\nSD repsonse = 0x%04X", dummy);
             //SysIoPrintf(" %02X", dummy);
             if(expectedResp == dummy)
             {
                cnt = 0;
                result = 0;
             }
          }
          break;
        }
        case SD_CARD_RESPONSE_R2:
        {
           break;
        }
        case SD_CARD_RESPONSE_R3:
        {
           p_SpiDev->Read(&dummy, 1);
		   //rprintf("\r\nSD repsonse = 0x%04X", dummy);
           //SysIoPrintf(" %02X", dummy);
           p_SpiDev->Read(&dummy, 1);
		   //rprintf("\r\nSD repsonse = 0x%04X", dummy);
           //SysIoPrintf(" %02X", dummy);
           p_SpiDev->Read(&dummy, 1);
		   //rprintf("\r\nSD repsonse = 0x%04X", dummy);
           //SysIoPrintf(" %02X", dummy);
           p_SpiDev->Read(&dummy, 1);
		   //rprintf("\r\nSD repsonse = 0x%04X", dummy);
           //SysIoPrintf(" %02X", dummy);
           p_SpiDev->Read(&dummy, 1);
		   //rprintf("\r\nSD repsonse = 0x%04X", dummy);
           //SysIoPrintf(" %02X", dummy);
           break;
        }
        default:
        {}
     } /* switch(cmdTypes) */

   return result;
} /* SdCardRespReceive */

/*
********************************************************************************
*
* FUNCTION NAME:  SdCardGoIdleState
*
********************************************************************************
*/
int32_t SdCardGoIdleState(void)
{
   int32_t result = -1;
   uint8_t arg[4];
   /* TODO: Should we scan all spi devices for a SD card? */
   
   LOG_TRACE("SdCardGoIdleState");

   memset(arg, 0, sizeof(arg));
   /* Send the go_idle_command */
   //SysIoPrintf("\r\nSD_CARD_CMD_GO_IDLE_STATE");
   SdCardCmdSend(SD_CARD_CMD_GO_IDLE_STATE, arg);
   
   result = SdCardRespReceive(SD_CARD_RESPONSE_R1, 0x01);
   //result = SdCardRespReceive(slot, SD_CARD_RESPONSE_R1, arg, 0x00);
   
   return result;
} /* SdCardGoIdleState */

/*
********************************************************************************
*
* FUNCTION NAME:  SdCardSendOpCond
*
********************************************************************************
*/
int32_t SdCardSendOpCond(void)
{
   uint8_t arg[4];
   int32_t result = -1;
   /* TODO: Should we scan all spi devices for a SD card? */
   
   LOG_TRACE("SdCardSendOpCond");

   memset(arg, 0, sizeof(arg));
   /* Send the send_op_cond */
   //SysIoPrintf("\r\nSD_CARD_CMD_SEND_OP_COND");
   SdCardCmdSend(SD_CARD_CMD_SEND_OP_COND, arg);
   result = SdCardRespReceive(SD_CARD_RESPONSE_R1, 0x00);
   return result;
   
} /* SdCardGoIdleState */

/*
********************************************************************************
*
* FUNCTION NAME:  SdCardSetBlockLen
*
********************************************************************************
*/
int32_t SdCardSetBlockLen(
   uint32_t     blockLen
)
{
   uint8_t arg[4];
   
   LOG_TRACE("SdCardSetBlockLen");

   /* Block len, MSB first */
   arg[3] = (uint8_t)(blockLen & 0xFF);
   arg[2] = (uint8_t)((blockLen >> 8) & 0xFF);
   arg[1] = (uint8_t)((blockLen >> 16) & 0xFF);
   arg[0] = (uint8_t)((blockLen >> 24) & 0xFF);
   
   /* Send the send_op_cond */
   //SpiRead(slot, arg, sizeof(arg));
   //SysIoPrintf("\r\nSD_CARD_CMD_SET_BLOCKLEN");
   SdCardCmdSend(SD_CARD_CMD_SET_BLOCKLEN, arg);
   return SdCardRespReceive(SD_CARD_RESPONSE_R1, 0x00);

} /* SdCardSetBlockLen */
