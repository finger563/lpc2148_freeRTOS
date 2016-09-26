////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE NAME:     $Source:$
//                $Date:$
//                $Revision:$
//
// AUTHOR:        Jean-Sebastien Stoezel (js.stoezel@gmail.com)
//
// HISTORY:       $Log:$
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "CAnalogSampler.hpp"
#include "CLog.hpp"
#include "Logomatic_V2.hpp"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// METHODS



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAnalogSampler::CAnalogSampler(void):
   m_BufferIndex(0),
   m_BufferOverFlow(false),
   m_ClkDivider(1000),
   m_LoggingMode(ASCII),
   m_NumChannels(0),
   m_SampleIndex(0)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   // Reset all channels
   for(uint32_t j = 0; j < m_NumChannels; j++)
   {
      ChannelInit(j, false, "");
   } // for
   
   // Free all the buffers
   for(uint32_t i = 0; i < ANALOG_SAMPLER_BUFFER_NUM; i++)
   {
      m_BufferList[i].inUse = false;
   }
} // CAnalogSampler::CAnalogSampler

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAnalogSampler::~CAnalogSampler(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{} // CAnalogSampler::~CAnalogSampler

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAnalogSampler::SamplesPush(
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   const uint32_t tick  // in
   )
{
   signed portBASE_TYPE             xHigherPriorityTaskWoken;
   CSubsystem::sActivation_t         activationPacket;
   uint32_t                           dummy;
   
   #if 0
   100905 jstoezel: just too much logging
   LOG_TRACE("CAnalogSampler::SamplesPush");
   #endif
   
   //LOG_INFO("CAnalogSampler::SamplesPush %u %u %u", tick, m_ClkDivider, (tick % m_ClkDivider));

   if(0 == (tick % m_ClkDivider))
   {
      // Check if the buffer is available for writting. The assumption is that if the next buffer in line is not available for writting,
      // then no other buffer is available, since we treat them as FIFOs.
      if(false == m_BufferList[m_BufferIndex].inUse)
      {
         for(uint32_t i = 0; i < m_NumChannels; i++)
         {
            if(true == mp_ChannelList[i].enabled)
            {
               m_BufferList[m_BufferIndex].buffer[m_SampleIndex] = SampleGet(i);
               
               m_SampleIndex++;
               
               if(m_SampleIndex >= ANALOG_SAMPLER_BUFFER_NUM_SAMPLES)
               {
                  // Buffer is full, pass it on for flushing to file
                  #if 0
                  LOG_INFO("CAnalogSampler::SamplesPush buffer full (%u samples)", m_SampleIndex);
                  #endif
                  
                  activationPacket.p_Cmd           = CSubsystem::CommandGet();
                  
                  if(0 != activationPacket.p_Cmd)
                  {
                     activationPacket.p_Cmd->id       = CAnalogSampler::CMD_NEW_ANALOG_BUFFER;
                     activationPacket.p_Cmd->dataLen  = sizeof(&m_BufferList[m_BufferIndex]);
                     activationPacket.type            = CSubsystem::ACTIVATION_TYPE_CMD;
                     
                     
                     sAnalogBuffer_t * p_Buffer = &m_BufferList[m_BufferIndex];
                     memcpy(activationPacket.p_Cmd->data, &p_Buffer, 4);
                     memcpy(&dummy, activationPacket.p_Cmd->data, 4);
                     
                     xQueueSendFromISR(m_ActivationQueue, &activationPacket, &xHigherPriorityTaskWoken);
                     m_BufferIndex++;
                     m_BufferIndex %= ANALOG_SAMPLER_BUFFER_NUM;
                     m_SampleIndex = 0;
                  }
                  else
                  {
                     // Unable to get command from queue.
                     m_BufferOverFlow = true;
                  }
               }
               else
               {
                  // Buffer not full yet, normal case
               }
            }
            else
            {
               // Channel not enabled, normal case
            }
         } // for
         
         if(ASCII == m_LoggingMode)
         {
            // We add separators for end of lines... These are not logged
            m_BufferList[m_BufferIndex].buffer[m_SampleIndex] = 0xFFFF;
               
            m_SampleIndex++;
            
            if(m_SampleIndex >= ANALOG_SAMPLER_BUFFER_NUM_SAMPLES)
            {
               // Buffer is full, pass it on for flushing to file
               #if 0
               LOG_INFO("CAnalogSampler::SamplesPush buffer full (%u samples)", m_SampleIndex);
               #endif
               
               activationPacket.p_Cmd           = CSubsystem::CommandGet();
               
               if(0 != activationPacket.p_Cmd)
               {
                  activationPacket.p_Cmd->id       = CAnalogSampler::CMD_NEW_ANALOG_BUFFER;
                  activationPacket.p_Cmd->dataLen  = sizeof(&m_BufferList[m_BufferIndex]);
                  activationPacket.type            = CSubsystem::ACTIVATION_TYPE_CMD;
                  
                  
                  sAnalogBuffer_t * p_Buffer = &m_BufferList[m_BufferIndex];
                  memcpy(activationPacket.p_Cmd->data, &p_Buffer, 4);
                  memcpy(&dummy, activationPacket.p_Cmd->data, 4);
                  
                  xQueueSendFromISR(m_ActivationQueue, &activationPacket, &xHigherPriorityTaskWoken);
                  m_BufferIndex++;
                  m_BufferIndex %= ANALOG_SAMPLER_BUFFER_NUM;
                  m_SampleIndex = 0;
               }
               else
               {
                  // Unable to get command from queue.
                  m_BufferOverFlow = true;
               }
            }
            else
            {
               // Buffer not full yet, normal case
            }
         }
         else
         {}
      }
      else
      {
         // All buffers are full. Ideally woudl like to log a message, but this is the interrupt context.
         // Set the overflow flag up instead.
         m_BufferOverFlow = true;
      }
   }
   else
   {}
} // CAnalogSampler::SamplesPush

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int32_t CAnalogSampler::ChannelInit(
	const uint32_t id,   // in
	const bool enabled, // in
   const char name[]    // in
	)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   int32_t result = -1;
   
   LOG_TRACE("CAnalogSampler::ChannelInit");
   
   if(id < m_NumChannels)
   {
      LOG_DEBUG("CAnalogSampler::ChannelInit id=%u enabled=%u", id, enabled);
      mp_ChannelList[id].enabled = enabled;
      mp_ChannelList[id].p_Name = name;
      result = 0;
   }
   else
   {
      LOG_WARNING("CAnalogSampler::ChannelRateSet %u invalid channel", id);
   }
   
   return result;
} // CAnalogSampler::ChannelRateSet
   
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int32_t CAnalogSampler::Init(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   LOG_TRACE("CAnalogSampler::Init");
   LOG_DEBUG("CAnalogSampler::Init rate=%u stack_size=%u", m_Rate, m_StackSize);
   
   m_BufferOverFlow = false;
   
   LogFileCreate();
   
   return CSubsystem::Init();
} // CAnalogSampler::Init

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAnalogSampler::LogFileCreate(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   char     fileName[32];
   uint32_t cnt   = 0;
   FRESULT  result = FR_OK;
   UINT		bytesWritten = 0;
   char     extension[5];
   
   LOG_TRACE("CAnalogSampler::LogFileCreate");

   switch(m_LoggingMode)
   {
      case BINARY:
      {
         sprintf(extension, ".bin");
         break;
      }
      case ASCII:
      {
         sprintf(extension, ".csv");
         break;
      }
      default:
      {
         sprintf(extension, "");
      }
   } // switch
   
   sprintf(fileName, "%04u%s", cnt, extension);
   
   while(FR_OK == f_open(&m_SamplerFile, fileName, FA_READ | FA_OPEN_EXISTING	))
   {
      f_close(&m_SamplerFile);
      
      // Switch to the next file
      cnt++;
      sprintf(fileName, "%04u%s", cnt, extension);
   }
   
   // Create a new file
   if(FR_OK == f_open(&m_SamplerFile, fileName, FA_WRITE | FA_CREATE_NEW))
   {
      LOG_INFO("CAnalogSampler::LogFileCreate created %s file", fileName);
      
      if(ASCII == m_LoggingMode)
      {
         //f_puts("time,");
               
         for(uint32_t i = 0; i < m_NumChannels; i++)
         {
            if(true == mp_ChannelList[i].enabled)
            {
               f_printf(&m_SamplerFile, "%s,", mp_ChannelList[i].p_Name);
               LOG_DEBUG("CAnalogSampler::LogFileCreate channel %s(%u) included", mp_ChannelList[i].p_Name, i);
            }
            else
            {
               LOG_DEBUG("CAnalogSampler::LogFileCreate channel %s(%u) not included", mp_ChannelList[i].p_Name, i);
            }
         } // for
         
         f_putc ('\r', &m_SamplerFile);
         f_putc ('\n', &m_SamplerFile);         
      }
      else
      {
         // binary mode
      }
   }
   else
   {
      LOG_WARNING("CAnalogSampler::LogFileCreate failed to create %s file", fileName);
   }   
} // CAnalogSampler::LogFileCreate

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAnalogSampler::CommandHandler(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   UINT		bytesWritten = 0;
   FRESULT  result = FR_OK;
   char     buffer[10];
   
   LOG_TRACE("CAnalogSampler::CommandHandler");
   
   
   switch(m_ActivationPacket.p_Cmd->id)
   {
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      case CMD_NEW_ANALOG_BUFFER:
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      {
         sAnalogBuffer_t * p_Buffer = 0;
         memcpy(&p_Buffer, m_ActivationPacket.p_Cmd->data, sizeof(p_Buffer));
         
         uint32_t dummy = 0;
         memcpy(&dummy, m_ActivationPacket.p_Cmd->data, 4);
         
         if(0 != p_Buffer)
         {
            // Tell we are using this resource
            taskDISABLE_INTERRUPTS();
            p_Buffer->inUse = true;
            taskENABLE_INTERRUPTS();
         
            LogomaticV2Stat(0, LOGOMATIC_STAT_ON);
            
            switch(m_LoggingMode)
            {            
               case BINARY:
               {
                  // Handle buffer to be written to flash card
                  if(FR_OK  == (result = f_write(&m_SamplerFile, p_Buffer->buffer, sizeof(p_Buffer->buffer), &bytesWritten)) &&
                     bytesWritten == sizeof(p_Buffer->buffer)
                  )
                  {}
                  else
                  {
                     LOG_WARNING("CAnalogSampler::CommandHandler unable to write all bytes to flash (error=%u written=%u wanted_to_write=%u)", result, bytesWritten, sizeof(p_Buffer->buffer));
                  }
                  break;
               } // BINARY
               case ASCII:
               {
                  for(uint32_t i = 0; i < ANALOG_SAMPLER_BUFFER_NUM_SAMPLES; i++)
                  {
                     if(0xFFFF != p_Buffer->buffer[i])
                     {
                        sprintf(buffer, "%u,", p_Buffer->buffer[i]);
                        if(FR_OK  == (result = f_write(&m_SamplerFile, buffer, strlen(buffer), &bytesWritten)) &&
                           bytesWritten == strlen(buffer)
                        )
                        {}
                        else
                        {
                           LOG_WARNING("CAnalogSampler::CommandHandler unable to write all bytes to flash (error=%u written=%u wanted_to_write=%u)", result, bytesWritten, sizeof(p_Buffer->buffer));
                        }
                     }
                     else
                     {
                        f_printf(&m_SamplerFile, "\r\n");
                     }
                  } // for
                  break;
               } // ASCII
               default:
               {}
            } // switch
         
            // Release buffer
            taskDISABLE_INTERRUPTS();
            p_Buffer->inUse = false;
            taskENABLE_INTERRUPTS();

            result = f_sync(&m_SamplerFile);
            if(FR_OK == result)
            {}
            else
            {
               LOG_WARNING("CAnalogSampler::CommandHandler f_sync failed with code %d", result);
            }
            
            LogomaticV2Stat(0, LOGOMATIC_STAT_OFF);
   
            break;
         }
         else
         {
            LOG_WARNING("CAnalogSampler::CommandHandler invalid data");
         }
      } // CMD_NEW_ANALOG_BUFFER
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      default:
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      {
         LOG_WARNING("CAnalogSampler::Task unhandled command id=%u", m_ActivationPacket.p_Cmd->id);
      }
   } // switch
   
   CSubsystem::CommandHandler();
} // CAnalogSampler::CommandHandler

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int32_t CAnalogSampler::Modeset(
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   const eLoggingMode_t mode // in
   )
{
   LOG_TRACE("CAnalogSampler::Modeset");
   
   m_LoggingMode = mode;
   
   return 0;
} // CAnalogSampler::Modeset
      
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t CAnalogSampler::SampleGet(
   const uint32_t	channelId   // in
   )
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   LOG_TRACE("CAnalogSampler::SampleGet");
   return -1;
} // CAnalogSampler::SampleGet

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t CAnalogSampler::SamplingRateSet(
   const uint32_t	rate   // in
	)
{
   LOG_TRACE("CAnalogSampler::SamplingRateSet");
   
   m_ClkDivider = configTICK_RATE_HZ / rate;
   
   LOG_DEBUG("CAnalogSampler::SamplingRateSet clkDivider=%u", m_ClkDivider);
   
   return 0;
} // CAnalogSampler::SamplingRateSet

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
