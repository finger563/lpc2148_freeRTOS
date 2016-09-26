#ifndef cscifu_included
#define cscifu_included

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdarg.h>	/* For using variable arguments */
#include "CScifuDevice.hpp"

#define SCIFU_MAX_DEV_NAME_SIZE     (50)
// Maximum size of the buffered IO message
#define SCIFU_IO_BUFFER_SIZE   	   (uint16_t)(128)
#define SCIFU_MAX_STATIC_LOG_LEVEL  SCIFU_LOG_LEVEL_INFO

class CScifuDevice;

#define SCIFU_OFF 0
#define SCIFU_ERROR 1
#define SCIFU_WARNING 2
#define SCIFU_INFO 3
#define SCIFU_DEBUG 4
#define SCIFU_TRACE 5
      
class CScifu
{
   public:


   static char                      m_IoBuffer[SCIFU_IO_BUFFER_SIZE];
   static CScifuDevice *              mp_IoDev;
   static uint32_t                 m_LogLevel;
   static bool                        m_StopOnFirstError;
   
   #define CScifu_Error(...)                            \
         CScifu::Log(SCIFU_ERROR, __VA_ARGS__)
         
   #define CScifu_Warning(...)                          \
         CScifu::Log(SCIFU_WARNING, __VA_ARGS__)
         
   #define CScifu_Info(...)                             \
         CScifu::Log(SCIFU_INFO, __VA_ARGS__)
   #define CScifu_Debug(...)                             \
         CScifu::Log(SCIFU_DEBUG, __VA_ARGS__)
   #define CScifu_Trace(...)                             \
         CScifu::Log(SCIFU_TRACE, __VA_ARGS__)
		 
   #define CScifu_Stdio(...)							\
         CScifu::Stdio(__VA_ARGS__)
         
   template<class T>
   static void  ReleaseDevice(
      T **    pp_Device
      )
   {} // ReleaseDevice

   template<class T>
   static int32_t AddIoDevice(
      T &                  device
      )
   {
      mp_IoDev = &device;
	  //rprintf("\r\nCScifu::AddIoDevice");

      return 0;      
   }

   static void VerbositySet(
       const uint32_t  verbosity
       )
   {
      m_LogLevel = verbosity;
   }

   template<class T>
   static int8_t AddDevice(
      T &                  device,
      char const * const p_Name
      )
   {
      int8_t   			result		= -1;
	  CScifuDevice **	pp_Device	= &mp_FirstDevice;
	  
	  CScifu_Info("CScifu::AddDevice start is %x", *pp_Device);
	  
      if(NULL != p_Name)
      {
	     CScifu_Info("CScifu::AddDevice adding %s", p_Name);
         result = 0;
		 
		 // Go to the end of the chain
	     while(NULL != *pp_Device)
	     {
		    CScifu_Info("CScifu::AddDevice skipping %s %x", (*pp_Device)->mp_Name, *pp_Device);
			
		    if(NULL != (*pp_Device)->mp_Next)
			{
			   CScifu_Info("CScifu::AddDevice there's one more device");
	           *pp_Device = (*pp_Device)->mp_Next;
			}
			else
			{
			   CScifu_Info("CScifu::AddDevice end of chain");
			   break;
			}
	     }
		
 	     // Get this vacant spot
		 
         *pp_Device = &device;
		 CScifu_Info("CScifu::AddDevice adding device at the end %X", *pp_Device);
		 device.mp_Name = p_Name;
		 (*pp_Device)->mp_Next = NULL;
		 result = device.Init();

      }
      else
      {
	     CScifu_Warning("CScifu::AddDevice invalid name");
	  }

      return result;
   } // AddDevice

   template<class T>
   static int8_t  GetDevice(
      T **                  pp_Device,
      char const * const  p_Name
      )
      {
         int8_t   result   = -1;
		 CScifuDevice *	p_Device	= mp_FirstDevice;
		 
		 CScifu_Info("CScifu::GetDevice: %s", p_Name);
		 //rprintf("\r\nCScifu::GetDevice: %s", p_Name);

         while(NULL != p_Device)
	     {
            if(0 == strncmp(p_Device->mp_Name, p_Name, SCIFU_MAX_DEV_NAME_SIZE))
            {
               // This is the device we are looking for
               *pp_Device = reinterpret_cast<T *>(p_Device);
               result = 0;
			   CScifu_Info("CScifu::GetDevice: found");
			   //rprintf("\r\nCScifu::GetDevice: found");
               break;
            }
            else
            {
			   CScifu_Info("CScifu::GetDevice: %s not the device", p_Device->mp_Name);
			   //rprintf("\r\nCScifu::GetDevice: %s not the device", p_Device->mp_Name);
            }
			
			p_Device = p_Device->mp_Next;
         } // for(i = 0; i < SCIFU_MAX_NUM_DEV; i++)

         return result;
      }

      static void Log(
         uint32_t    verbosity,
         char const *   p_Format,
         ...
      )
      {
         va_list    argPtr;
         uint16_t   stringLen    = 0;
         int32_t    offset       = 0;
         va_start(argPtr, p_Format);
         
         if(verbosity <= m_LogLevel)
         {
            switch(verbosity)
            {
               case SCIFU_ERROR:
               {
                  strcpy(m_IoBuffer, "\r\nERROR,");
                  break;
               }
               case SCIFU_WARNING:
               {
                  strcpy(m_IoBuffer, "\r\nWARNING,");
                  break;
               }
               case SCIFU_INFO:
               {
                  strcpy(m_IoBuffer, "\r\nINFO,");
                  break;
               }
               case SCIFU_DEBUG:
               {
                  strcpy(m_IoBuffer, "\r\nDEBUG,");
                  break;
               }
               case SCIFU_TRACE:
               {
                  strcpy(m_IoBuffer, "\r\nTRACE,");
                  break;
               }
               default:
               {
                  strcpy(m_IoBuffer, "\r\nUNKNOWN,");
               }
            } // switch(verbosity)
            
            offset = strlen(m_IoBuffer);

            // Proceed to the formating
            stringLen = (uint16_t)(vsprintf(reinterpret_cast<char *>(m_IoBuffer + offset), reinterpret_cast<const char *>(p_Format), argPtr)) + offset;
   
            va_end(argPtr);

            // Write data to the serial port

            if(NULL != mp_IoDev)
			{
                mp_IoDev->Write(m_IoBuffer, stringLen);
		    }
			else
			{}

            if( verbosity == SCIFU_ERROR &&
                true == m_StopOnFirstError
              )
            {
               while(1)
               {}
            }
            else
            {}

         }
         else
         {}
      }
	  
	  static int GetC(void)
	  {
	     int 		result = -1;
		 uint8_t	myChar = 0;
		 
		 if(NULL != mp_IoDev)
		 {
            if(1 == mp_IoDev->Read(&myChar, 1))
			{
			   result = myChar;
			}
			else
			{}   
		 }
	     else
		 {}
		 
		 return result;
	  }
	  
	   static int32_t PutC(
	     int8_t myChar
	     )
	   {
         int32_t result = -1;
         
	     if(NULL != mp_IoDev)
		  {
            if(1 == mp_IoDev->Write(&myChar, 1))
			{
			   result = 0;
			}
			else
			{}   
		 }
	     else
		 {}
		 
		 return result;
	  }
	  
	  static void Stdio(
         char const *   p_Format,
         ...
      )
      {
         
            va_list    argPtr;
            uint16_t   stringLen    = 0;

            va_start(argPtr, p_Format);
            
            // Proceed to the formating
            stringLen = (uint16_t)(vsprintf(reinterpret_cast<char *>(m_IoBuffer), reinterpret_cast<const char *>(p_Format), argPtr));
   
            va_end(argPtr);

            // Write data to the serial port

            if(NULL != mp_IoDev)
			{
                mp_IoDev->Write(m_IoBuffer, stringLen);
		    }
			else
			{}
      }
	  
   static CScifuDevice *	mp_FirstDevice;
}; // CScifu

#endif // cscifu_included
