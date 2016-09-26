#ifndef clpc21xxuartdev_included
#define clpc21xxuartdev_included

#include <inttypes.h>
#include "CScifuUartDevice.hpp"

class CLpc21xxUartDev :
   public CScifuUartDevice
{
   public:
      CLpc21xxUartDev(
         uint8_t * const p_BaseAddress,
         const uint32_t   pclock
         );

      ~CLpc21xxUartDev();

      virtual int8_t Init(void);

      int32_t Read(
         void * const   p_Data,
         size_t         nBytes
         );

      int8_t SetBaudRate(
         const uint32_t baudRate
         );

      uint32_t GetBaudRate(void);

      int32_t Write(
         void const * const   p_Data,
         size_t               nBytes
         );

   private:
      size_t SendData(
         void const * const             p_Data, /* in */
         const size_t                   nBytes  /* in */
      );
	  
	  size_t ReceiveData(
        void * const	p_Data, /* in */
        const size_t		nBytes  /* in */
      );

      CLpc21xxUartDev();
      uint8_t volatile *  mp_BaseAddr;
      bool                m_PollMode;
      uint32_t            m_BaudRate;
      static uint32_t     m_PClock;

}; // CLpc21xxUartDev

#endif // CLpc21xxUartDev_included
