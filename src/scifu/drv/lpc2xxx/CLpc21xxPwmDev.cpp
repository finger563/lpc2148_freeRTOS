#include "cLpc21xxPwmDev.h"

const  uint8_t   CLpc21xxPwmDev::m_IR  = 0;
const  uint8_t   CLpc21xxPwmDev::m_TCR = 1;
const  uint8_t   CLpc21xxPwmDev::m_TC  = 2;
const  uint8_t   CLpc21xxPwmDev::m_PR  = 3;
const  uint8_t   CLpc21xxPwmDev::m_PC  = 4;
const  uint8_t   CLpc21xxPwmDev::m_MCR = 5;
const  uint8_t   CLpc21xxPwmDev::m_MR0 = 6;
const  uint8_t   CLpc21xxPwmDev::m_MR1 = 7;
const  uint8_t   CLpc21xxPwmDev::m_MR2 = 8;
const  uint8_t   CLpc21xxPwmDev::m_MR3 = 9;
const  uint8_t   CLpc21xxPwmDev::m_MR4 = 16;
const  uint8_t   CLpc21xxPwmDev::m_MR5 = 17;
const  uint8_t   CLpc21xxPwmDev::m_MR6 = 18;
const  uint8_t   CLpc21xxPwmDev::m_PCR = 19;
const  uint8_t   CLpc21xxPwmDev::m_LER = 20;

CLpc21xxPwmDev::CLpc21xxPwmDev(
   const uint32_t baseAddr
   )
{
   mp_BaseAddr  = reinterpret_cast<uint32_t *>(baseAddr);
   m_Prescaler  = 65535;
   Init();
}

CLpc21xxPwmDev::~CLpc21xxPwmDev(void)
{}

CLpc21xxPwmDev::CLpc21xxPwmDev(void)
{}

void CLpc21xxPwmDev::Init(void)
{
   // IR
   // Clear all interrupts
   mp_BaseAddr[m_IR] = 0;

   // TCR
   mp_BaseAddr[m_TCR] = 0x02;

   // TC
   mp_BaseAddr[m_TC] = 0;

   // PR
   mp_BaseAddr[m_PR] = 0;

   // PC
   mp_BaseAddr[m_PC] = 0;

   // MCR
   mp_BaseAddr[m_MCR] = 0;

   // MR0
   mp_BaseAddr[m_MR0] = 0;

   // MR1
   mp_BaseAddr[m_MR1] = 0;

   // MR2
   //mp_BaseAddr[m_MR2] = 3500000;
   mp_BaseAddr[m_MR2] = 1000;

   // MR3
   mp_BaseAddr[m_MR3] = 0;

   // MR4
   mp_BaseAddr[m_MR4] = 0;

   // MR5
   mp_BaseAddr[m_MR5] = 0;

   // MR6
   mp_BaseAddr[m_MR6] = 0;

   // PCR
   mp_BaseAddr[m_PCR] = (1 << 2) | (1 << 10);

   // LER
   mp_BaseAddr[m_LER] = 0;

   mp_BaseAddr[m_TCR] = 13;
} // CLpc21xxPwmDev::Init
