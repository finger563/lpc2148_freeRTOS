#include "cscifu.hpp"
#include "cLpc2xxxCanDev.hpp"
#include "CScifuVicDevice.hpp"
#include <targets/lpc2000.h>

#define LPC214x_PCB_REG_WRITE_32(_reg, _value)          (*((uint32_t volatile *)(_reg)) = _value)
#define LPC214x_PCB_REG_READ_32(_reg)                   (*((uint32_t volatile *)(_reg)))

#define LPC2xxx_CAN_CTRL_REG_WRITE_32(_reg, _value)           (mp_DevBaseAddr[_reg] = _value)
#define LPC2xxx_CAN_CTRL_REG_READ_32(_reg)                    (mp_DevBaseAddr[_reg])

#define LPC2xxx_CAN_CTRL_REG_WRITE_32_(_this, _reg, _value)           ((_this)->mp_DevBaseAddr[_reg] = _value)
#define LPC2xxx_CAN_CTRL_REG_READ_32_(_this, _reg)                    ((_this)->mp_DevBaseAddr[_reg])

#define LPC2xxx_CAN_ACC_FILTER_REG_WRITE_32(_reg, _value)     (mp_AccFilterRegs[_reg] = _value)
#define LPC2xxx_CAN_ACC_FILTER_REG_READ_32(_reg)              (mp_AccFilterRegs[_reg])

#define LPC2xxx_REG_WRITE_32(_reg, _value)              (*((uint32_t volatile *)(_reg)) = _value)
#define LPC2xxx_REG_READ_32(_reg)                       (*((uint32_t volatile *)(_reg)))

uint32_t *        CLpc21xxCanDev::mp_AccFilterRegs  = NULL;
uint32_t          CLpc21xxCanDev::m_Pclk            = 0;
bool              CLpc21xxCanDev::m_CommonInitFlag  = false;

CLpc21xxCanDev::CLpc21xxCanDev(
   const uint32_t accFilterRam,
   const uint32_t accFilterRegs,
   const uint32_t centralCanRegs,
   const uint32_t baseAddr,
   const uint32_t pclk,
   const uint32_t rxIntVector,
   const uint32_t rxIntSource,
   const uint32_t txIntVector,
   const uint32_t txIntSource
   )
{
   mp_DevBaseAddr   = reinterpret_cast<uint32_t *>(baseAddr);
   mp_AccFilterRegs = reinterpret_cast<uint32_t *>(accFilterRegs);
   m_Pclk           = pclk;
   m_RxIntVector    = rxIntVector;
   m_RxIntSource    = rxIntSource;
   m_TxIntVector    = txIntVector;
   m_TxIntSource    = txIntSource;
   RingBufInit(&m_RxFifo, m_RxBuffer, sizeof(m_RxBuffer));
   RingBufInit(&m_TxFifo, m_TxBuffer, sizeof(m_TxBuffer));
   m_TxTransmitting = false;

} // CLpc21xxCanDev::CLpc21xxCanDev

CLpc21xxCanDev::~CLpc21xxCanDev(void)
{
   mp_DevBaseAddr = NULL;
}

void CLpc21xxCanDev::Isr(
   uint32_t arg
)
{
   CLpc21xxCanDev *   p_This  = reinterpret_cast<CLpc21xxCanDev *>(arg);
   uint32_t           reg     = 0;
   sScifuCanMsg_t     msg;

   reg = LPC2xxx_CAN_CTRL_REG_READ_32_(p_This, CANICR);
   //SysIo::Printf("\r\nisr_reg_sts=%08X\r\n", reg);

   // Check if there is anything to receive
   if(0x01 == (reg & 0x01))
   {
      //SysIo::Printf("\r\nrx\r\n");
      // Read the message.
      if(0 == p_This->Read(msg))
      {
         // Push it in the FIFO
         if(sizeof(msg) == RingBufPut(&(p_This->m_RxFifo), &msg, sizeof(msg)))
         {}
         else
         {
            //SysIo::Printf("\r\nfailed to FIFO\r\n");
         }
      }
      else
      {
         //SysIo::Printf("\r\nfailed to rx\r\n");
      }
   }
   // Anything to transmit
   else
   {}

   if(0 != (reg & 0x602))
   {
      //SysIo::Printf("\r\ntx\r\n");
      // Get data out of the transmit FIFO
      if(sizeof(sScifuCanMsg_t) == RingBufGet(&(p_This->m_TxFifo), &msg, sizeof(sScifuCanMsg_t)))
      {
         if(0 == p_This->Write(msg))
         {}
         else
         {
            //SysIo::Printf("\r\nfailed to tx\r\n");
         }
      }
      else
      {
         // Nothing to send
         p_This->m_TxTransmitting = false;
         //SysIo::Printf("\r\nnothing to tx\r\n");
      }
   }
   else
   {
      //SysIo::Printf("\r\nother int\r\n");
   }

   // TODO: take in charge the rest of the interrupts


} // CLpc21xxCanDev::RxIsr

CLpc21xxCanDev::CLpc21xxCanDev(void)
{}

int8_t CLpc21xxCanDev::Read(
   sScifuCanMsg_t * p_Msg,
   const uint8_t    numMsg
   )
{
   int32_t result = -1;

   //SysIo::Printf("\r\nCLpc21xxCanDev::Read");

   if(NULL != p_Msg)
   {
      result = RingBufGet(&m_RxFifo, p_Msg, numMsg * sizeof(sScifuCanMsg_t));
      
      result /= sizeof(sScifuCanMsg_t);
   }
   else
   {
      CScifu_Error("\r\nmsg is NULL\r\n");
   }

   return (int8_t)result;
}

int8_t CLpc21xxCanDev::Read(
   sScifuCanMsg_t * p_Msg,
   const uint8_t    numMsg,
   const uint32_t   timeout
   )
{
   return -1;
} // CLpc21xxCanDev::Read

int8_t CLpc21xxCanDev::Write(
   sScifuCanMsg_t * p_Msg,
   const uint8_t    numMsg
   )
{
   int8_t result = -1;

   if(NULL != p_Msg)
   {
      result = RingBufPut(&m_TxFifo, p_Msg, numMsg * sizeof(sScifuCanMsg_t));
      result /= sizeof(sScifuCanMsg_t);

      // check if the transmit put is started
      if(false == m_TxTransmitting)
      {
         sScifuCanMsg_t msg;
         // Not transmitting.. Initiate
         if(sizeof(sScifuCanMsg_t) == RingBufGet(&m_TxFifo, &msg, sizeof(sScifuCanMsg_t)))
         {
            if( 0 == Write(msg))
            {}
            else
            {}
         }
         else
         {}
      }
      else
      {}
   }
   else
   {}

   return result;
}

int8_t CLpc21xxCanDev::Read(
   sScifuCanMsg_t & msg
   )
{
   int8_t   result  = -1;
   uint32_t reg     = 0;

   // Check if there is any data available from the RX buffer
   if(0x00000001 == (LPC2xxx_CAN_CTRL_REG_READ_32(CANGSR) & 0x00000001))
   {
      reg = LPC2xxx_CAN_CTRL_REG_READ_32(CANRFS);

      msg.dlc = (reg & 0x000F0000) >> 16;

      if(0 == (reg & (1 << 30)))
      {
         msg.rtr = false;
      }
      else
      {
         msg.rtr = true;
      }

      if(0 == (reg & (1 << 31)))
      {}
      else
      {}

      msg.id = LPC2xxx_CAN_CTRL_REG_READ_32(CANRID);

      reg = LPC2xxx_CAN_CTRL_REG_READ_32(CANRDA);
      msg.data[0] = (0x000000FF & reg);
      msg.data[1] = (0x0000FF00 & reg) >> 8;
      msg.data[2] = (0x00FF0000 & reg) >> 16;
      msg.data[3] = (0xFF000000 & reg) >> 24;

      reg = LPC2xxx_CAN_CTRL_REG_READ_32(CANRDB);
      msg.data[4] = (0x000000FF & reg);
      msg.data[5] = (0x0000FF00 & reg) >> 8;
      msg.data[6] = (0x00FF0000 & reg) >> 16;
      msg.data[7] = (0xFF000000 & reg) >> 24;

      // Release the receive buffer
      LPC2xxx_CAN_CTRL_REG_WRITE_32(CANCMR, 1 << 2);

      //SysIo::Printf("\r\nRead on %08x\r\n", this->mp_DevBaseAddr);

      result = 0;
   }
   else
   {}

   return result;
} // CLpc21xxCanDev::Read

int8_t CLpc21xxCanDev::Write(
   sScifuCanMsg_t & msg
   )
{
   int8_t   result      = -1;
   uint32_t bitList[3]  = {1 << 2, 1 << 10, 1 << 18};
   uint32_t i           = 0;
   uint32_t reg         = 0;

   // Check if any TX FIFO is available
   for(i = 0; i < 3; i++)
   {
      reg = LPC2xxx_CAN_CTRL_REG_READ_32(CANSR);

      if(0 != (reg & bitList[i]))
      {
         reg = 0;

         // This spot is available
         if(msg.id >= (1 << 11))
         {
            // This is an extended identifier
            reg = (1 << 31);
         }
         else
         {}

         if(true == msg.rtr)
         {
            reg |= (1 << 30);
         }
         else
         {}

         reg |= 0x000F0000 & (msg.dlc << 16);

         LPC2xxx_CAN_CTRL_REG_WRITE_32(CANTFI1 + i * 4, reg);

         LPC2xxx_CAN_CTRL_REG_WRITE_32(CANTID1 + i * 4, msg.id);

         LPC2xxx_CAN_CTRL_REG_WRITE_32(CANTDA1 + i * 4, msg.data[0] | (msg.data[1] << 8) | (msg.data[2] << 16) | (msg.data[3] << 24));

         LPC2xxx_CAN_CTRL_REG_WRITE_32(CANTDB1 + i * 4, msg.data[4] | (msg.data[5] << 8) | (msg.data[6] << 16) | (msg.data[7] << 24));

         // Request the message to be sent
         reg = (0x01 | (0x20 << i));
         LPC2xxx_CAN_CTRL_REG_WRITE_32(CANCMR, reg);

         //SysIo::Printf("\r\nWrite to %u on %08X\r\n", i, this->mp_DevBaseAddr);
         //SysIo::Printf("\r\nID=%08X DLC=%1u DATA=%02X%02X%02X%02X%02X%02X%02X%02X\r\n", msg.id, msg.dlc, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
         // Wait for transmission to be done
         //while ( (LPC2xxx_CAN_CTRL_REG_READ_32(CANGSR) & 0x0048) == 0x0000 );

         //LPC2xxx_CAN_CTRL_REG_WRITE_32(CANMOD, 0);

         result = 0;

         break;
      }
      else
      {
         // This spot is not available
      }
   } // for(i = 2; i <= 18; i += 8)

   return result;
} // CLpc21xxCanDev::Write

int8_t CLpc21xxCanDev::SetBaudRate(
   const uint32_t baudRate
   )
{
  m_BaudRate = baudRate;
  Init();
  return -1;
} // CLpc21xxCanDev::SetBaudRate

int8_t CLpc21xxCanDev::SetFilter(
   const uint8_t  filterId,
   const uint32_t id
   )
{
   return -1;
} // CLpc21xxCanDev::SetFilter

int8_t CLpc21xxCanDev::Init(void)
{
   int8_t             result  = -1;
   uint32_t           reg     = 0;
   const uint32_t     TSEG1   = 12;
   const uint32_t     TSEG2   = 1;
   uint32_t           brp     = 0;
   CScifuVicDevice *  p_Vic   = NULL;

   CScifu::GetDevice(&p_Vic, "vic");

   CommonInit();

   if(NULL != p_Vic)
   {
     // Systematic initialization of all the registers.
     // Go into reset mode.
     LPC2xxx_CAN_CTRL_REG_WRITE_32(CANMOD, 1);
  
     // CANCMR
     // Nothing to do
  
     // CANGSR
     // Clear the RX, TX counters
     LPC2xxx_CAN_CTRL_REG_WRITE_32(CANGSR, 0x00000000);
  
     // CANICR
     // Nothing to do.
  
     // CANIER
     // Disable all interrupts
     LPC2xxx_CAN_CTRL_REG_WRITE_32(CANIER, 0x00000000);
  
     // CANBTR
     brp = (m_Pclk / (m_BaudRate * (TSEG1 + TSEG2 + 3))) - 1;
     reg = brp | (TSEG1 << 16) | (TSEG1 << 20);
     LPC2xxx_CAN_CTRL_REG_WRITE_32(CANBTR, reg);
     //LPC2xxx_CAN_CTRL_REG_WRITE_32(CANBTR, 0x00CC0011);
  
     // CANEWL
     // Leave to default?
     LPC2xxx_CAN_CTRL_REG_WRITE_32(CANEWL, 96);
  
     // CANSR
     // Nothing to do with the status register
  
     // CANRFS
     // Nothing to do with this.
  
     // CANRID
     // Nothing to do with this.
  
     // CANRDA
     // Nothing to do with this.
  
     // CANRDB
     // Nothing to do with this.
  
     // CANTFI1
     // Nothing to do with this.
  
     // CANTID1
     // Nothing to do with this.
  
     // CANTDA1
     // Nothing to do with this.
  
     // CANTDB1
     // Nothing to do with this.
  
     // CANTFI2
     // Nothing to do with this.
  
     // CANTID2
     // Nothing to do with this.
  
     // CANTDA2
     // Nothing to do with this.
  
     // CANTDB2
     // Nothing to do with this.
  
     // CANTDFI3
     // Nothing to do with this.
  
     // CANTID3
     // Nothing to do with this.
  
     // CANTDA3
     // Nothing to do with this.
  
     // CANTDB3
     // Nothing to do with this.
  
     // Got back out of reset
     LPC2xxx_CAN_CTRL_REG_WRITE_32(CANMOD, 0);
  
      // Attach the ISRs
      if(m_RxIntSource == 27)
      {
         p_Vic->Attach(CLpc21xxCanDev::Isr, reinterpret_cast<uint32_t>(this), m_RxIntVector, m_RxIntSource);
         p_Vic->Enable(m_RxIntSource);

         // Enable module's interrupts
         LPC2xxx_CAN_CTRL_REG_WRITE_32(CANIER, 0x01);
      }
      else
      {
         p_Vic->Attach(CLpc21xxCanDev::Isr, reinterpret_cast<uint32_t>(this), m_TxIntVector, m_TxIntSource);
         p_Vic->Enable(m_TxIntSource);

         // Enable interrupts
         LPC2xxx_CAN_CTRL_REG_WRITE_32(CANIER, 0x02);
      }

      result = 0;
   }
   else
   {}

   return result;
} // CLpc21xxCanDev::Init

void CLpc21xxCanDev::CommonInit(void)
{
   if(false == m_CommonInitFlag)
   {
      // Bypass mode by default
      LPC2xxx_CAN_ACC_FILTER_REG_WRITE_32(0, 0x00000003);

      m_CommonInitFlag = true;
   }
   else
   {}
} // CLpc21xxCanDev::CommonInit

int32_t CLpc21xxCanDev::NumMsgAvailableGet(void)
{
   #if 0
   int32_t            result  = -1;
   
   if(0x00000001 == (LPC2xxx_CAN_CTRL_REG_READ_32(CANGSR) & 0x00000001))
   {
      result = 1;
   }
   else
   {
      result = 0;
   }

   return result;
   #endif // 0

   return ( RingBufNumBytesGet(&m_RxFifo) / sizeof(sScifuCanMsg_t));
} // CLpc21xxCanDev::NumMsgAvailableGet
