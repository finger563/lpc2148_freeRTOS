#ifndef cscifuspidevice_included
#define cscifuspidevice_included

#include "cscifudevice.hpp"

class CScifuSpiDevice :
   public CScifuDevice
{
   public:

      CScifuSpiDevice(void);

      ~CScifuSpiDevice(void);

      int8_t AttachCs(
        void (* p_Select)(const uint8_t index),
        void (* p_UnSelect)(const uint8_t index),
        const uint8_t nDevs
        );

      void Select(
         const uint8_t id
         );

      void UnSelect(
         const uint8_t id
         );

      void (* mp_Select)(const uint8_t index);
      void (* mp_UnSelect)(const uint8_t index);
      uint8_t m_NumDevices;
}; // CScifuSpiDevice

#endif // cscifuspidevice_included
