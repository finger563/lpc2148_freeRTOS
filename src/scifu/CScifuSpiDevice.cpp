#include "CScifuSpiDevice.hpp"

CScifuSpiDevice::CScifuSpiDevice(void)
{} // CScifuSpiDevice::CScifuSpiDevice

CScifuSpiDevice::~CScifuSpiDevice(void)
{} // CScifuSpiDevice::~CScifuSpiDevice

int8_t CScifuSpiDevice::AttachCs(
   void (* p_Select)(const uint8_t index),
   void (* p_UnSelect)(const uint8_t index),
   const uint8_t nDevs
)
{
   int8_t result = 0;

   if(NULL != p_Select &&
      NULL != p_UnSelect)
   {
      mp_Select = p_Select;
      mp_UnSelect = p_UnSelect;
      m_NumDevices = nDevs;
   }
   else
   {}

   return result;
}

void CScifuSpiDevice::Select(
   const uint8_t id
   )
 {
    uint8_t dev = 0;

    if(NULL != mp_UnSelect &&
       NULL != mp_Select
       )
    {
      for(dev = 0; dev < m_NumDevices; dev++)
      {
         if(id != dev)
         {
            mp_UnSelect(dev);
         }
         else
         {}
      }
      mp_Select(id);
   }
   else
   {}
 }

void CScifuSpiDevice::UnSelect(
   const uint8_t id
   )
 {
    if(NULL != mp_UnSelect &&
       NULL != mp_Select
       )
    {
       mp_UnSelect(id);
     }
     else
     {}
 }
