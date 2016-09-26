#ifndef clpc21xxcandev_included
#define clpc21xxcandev_included

#include "CScifuCanDevice.hpp"
#include "ringbuf.h"

#define LPC2xxxCANDEV_RX_SIZE (sizeof(CScifuCanDevice::sScifuCanMsg_t) * 10)
#define LPC2xxxCANDEV_TX_SIZE (sizeof(CScifuCanDevice::sScifuCanMsg_t) * 10)

class CLpc21xxCanDev :
   public CScifuCanDevice
{
   public:
   protected:
   private:
      typedef enum eCanCtrlRegs
      {
         CANMOD   = 0,
         CANCMR   = 1,
         CANGSR   = 2,
         CANICR   = 3,
         CANIER   = 4,
         CANBTR   = 5,
         CANEWL   = 6,
         CANSR    = 7,
         CANRFS   = 8,
         CANRID   = 9,
         CANRDA   = 10,
         CANRDB   = 11,
         CANTFI1  = 12,
         CANTID1  = 13,
         CANTDA1  = 14,
         CANTDB1  = 15
      } eCanCtrlRegs_t;

      typedef enum eAccFilterRegs_t
      {
         AFMR = 0
      };

   public:
      
      CLpc21xxCanDev(
         const uint32_t accFilterRam,
         const uint32_t accFilterRegs,
         const uint32_t centralCanRegs,
         const uint32_t baseAddr,
         const uint32_t pclk,
         const uint32_t rxIntVector,
         const uint32_t rxIntSource,
         const uint32_t txIntVector,
         const uint32_t txIntSource
      );

      ~CLpc21xxCanDev(void);

      virtual int8_t SetBaudRate(
         const uint32_t baudRate
         );

      virtual int8_t Read(
         sScifuCanMsg_t & msg
         );
      
      virtual int8_t Read(
         sScifuCanMsg_t * p_Msg,
         const uint8_t    numMsg
         );

      virtual int8_t Read(
         sScifuCanMsg_t * p_Msg,
         const uint8_t    numMsg,
         const uint32_t   timeout
         );

      int8_t SetFilter(
         const uint8_t  filterId,
         const uint32_t id
      );

      virtual int32_t NumMsgAvailableGet(void);

      virtual int8_t Write(
         sScifuCanMsg_t & msg
         );

      virtual int8_t Write(
         sScifuCanMsg_t * p_Msg,
         const uint8_t    numMsg
         );

   protected:
   private:
      virtual int8_t Init(void);

      void CommonInit(void);

      static void Isr(
         uint32_t arg
      );

      CLpc21xxCanDev(void);

   public:
   protected:
   private:
      // Common registers
      static uint32_t *         mp_AccFilterRegs;
      // Common variabled
      static uint32_t           m_Pclk;
      // Common init flagged
      static bool               m_CommonInitFlag;
      uint32_t *                mp_DevBaseAddr;
      uint32_t                  m_RxIntVector;
      uint32_t                  m_RxIntSource;
      uint32_t                  m_TxIntVector;
      uint32_t                  m_TxIntSource;
      sRingBuf_t                m_RxFifo;
      sRingBuf_t                m_TxFifo;
      uint8_t                   m_RxBuffer[LPC2xxxCANDEV_RX_SIZE];
      uint8_t                   m_TxBuffer[LPC2xxxCANDEV_TX_SIZE];
      bool                      m_TxTransmitting;
      
 
}; // CLpc21xxCanDev

#endif // clpc21xxcandev_included
