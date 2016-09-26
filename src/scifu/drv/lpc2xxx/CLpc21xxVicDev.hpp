#ifndef clpc21xxvicdev_included
#define clpc21xxvicdev_included

#include "cscifuintdevice.hpp"

class CLpc21xxVicDev :
   public CScifuIntDevice
{
   public:
   protected:
   private:

   public:
      
      CLpc21xxVicDev(
         const uint32_t baseAddr
      );

      ~CLpc21xxVicDev(void);

   protected:
   private:

      CLpc21xxVicDev(void);

      virtual int8_t Init(void);

      static void MainIsr(void);

   public:
   protected:
   private:
      uint32_t *                        mp_BaseAddr;
      static  CScifuIntDevice::sIsr_t   m_IsrList[10];

}; // CLpc21xxVicDev

#endif // clpc21xxvicdev_included
