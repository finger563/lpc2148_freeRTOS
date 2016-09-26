#include "cLpc21xxVicDev.hpp"

CScifuIntDevice::sIsr_t   CLpc21xxVicDev::m_IsrList[10];

CLpc21xxVicDev::CLpc21xxVicDev(
   const uint32_t baseAddr
   )
{
   mp_BaseAddr  = reinterpret_cast<uint32_t *>(baseAddr);
   Init();
}

CLpc21xxVicDev::~CLpc21xxVicDev(void)
{}

CLpc21xxVicDev::CLpc21xxVicDev(void)
{}

int8_t CLpc21xxVicDev::Init(void)
{
   CScifuIntDevice::mp_IsrList      = m_IsrList;
   CScifuIntDevice::m_IsrListSize   = 10;

   // TODO
   // Attach the main ISR to all vectors.

} // CLpc21xxVicDev::Init

void CLpc21xxVicDev::MainIsr(void)
{
   // TODO
   // Detect which vector generated an interrupt.
   // Invoke the proper ISR.
} // CLpc21xxVicDev::MainIsr
