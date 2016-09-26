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

#include <string.h>        // strncmp
#include "CSubsystem.hpp"
#include "CScheduler.hpp"
#include "CLog.hpp"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MACROS
#define CSUBSYSTEM_MAX_NAME_LEN     (50)
#define SUBSYSTEM_MAX_MESSAGES         (10)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// STATIC VARIABLES
CSubsystem * 			   CSubsystem::mp_SubsystemList = 0;
CSubsystem::sCommand_t	CSubsystem::m_CmdPool[SUBSYSTEM_MAX_MESSAGES];

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSubsystem::CSubsystem():
   m_TaskHandle(0),
   m_ActivationQueue(0),
   mp_Name(0),
   m_IsCritical(true),         // By default a subsystem is critical
   m_Priority(0),
   m_Rate(0),
   m_RunTask(true),
   m_StackSize(512),
   m_ActivationTimeout(2000),
   m_MaxActivationPackets(1),
   mp_NextSubsystem(0)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
#ifdef CLOG_FROM_CTORS
   LOG_TRACE("CSubsystem::CSubsystem");
#endif
   // Upon instantiation this subsystem is added to list of subsystems that compose the system
   Add();
} // CSubsystem::CSubsystem

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSubsystem::~CSubsystem()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
#ifdef CLOG_FROM_CTORS
   LOG_TRACE("CSubsystem::~CSubsystem");
#endif
} // CSubsystem::~CSubsystem

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSubsystem::WaitForActivation(
   const uint32_t timeout  // in
)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   LOG_TRACE("CSubsystem::WaitForActivation");
#ifdef FREERTOS   
   if(pdTRUE == xQueueReceive(m_ActivationQueue, &m_ActivationPacket, timeout))
   {
      LOG_DEBUG("CSubsystem::WaitForActivation activation");
   }
   else
   {
      // Time out
      m_ActivationPacket.type = ACTIVATION_TYPE_TIMEOUT;
      LOG_DEBUG("CSubsystem::WaitForActivation timeout");
   }
#else
#endif

} // CSubsystem::WaitForActivation

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSubsystem::Add(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   CSubsystem *   p_Subsystem	= 0;

#ifdef CLOG_FROM_CTORS
   LOG_TRACE("CSubsystem::Add");
   #if 0
   100618 jstoezel: The default constructor is called before any other constructor, including children constructors.
   As such mp_Name is not initialized, and should not be used.
   CLog::Debug("CSubsystem::Add name=%s", mp_Name);
   #endif
#endif

   if(0 == mp_SubsystemList)
   {
   #ifdef CLOG_FROM_CTORS
      LOG_DEBUG("CSubsystem::Add first entry in list");
   #endif
      mp_SubsystemList       = this;
      this->mp_NextSubsystem = 0;
   }
   else
   {
      p_Subsystem = mp_SubsystemList;

      while(0 != p_Subsystem->mp_NextSubsystem)
      {
      #ifdef CLOG_FROM_CTORS
         LOG_DEBUG("CSubsystem::Add skipping name=%s", p_Subsystem->mp_Name);
      #endif
         p_Subsystem = p_Subsystem->mp_NextSubsystem;
      }

   #ifdef CLOG_FROM_CTORS
      LOG_DEBUG("CSubsystem::Add inserting after name=%s", p_Subsystem->mp_Name);
   #endif
      p_Subsystem->mp_NextSubsystem = this;
      this->mp_NextSubsystem        = 0;
   }
} // CSubsystem::Add

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t CSubsystem::ClockGet(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   return (xTaskGetTickCount());
} // CSubsystem::ClockGet

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSubsystem::sCommand_t * CSubsystem::CommandGet(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   uint32_t					      i		   = 0;
   CSubsystem::sCommand_t *   result   = 0;
   
   LOG_TRACE("CSubsystem::CommandGet");
   
   // Ideally lock the access to the pool
   while(true == m_CmdPool[i].inUse &&
          i < SUBSYSTEM_MAX_MESSAGES
		  )
   {
      i++;
   } // while
   
   if(SUBSYSTEM_MAX_MESSAGES == i)
   {
      // Could not find a free command
      LOG_WARNING("CSubsystem::CommandGet no command available");
   }
   else
   {
      m_CmdPool[i].inUse = true;
      result = &(m_CmdPool[i]);
   }
   
   return &(m_CmdPool[i]);
} // CSubsystem::CommandGet

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSubsystem::CommandHandler(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   LOG_TRACE("CSubsystem::CommandHandler");
   
   // Release the command
   CommandRelease(m_ActivationPacket.p_Cmd);
} // CSubsystem::CommandHandler

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSubsystem::CommandRelease(
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   CSubsystem::sCommand_t * const p_Cmd   // in
   )
{
   // ideally lock the command pool.
   LOG_TRACE("CSubsystem::CommandRelease");
   p_Cmd->inUse = false;
} // CSubsystem::CommandRelease


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int32_t CSubsystem::CommandSend(
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   CSubsystem const * const   	           p_Subsystem, // in
   CSubsystem::sCommand_t const * const   p_Cmd          // in
   )
{
   int32_t        result = -1;
   sActivation_t  activation;
   
   if(0 != p_Subsystem &&
      0 != p_Cmd
	  )
   {
      activation.type = CSubsystem::ACTIVATION_TYPE_CMD;
#ifdef FREERTOS
      if(pdTRUE == xQueueSend(p_Subsystem->m_ActivationQueue, &activation, portTICK_RATE_MS))
      {
         result = 0;
      }
      else
      {}
#else
#endif
   }
   else
   {}
   
   return result;
} // CSubsystem::CommandSend

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSubsystem * CSubsystem::Get(
   const uint32_t	id // in
   )
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   CSubsystem *   p_Result    = mp_SubsystemList;
   uint32_t       cnt         = 0;
   
   LOG_TRACE("CSubsystem::Get");
   
   while(cnt < id &&
          0 != p_Result
         )
   {
      LOG_DEBUG("CSubsystem::Get id=%u cnt=%u", id, cnt);
      cnt++;
      p_Result = p_Result->mp_NextSubsystem;
   } // while
   
   return p_Result;
} // CSubsystem::Get

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSubsystem * CSubsystem::Get(
   char const * const   p_Name // in
   )
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   CSubsystem *   p_Subsystem = mp_SubsystemList;
   
   LOG_TRACE("CSubsystem::Get");
   LOG_DEBUG("CSubsystem::Get name=%s", p_Name);
   
   while(0 != p_Subsystem)
   {
      LOG_DEBUG("CSubsystem::Get current_name=%s", p_Subsystem->mp_Name);
      if(0 == strncmp(p_Name, p_Subsystem->mp_Name, CSUBSYSTEM_MAX_NAME_LEN))
      {
         LOG_DEBUG("CSubsystem::Get match");
         break;
      }
      else
      {
         p_Subsystem = p_Subsystem->mp_NextSubsystem;
      }
   } // while

   return p_Subsystem;
} // CSubsystem::Get

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int32_t CSubsystem::Init(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   int32_t        result            = -1;
   portBASE_TYPE  taskCreatResult   = pdPASS;
   
   LOG_TRACE("CSubsystem::Init");
   LOG_DEBUG("CSubsystem::Init name=%s stack_size=%u priority=%u max_act_packet=%u act_packset_size=%u", mp_Name, m_StackSize, m_Priority, m_MaxActivationPackets, sizeof(m_MaxActivationPackets));
   
#ifdef FREERTOS
   m_ActivationQueue = xQueueCreate(m_MaxActivationPackets, sizeof(m_ActivationPacket));
   
   if(0 == m_ActivationQueue)
   {
      result = -1;
      LOG_WARNING("CSubsystem::Init failed to create activation queue");
   }
   else
   {
      m_SchedulerEntry.p_ActivationQueue = &m_ActivationQueue;
      LOG_DEBUG("CSubsystem::Init succeeded to create activation queue address=%08X", &m_ActivationQueue);
      result = 0;
   }
   
   LOG_DEBUG("CSubsystem::Init heap space left %u", xPortGetFreeHeapSize());

   if(-1 != result &&
      0 != m_Rate &&
      -1 != CScheduler::Register(m_Rate, m_SchedulerEntry)
      )
   {
      LOG_DEBUG("CSubsystem::Init CScheduler::Register OK");
      if(
       pdPASS == (taskCreatResult = xTaskCreate(CSubsystem::TaskWrapper, reinterpret_cast<signed char const * const>(mp_Name), m_StackSize, this, m_Priority, &m_TaskHandle)))
      {
         LOG_DEBUG("CSubsystem::Init xTaskCreate succeeded");
         result = 0;
      }
      else
      {
         LOG_WARNING("CSubsystem::Init xTaskCreate failed with code %d", taskCreatResult);
         result = -1;
      }
   }
   else if(0 == m_Rate)
   {
      if(
         pdPASS == (taskCreatResult = xTaskCreate(CSubsystem::TaskWrapper, reinterpret_cast<signed char const * const>(mp_Name), m_StackSize, this, m_Priority, &m_TaskHandle))
         )
      {
         LOG_DEBUG("CSubsystem::Init xTaskCreate succeeded");
         result = 0;
      }
      else
      {
         LOG_WARNING("CSubsystem::Init xTaskCreate failed with code %d", taskCreatResult);
         result = -1;
      }
   }
   else
   {
      LOG_WARNING("CSubsystem::Init CScheduler::Register failed");
      result = -1;
   }
   
   LOG_DEBUG("CSubsystem::Init heap space left %u", xPortGetFreeHeapSize());
   
#else

   result = 0;

#endif

   return result;
} // CSubsystem::Init

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CSubsystem::IsCritical(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   LOG_TRACE("CSubsystem::IsCritical");
   return m_IsCritical;
} // CSubsystem::IsCritical

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t CSubsystem::NumGet(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   uint32_t result = 0;

   LOG_TRACE("CSubsystem::NumGet");

   while(0 != CSubsystem::Get(result))
   {
      result++;
   }

   return result;
} // CSubsystem::NumGet

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSubsystem::PeriodicHandler(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   LOG_TRACE("CSubsystem::PeriodicHandler");
   CommandRelease(m_ActivationPacket.p_Cmd);
} // CSubsystem::PeriodicHandler

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSubsystem::ResetAll(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   CSubsystem *   p_Subsystem = mp_SubsystemList;
   LOG_TRACE("CSubsystem::ResetAll");

   while(0 != p_Subsystem)
   {
      p_Subsystem->UnInit();

      p_Subsystem = p_Subsystem->mp_NextSubsystem;
   } // while

   // For now only kill the list at the base, do not go after each subsystem.
   mp_SubsystemList = 0;
} // CSubsystem::ResetAll

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int32_t CSubsystem::Startup(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   int32_t        result      = -1;
   CSubsystem *   p_Subsystem = mp_SubsystemList;

   LOG_TRACE("CSubsystem::Startup");

   LOG_INFO("CSubsystem::Startup %u subsystem to initialize", CSubsystem::NumGet());
   
#ifdef FREERTOS
   LOG_DEBUG("CSubsystem::Startup heap space left %u", xPortGetFreeHeapSize());
#endif

   // Free the command pool
   for(uint32_t i = 0; i < SUBSYSTEM_MAX_MESSAGES; i ++)
   {
      m_CmdPool[i].id      = 0;
      m_CmdPool[i].inUse   = false;
      m_CmdPool[i].dataLen = 0;
   } // for

   while(0 != p_Subsystem)
   {
      LOG_DEBUG("CSubsystem::Startup subsystem_name=%s", p_Subsystem->mp_Name);

      if(0 == p_Subsystem->Init())
      {
         LOG_DEBUG("CSubsystem::Startup %s init succeeded", p_Subsystem->mp_Name);
         result = 0;
      }
      else
      {
         if(true == p_Subsystem->IsCritical())
         {
            LOG_ERROR("CSubsystem::Startup critical %s init failed - ABORTING", p_Subsystem->mp_Name);
            result = -1;
            ResetAll();
            break;
         }
         else
         {
            LOG_WARNING("CSubsystem::Startup non critical %s init failed - CONTINUING", p_Subsystem->mp_Name);
            result = 0;
         }
      }

#ifdef FREERTOS
      LOG_DEBUG("CSubsystem::Startup heap space left %u", xPortGetFreeHeapSize());
#endif

      p_Subsystem = p_Subsystem->mp_NextSubsystem;
   } // while

#ifdef FREERTOS
   LOG_DEBUG("CSubsystem::Startup heap space left %u", xPortGetFreeHeapSize());
#endif

   #if 0
   100812 jstoezel: removed this as it does not allow for an easy unit test. Multitasking must be invoked after the startup method is called.
   // Start the multitasking environment
   vTaskStartScheduler();
   #endif

   return result;
} // CSubsystem::Startup

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSubsystem::Task(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   LOG_TRACE("CSubsystem::Task");
   
   while(true == m_RunTask)
   {
      LOG_DEBUG("CSubsystem::Task going to wait for activation");
      
      // Wait for activation
      WaitForActivation(m_ActivationTimeout);
      
      switch(m_ActivationPacket.type)
      {
         case ACTIVATION_TYPE_CMD:
         {
            LOG_DEBUG("CSubsystem::Task command received");
            CommandHandler();
            break;
         }
         case ACTIVATION_TYPE_SCHEDULER:
         {
            LOG_DEBUG("CSubsystem::Task periodic activation");
            PeriodicHandler();
            break;
         }
         case ACTIVATION_TYPE_TIMEOUT:
         {
            LOG_DEBUG("CSubsystem::Task activation timeout");
            TimeoutHandler();
            break;
         }
         default:
         {
            LOG_WARNING("CSubsystem::Task invalid activation type");
         }
      } // switch
      
   } // while
} // CSubsystem::Task

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSubsystem::TaskWrapper(
   void * p_This  // in
   )
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   LOG_TRACE("CSubsystem::TaskWrapper");
   reinterpret_cast<CSubsystem *>(p_This)->Task();
} // CSubsystem::TaskWrapper

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t CSubsystem::TickGet(void) const
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   LOG_TRACE("CSubsystem::TickGet");
   return CScheduler::TickGet(m_SchedulerEntry);
} // CSubsystem::TickGet

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSubsystem::TimeoutHandler(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   LOG_TRACE("CSubsystem::TimeoutHandler");
} // CSubsystem::TimeoutHandler

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSubsystem::UnInit(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   LOG_TRACE("CSubsystem::UnInit");
   
   #if 0
   if(0 != m_TaskHandle)
   {
      vTaskDelete(m_TaskHandle);
   }
   else
   {}
   
   if(0 != m_ActivationQueue)
   {
      vQueueDelete(m_ActivationQueue);
   }
   else
   {}
   #endif
   
} // CSubsystem::UnInit
