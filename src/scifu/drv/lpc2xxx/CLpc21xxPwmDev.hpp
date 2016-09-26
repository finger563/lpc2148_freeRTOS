#ifndef clpc21xxpwmdev_included
#define clpc21xxpwmdev_included

#include "cscifupwmdevice.h"

class CLpc21xxPwmDev :
   public CScifuPwmDevice
{
   public:
   protected:
   private:

   public:
      
      CLpc21xxPwmDev(
         const uint32_t baseAddr
      );

      ~CLpc21xxPwmDev(void);

   protected:
   private:

      CLpc21xxPwmDev(void);

      void Init(void);

   public:
   protected:
   private:
      uint32_t *              mp_BaseAddr;
      static const  uint8_t   m_IR;
      static const  uint8_t   m_TCR;
      static const  uint8_t   m_TC;
      static const  uint8_t   m_PR;
      static const  uint8_t   m_PC;
      static const  uint8_t   m_MCR;
      static const  uint8_t   m_MR0;
      static const  uint8_t   m_MR1;
      static const  uint8_t   m_MR2;
      static const  uint8_t   m_MR3;
      static const  uint8_t   m_MR4;
      static const  uint8_t   m_MR5;
      static const  uint8_t   m_MR6;
      static const  uint8_t   m_PCR;
      static const  uint8_t   m_LER;

      uint32_t  m_Prescaler;
}; // CLpc21xxPwmDev

#endif // clpc21xxpwmdev_included
