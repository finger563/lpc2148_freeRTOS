#ifndef cscifudevice_included
#define cscifudevice_included

#include <stddef.h>
#include "inttypes.h"

class CScifuDevice
{
   public:
   
      CScifuDevice(void);
   
      virtual ~CScifuDevice(void);

      virtual int32_t Read(
         void * const   p_Data,
         size_t         nBytes
         );

      virtual int8_t SetBaudRate(
         const uint32_t baudRate
         );

      virtual uint32_t GetBaudRate(void);

      virtual int8_t Init(void);

      virtual int32_t Write(
         void const * const   p_Data,
         size_t               nBytes
         );

      CScifuDevice *          mp_Next;
	  char const * 				  mp_Name;

}; // CScifuDevice

#endif // cscifudevice_included
