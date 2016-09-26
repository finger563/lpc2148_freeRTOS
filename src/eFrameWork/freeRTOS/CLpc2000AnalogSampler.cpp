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
#include "CLpc2000AnalogSampler.hpp"
#include "CLog.hpp"
#include "logomatic_v2.hpp"

#include <stdlib.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// METHODS

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CLpc2000AnalogSampler::CLpc2000AnalogSampler(
   const uint32_t rate,       // in
   char const *  p_Name,      // in
   const uint32_t stackSize,  // in
   const uint32_t priority    // in
   )
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   m_Rate         = rate;
   mp_Name        = p_Name;
   m_StackSize    = stackSize;
   m_Priority     = priority;
   mp_ChannelList = m_ChannelList;
   m_NumChannels  =LPC2000_ANALOG_SAMPLER_NUM_CHANNELS;
} // CLpc2000AnalogSampler::CLpc2000AnalogSampler

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CLpc2000AnalogSampler::CLpc2000AnalogSampler(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{} // CLpc2000AnalogSampler::CLpc2000AnalogSampler

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CLpc2000AnalogSampler::~CLpc2000AnalogSampler(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{} // CLpc2000AnalogSampler::~CLpc2000AnalogSampler

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int32_t CLpc2000AnalogSampler::Init(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   uint32_t volatile *	volatile	p_BaseAddr	= 0;
   
   LOG_TRACE("CLpc2000AnalogSampler::Init");
   
   // Global initialization of ADC0 and ADC1
   // CONTROL REGISTER
   // ADC0
   p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE0034000);
   *p_BaseAddr = 0;
   *p_BaseAddr = (0x02 << 8) | (1 << 21)/* | (1 << 16)*/;
   
   // ADC1
   p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE0060000);
   *p_BaseAddr = 0;
   *p_BaseAddr = (0x02 << 8) | (1 << 21) /*| (1 << 16)*/;
   
   // INT ENABLE
   // ADC0
   p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE003400C);
   *p_BaseAddr = 0;
   // ADC1
   p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE006000C);
   *p_BaseAddr = 0;
   
   
   for(uint32_t channelId = 0; channelId < LPC2000_ANALOG_SAMPLER_NUM_CHANNELS; channelId++)
   {
      if(channelId < LPC2000_ANALOG_SAMPLER_NUM_CHANNELS_ADC0)
      {
         // ADC0
         p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE0034000);
      }
      else
      {
         // ADC1
         p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE0060000);
      }
      
      if(true == mp_ChannelList[channelId].enabled)
      {
         *p_BaseAddr |= ((1 << (channelId % 8)) & 0xFF);
      }
      else
      {}
   }
   
   p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE0034000);
   p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE0060000);
   
   // Start sampling
   #if 1
   p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE0034000);
   *p_BaseAddr |= (1 << 16);
   p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE0060000);
   *p_BaseAddr |= (1 << 16);
   #endif
   
   return CAnalogSampler::Init();
} // CLpc2000AnalogSampler::Init

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t CLpc2000AnalogSampler::SampleGet(
   const uint32_t	channelId   // in
   )
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   uint32_t                         result      = 0;
   uint32_t volatile *	volatile	p_BaseAddr	= 0;
   
   LOG_TRACE("CLpc2000AnalogSampler::SampleGet");
   
#if 0   
   if(channelId < LPC2000_ANALOG_SAMPLER_NUM_CHANNELS_ADC0)
   {
      // ADC0
      p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE0034010);
   }
   else
   {
      // ADC1
      p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE0060010);
   }
   
   result = *(p_BaseAddr + (channelId % 8));
#else
   switch(channelId)
   {
      case 0:
      {
         p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE0034010);
         break;
      }
      case 1:
      {
         p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE0034014);
         break;
      }
      case 2:
      {
         p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE0034018);
         break;
      }
      case 3:
      {
         p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE003401C);
         break;
      }
      case 4:
      {
         p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE0034020);
         break;
      }
      case 5:
      {
         p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE0034024);
         break;
      }
      case 6:
      {
         p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE0034028);
         break;
      }
      case 7:
      {
         p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE003402C);
         break;
      }
      case 8:
      {
         p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE0060010);
         break;
      }
      case 9:
      {
         p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE0060014);
         break;
      }
      case 10:
      {
         p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE0060018);
         break;
      }
      case 11:
      {
         p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE006001C);
         break;
      }
      case 12:
      {
         p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE0060020);
         break;
      }
      case 13:
      {
         p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE0060024);
         break;
      }
      case 14:
      {
         p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE0060028);
         break;
      }
      case 15:
      {
         p_BaseAddr = reinterpret_cast<uint32_t volatile * volatile>(0xE006002C);
         break;
      }
      default:
      {}
   }
   result = *(p_BaseAddr);
#endif
   result = ((result >> 6) & 0x3FF);
   
   // Convert to volts
   result = result * 3300 / 1024;
   
   return result;
} // CLpc2000AnalogSampler::SampleGet

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
