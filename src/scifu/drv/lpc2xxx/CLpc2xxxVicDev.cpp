#include <ctl_api.h> // This is a dependency on Rowley that needs to be removed
#include "CLpc2xxxVicDev.hpp"
#include "CScifu.hpp"

#define LPC2xxx_REG_READ_32(_reg)                  (mp_BaseAddr[_reg])
#define LPC2xxx_REG_WRITE_32(_reg, _value)         ((mp_BaseAddr[_reg]) = _value)

CScifuVicDevice::sIsr_t                  CLpc2xxxVicDev::m_IsrList[16];

CLpc2xxxVicDev::CLpc2xxxVicDev(
   const uint32_t baseAddress
   )
{
   mp_BaseAddr = reinterpret_cast<uint32_t *>(baseAddress);

   Init();
} // CLpc2xxxVicDev::CLpc2xxxVicDev

CLpc2xxxVicDev::CLpc2xxxVicDev(void)
{
   mp_BaseAddr = NULL;
} // CLpc2xxxVicDev::CLpc2xxxVicDev

CLpc2xxxVicDev::~CLpc2xxxVicDev(void)
{}

int8_t CLpc2xxxVicDev::Init(void)
{
   int8_t   result  = -1;
   uint32_t vector  = 0;

   mp_IsrList = m_IsrList;
   m_NumIsr   = 16;

   return result;
} // CLpc2xxxVicDev::Init

void CLpc2xxxVicDev::Isr0(void)
{
   //SysIo::Printf("\r\nisr0");
   (m_IsrList[0].p_Isr)(m_IsrList[0].arg);
} // CLpc2xxxVicDev::Isr0

void CLpc2xxxVicDev::Isr1(void)
{
   //SysIo::Printf("\r\nisr1");
   (m_IsrList[1].p_Isr)(m_IsrList[1].arg);
} // CLpc2xxxVicDev::Isr1

void CLpc2xxxVicDev::Isr2(void)
{
   //SysIo::Printf("\r\nrx2_isr");
   (m_IsrList[2].p_Isr)(m_IsrList[2].arg);
} // CLpc2xxxVicDev::Isr2

void CLpc2xxxVicDev::Isr3(void)
{
   //SysIo::Printf("\r\ntx1_isr");
   (m_IsrList[3].p_Isr)(m_IsrList[3].arg);
} // CLpc2xxxVicDev::Isr3

void CLpc2xxxVicDev::Isr4(void)
{
   CScifu_Warning("\r\nisr4");
   (m_IsrList[4].p_Isr)(m_IsrList[4].arg);
} // CLpc2xxxVicDev::Isr4

void CLpc2xxxVicDev::Isr5(void)
{
   CScifu_Warning("\r\nisr5");
   (m_IsrList[5].p_Isr)(m_IsrList[5].arg);
} // CLpc2xxxVicDev::Isr5

void CLpc2xxxVicDev::Isr6(void)
{
   CScifu_Warning("\r\nisr6");
   (m_IsrList[6].p_Isr)(m_IsrList[6].arg);
} // CLpc2xxxVicDev::Isr6

void CLpc2xxxVicDev::Isr7(void)
{
   CScifu_Warning("\r\nisr7");
   (m_IsrList[7].p_Isr)(m_IsrList[7].arg);
} // CLpc2xxxVicDev::Isr7

void CLpc2xxxVicDev::Isr8(void)
{
   CScifu_Warning("\r\nisr8");
   (m_IsrList[8].p_Isr)(m_IsrList[8].arg);
} // CLpc2xxxVicDev::Isr8

void CLpc2xxxVicDev::Isr9(void)
{
   CScifu_Warning("\r\nisr9");
   (m_IsrList[9].p_Isr)(m_IsrList[9].arg);
} // CLpc2xxxVicDev::Isr9

void CLpc2xxxVicDev::Isr10(void)
{
   CScifu_Warning("\r\nisr10");
   (m_IsrList[10].p_Isr)(m_IsrList[10].arg);
} // CLpc2xxxVicDev::Isr10

void CLpc2xxxVicDev::Isr11(void)
{
   CScifu_Warning("\r\nisr11");
   (m_IsrList[11].p_Isr)(m_IsrList[11].arg);
} // CLpc2xxxVicDev::Isr11

void CLpc2xxxVicDev::Isr12(void)
{
   CScifu_Warning("\r\nisr12");
   (m_IsrList[12].p_Isr)(m_IsrList[12].arg);
} // CLpc2xxxVicDev::Isr12

void CLpc2xxxVicDev::Isr13(void)
{
   CScifu_Warning("\r\nisr13");
   (m_IsrList[13].p_Isr)(m_IsrList[13].arg);
} // CLpc2xxxVicDev::Isr13

void CLpc2xxxVicDev::Isr14(void)
{
   CScifu_Warning("\r\nisr14");
   (m_IsrList[14].p_Isr)(m_IsrList[14].arg);
} // CLpc2xxxVicDev::Isr14

void CLpc2xxxVicDev::Isr15(void)
{
   CScifu_Warning("\r\nisr15");
   (m_IsrList[15].p_Isr)(m_IsrList[15].arg);
} // CLpc2xxxVicDev::Isr15

int8_t CLpc2xxxVicDev::Attach(
   void                   (* p_Isr)(uint32_t),
   const uint32_t         arg,
   const uint32_t         vector,
   const uint32_t         source
   )
{
   int8_t result = 0;
   uint32_t reg = 0;

   CScifuVicDevice::Attach(p_Isr, arg, vector, source);

   reg = LPC2xxx_REG_READ_32(3);
   reg &= ~(1 << source);
   LPC2xxx_REG_WRITE_32(3, reg);
   
   #if 0
   switch(vector)
   {
      case 0:
      {
         ctl_set_isr(source, vector, CTL_ISR_TRIGGER_FIXED, CLpc2xxxVicDev::Isr0, NULL);
         break;
      }
      case 1:
      {
         ctl_set_isr(source, vector, CTL_ISR_TRIGGER_FIXED, &(CLpc2xxxVicDev::Isr1), NULL);
         break;
      }
      case 2:
      {
         ctl_set_isr(source, vector, CTL_ISR_TRIGGER_FIXED, CLpc2xxxVicDev::Isr2, NULL);
         break;
      }
      case 3:
      {
         ctl_set_isr(source, vector, CTL_ISR_TRIGGER_FIXED, CLpc2xxxVicDev::Isr3, NULL);
         break;
      }
      case 4:
      {
         ctl_set_isr(source, vector, CTL_ISR_TRIGGER_FIXED, CLpc2xxxVicDev::Isr4, NULL);
         break;
      }
      case 5:
      {
         ctl_set_isr(source, vector, CTL_ISR_TRIGGER_FIXED, CLpc2xxxVicDev::Isr5, NULL);
         break;
      }
      case 6:
      {
         ctl_set_isr(source, vector, CTL_ISR_TRIGGER_FIXED, CLpc2xxxVicDev::Isr6, NULL);
         break;
      }
      case 7:
      {
         ctl_set_isr(source, vector, CTL_ISR_TRIGGER_FIXED, CLpc2xxxVicDev::Isr7, NULL);
         break;
      }
      case 8:
      {
         ctl_set_isr(source, vector, CTL_ISR_TRIGGER_FIXED, CLpc2xxxVicDev::Isr8, NULL);
         break;
      }
      case 9:
      {
         ctl_set_isr(source, vector, CTL_ISR_TRIGGER_FIXED, CLpc2xxxVicDev::Isr9, NULL);
         break;
      }
      case 10:
      {
         ctl_set_isr(source, vector, CTL_ISR_TRIGGER_FIXED, CLpc2xxxVicDev::Isr10, NULL);
         break;
      }
      case 11:
      {
         ctl_set_isr(source, vector, CTL_ISR_TRIGGER_FIXED, CLpc2xxxVicDev::Isr11, NULL);
         break;
      }
      case 12:
      {
         ctl_set_isr(source, vector, CTL_ISR_TRIGGER_FIXED, CLpc2xxxVicDev::Isr12, NULL);
         break;
      }
      case 13:
      {
         ctl_set_isr(source, vector, CTL_ISR_TRIGGER_FIXED, CLpc2xxxVicDev::Isr13, NULL);
         break;
      }
      case 14:
      {
         ctl_set_isr(source, vector, CTL_ISR_TRIGGER_FIXED, CLpc2xxxVicDev::Isr14, NULL);
         break;
      }
      case 15:
      {
         ctl_set_isr(source, vector, CTL_ISR_TRIGGER_FIXED, CLpc2xxxVicDev::Isr15, NULL);
         break;
      }
      default:
      {
         result = -1;
      }
   } // switch(vector)
   #endif
   return result;
} // CLpc2xxxVicDev::Attach


int8_t CLpc2xxxVicDev::Enable(
   const uint32_t source
   )
{
   #if 0
   ctl_unmask_isr(source);
   #endif
   return 0;
} // CLpc2xxxVicDev::Enable
