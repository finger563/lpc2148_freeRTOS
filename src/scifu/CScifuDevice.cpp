#include "CScifuDevice.hpp"

CScifuDevice::CScifuDevice(void)
{
   mp_Next = NULL;
} // CScifuDevice::CScifuDevice

CScifuDevice::~CScifuDevice(void)
{} // CScifuDevice::~CScifuDevice

int8_t CScifuDevice::Init(void)
{
   return 0;
} // CScifuDevice::Init

int32_t CScifuDevice::Read(
   void * const   p_Data,
   size_t         nBytes
   )
{
   if(nBytes > 0)
   {
      *(reinterpret_cast<uint8_t *>(p_Data)) = 0;
   }
   else
   {}
   
   return -1;
} // CScifuDevice::Read

int32_t CScifuDevice::Write(
   void const * const   p_Data,
   size_t               nBytes
   )
{
   if(nBytes > 0 &&
      NULL != p_Data
      )
   {}
   else
   {}
   
   return -1;
} // CScifuDevice::Write

int8_t CScifuDevice::SetBaudRate(
   const uint32_t baudRate
   )
{
   if(baudRate >0)
   {}
   else
   {}
   
   return -1;
} // CScifuDevice::SetBaudRate

uint32_t CScifuDevice::GetBaudRate(void)
{
   return 0;
} // CScifuDevice::GetBaudRate
