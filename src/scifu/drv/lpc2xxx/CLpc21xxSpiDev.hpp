#ifndef clpc21xxspidev_included
#define clpc21xxspidev_included

#include <inttypes.h>
#include "cscifuspidevice.hpp"

class CLpc21xxSpiDev :
   public CScifuSpiDevice
{
   public:
   protected:
   private:

   public:
      CLpc21xxSpiDev(
         const uint32_t baseAddress,
         const uint32_t pclock
         );

      ~CLpc21xxSpiDev(void);

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

      virtual int8_t Init(void);

   private:
      CLpc21xxSpiDev(void);
      uint16_t ByteWrite(
  uint8_t data
);
      uint32_t volatile *    mp_BaseAddr;
      uint32_t              m_PClock;
      static const uint8_t  m_SPCR;
      static const uint8_t  m_SPSR;
      static const uint8_t  m_SPDR;
      static const uint8_t  m_SPCCR;
      static const uint8_t  m_SPINT;
      uint32_t              m_BaudRate;

}; // CLpc21xxSpiDev

#endif // clpc21xxspidev_included
