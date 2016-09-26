#include "CScifu.hpp"
#include "CLpc21xxUartDev.hpp"

/*----------------------------------------------------------------------------*/
/* CONSTANTS                                                                  */
/*----------------------------------------------------------------------------*/
/* Software FIFO size, in bytes */
#define LPC214x_FIFO_SIZE                         64
#define LPC214x_HW_FIFO_SIZE                      16

#define LPC214x_BAUD_RATE_MULVAL                  1
#define LPC214x_BAUD_RATE_DIVADDVAL               1

/*
Registers offset
*/
#define UxRBR 0x00
#define UxTHR 0x00
#define UxDLL 0x00
#define UxDLM 0x04
#define UxIER 0x04
#define UxIIR 0x08
#define UxFCR 0x08
#define UxLCR 0x0C
#define UxLSR 0x14
#define UxACR 0x20
#define UxFDR 0x28
#define UxTER 0x30

/*
REGISTERS MASKS
*/

#define UxLSR_THRE_MASK       (1 << 5)
#define UxLSR_RDR_MASK        (1 << 0)

/*----------------------------------------------------------------------------*/
/* MACROS                                                                     */
/*----------------------------------------------------------------------------*/
#define LPC214x_BAUD_RATE_DIVIDER(_baudRate)      (m_PClock / (16 * (_baudRate) * (1 + LPC214x_BAUD_RATE_DIVADDVAL / LPC214x_BAUD_RATE_MULVAL) ))
#define LPC214x_BAUD_RATE_DIVIDER_LSB(_baudRate)  (uint8_t)(LPC214x_BAUD_RATE_DIVIDER(_baudRate) & 255)
#define LPC214x_BAUD_RATE_DIVIDER_MSB(_baudRate)  (uint8_t)((LPC214x_BAUD_RATE_DIVIDER(_baudRate) & 0xFF00) / 256)

#define LPC214x_REG_READ_8(_reg)                  (mp_BaseAddr[_reg])
#define LPC214x_REG_WRITE_8(_reg, _value)         ((mp_BaseAddr[_reg]) = _value)

uint32_t    CLpc21xxUartDev::m_PClock = 60000000;

CLpc21xxUartDev::CLpc21xxUartDev(
   uint8_t * const p_BaseAddress,
   const uint32_t   pclock
   )
{
   mp_BaseAddr  = p_BaseAddress;
   m_PClock     = pclock;
   m_BaudRate   = 9600;
   m_PollMode   = true;
   Init();
}

CLpc21xxUartDev::CLpc21xxUartDev()
{
   mp_BaseAddr = NULL;
} // CLpc21xxUartDev::CLpc21xxUartDev

int8_t CLpc21xxUartDev::Init(void)
{
   int8_t   result = -1;
   uint32_t reg = 0;

   /* Initialize all registers */
      
    /* UxIER */
    /* Disable all interrupts */
    LPC214x_REG_WRITE_8(UxIER, 0);

    /* UxIIR */
    /* Clear all interrupts */
    reg = LPC214x_REG_READ_8(UxIIR);

    /* UxFCR */
    /* Enable FIFOs, reset FIFOs, trigger level 0 */
    LPC214x_REG_WRITE_8(UxFCR, 0x07);

    /* UxRBR */
    reg = LPC214x_REG_READ_8(UxRBR);

    /* UxTHR */
    /* nop */

    /* UxDLL */
    /* UxDLM */
    /* Set baud rate */
    LPC214x_REG_WRITE_8(UxLCR, 0x80);
    /* Only consider the input baud rate */

    // TODO: fix the baud rate
    uint8_t myreg = LPC214x_BAUD_RATE_DIVIDER_LSB(m_BaudRate);
    LPC214x_REG_WRITE_8(UxDLL, myreg);
    myreg = LPC214x_BAUD_RATE_DIVIDER_MSB(m_BaudRate);
    LPC214x_REG_WRITE_8(UxDLM, myreg);

    /* UxLCR */
    /* TODO: FOR NOW ONLY SUPPORT A SUBSET */
    LPC214x_REG_WRITE_8(UxLCR, 0x03);

    /* UxFDR */
    LPC214x_REG_WRITE_8(UxFDR, 0); /* TODO: TBD */

    /* UxLSR */
    reg = LPC214x_REG_READ_8(UxLSR);

    /* UxSCR */
    /* nop */

    /* UxACR */
    LPC214x_REG_WRITE_8(UxACR, 0);

    /* UxTER */
    /* Enable transmit */
    LPC214x_REG_WRITE_8(UxTER, 0x80);

   result = 0;
   return result;
} // CLpc21xxUartDev::Init

CLpc21xxUartDev::~CLpc21xxUartDev(void)
{} // CLpc21xxUartDev::~CLpc21xxUartDev

int32_t CLpc21xxUartDev::Read(
   void * const   	p_Data,
   size_t         	nBytes
   )
{
   int32_t	result 		= -1;
   uint32_t	bytesRead	= 0;
   
   /*----------------------------------------------------------------------*/
   if(true == m_PollMode)
   /*----------------------------------------------------------------------*/
   {
      while(bytesRead < nBytes)
      {
	     while(0 == (LPC214x_REG_READ_8(UxLSR) & UxLSR_RDR_MASK))
	     {}
  
         reinterpret_cast<uint8_t *>(p_Data)[bytesRead] = LPC214x_REG_READ_8(UxRBR);
         bytesRead++;
      }
      
      result = bytesRead;
   }
   /*----------------------------------------------------------------------*/
   else
   /*----------------------------------------------------------------------*/
   {
      /* TODO: support this mode */
   }
   
   return result;
} // CLpc21xxUartDev::Read

int32_t CLpc21xxUartDev::Write(
   void const * const   p_Data,
   size_t               nBytes
   )
{
   uint32_t   bytesWritten   = 0;

   /*----------------------------------------------------------------------*/
   if(true == m_PollMode)
   /*----------------------------------------------------------------------*/
   {
      while(bytesWritten < nBytes)
      {
        /* Wait until there is room in the transmitt FIFO */
        while(0 == (LPC214x_REG_READ_8(UxLSR) & UxLSR_THRE_MASK))
        {}

        bytesWritten += SendData((uint8_t *)(p_Data) + bytesWritten, nBytes - bytesWritten);
      } /* while(bytesWritten < dataSize) */
   }
   /*----------------------------------------------------------------------*/
   else
   /*----------------------------------------------------------------------*/
   {
      /* TODO: support this mode */
   }

   return bytesWritten;
} // CLpc21xxUartDev::Write

int8_t CLpc21xxUartDev::SetBaudRate(
   const uint32_t baudRate
   )
{
   m_BaudRate = baudRate;
   Init();
   return 0;
} // CLpc21xxUartDev::SetBaudRate

uint32_t CLpc21xxUartDev::GetBaudRate(void)
{
   return m_BaudRate;
} // CLpc21xxUartDev::GetBaudRate

size_t CLpc21xxUartDev::SendData(
   void const * const             p_Data, /* in */
   const size_t                   nBytes  /* in */
   )
{
   size_t          bytesToWrite  = 0;
   size_t          bytesWritten  = 0;

   if(nBytes > LPC214x_HW_FIFO_SIZE)
   {
      bytesToWrite = LPC214x_HW_FIFO_SIZE;
   }
   else
   {
      bytesToWrite = nBytes;
   }
  
   while(bytesWritten < bytesToWrite)
   {
      LPC214x_REG_WRITE_8(UxTHR, ((uint8_t *)p_Data)[bytesWritten]);
      //LPC214x_REG_WRITE_8(UxTHR, 'a');
      bytesWritten++;
   } /* while(bytesWritten < bytesToWrite) */

  return bytesWritten;
}

size_t CLpc21xxUartDev::ReceiveData(
   void * const	p_Data, /* in */
   const size_t		nBytes  /* in */
   )
{
   size_t          bytesRead  = 0;
   
   while(UxLSR_RDR_MASK == LPC214x_REG_READ_8(UxLSR) & UxLSR_RDR_MASK &&
          bytesRead < nBytes
         )
   {
      reinterpret_cast<uint8_t *>(p_Data)[bytesRead] = LPC214x_REG_READ_8(UxRBR);
      bytesRead++;
   }
   
  return bytesRead;
}
