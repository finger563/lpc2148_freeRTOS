#ifndef clpc2xxxvicdev_included
#define clpc2xxxvicdev_included

#include <inttypes.h>
#include "cscifuvicdevice.hpp"

class CLpc2xxxVicDev :
   public CScifuVicDevice
{
   public:
      typedef enum eSources
      {
         WDT,
         SW,
         ARM_CORE_0,
         ARM_CORE_1,
         TIMER_0,
         TIMER_1,
         UART_0,
         UART_1,
         PWM_0,
         I2C,
         SPI_0,
         SPI_1,
         PLL,
         RTC,
         EINT_0,
         EINT_1,
         EINT_2,
         EINT_3,
         AD,
         CAN,
         CAN1_TX,
         CAN2_TX,
         CAN3_TX,
         CAN4_TX,
         RSVD0,
         RSVD1,
         CAN1_RX,
         CAN2_RX,
         CAN3_RX,
         CAN4_RX,
         RSVD2,
         RSVD3,
      } eSources_t;
   protected:
   private:

   public:
      CLpc2xxxVicDev(
         const uint32_t baseAddress
         );

      ~CLpc2xxxVicDev(void);

      virtual int8_t Attach(
         void                   (* p_Isr)(uint32_t),
         const uint32_t         arg,
         const uint32_t         vector,
         const uint32_t         source
         );

      virtual int8_t Enable(
         const uint32_t         source
         );

   protected:
   private:
      CLpc2xxxVicDev(void);

      int8_t Init(void);

      static void Isr0(void);
      static void Isr1(void);
      static void Isr2(void);
      static void Isr3(void);
      static void Isr4(void);
      static void Isr5(void);
      static void Isr6(void);
      static void Isr7(void);
      static void Isr8(void);
      static void Isr9(void);
      static void Isr10(void);
      static void Isr11(void);
      static void Isr12(void);
      static void Isr13(void);
      static void Isr14(void);
      static void Isr15(void);


   public:
   protected:
   private:
      
      uint32_t volatile *             mp_BaseAddr;
      static CScifuVicDevice::sIsr_t  m_IsrList[16];
}; // CLpc2xxxVicDev

#endif // clpc2xxxvicdev_included
