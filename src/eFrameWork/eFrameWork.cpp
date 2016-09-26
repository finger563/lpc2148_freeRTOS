#include "eFrameWork.hpp"
#include "CScheduler.hpp"

#include "CLog.hpp"

#if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
CLpc2000AnalogSampler	CeFrameWork::m_AnalogSampler(0, "analogSampler", 5*256, 2);
#endif

#if EFRAMEWORK_INCLUDE_SYSTEM_MONITOR_SYSTEM
CSystemMonitor			CeFrameWork::m_SystemMonitor(1, "systemMonitor", 3*256, 1);
#endif

#if EFRAMEWORK_INCLUDE_SYSTEM_MONITOR
CSystemMonitor		CeFrameWork::m_SystemMonitorSubsystem(1, "cpuLoad",256, 0);
#endif

#if EFRAMEWORK_INCLUDE_CONSOLE_SYSTEM
CConsoleSubsystem       CeFrameWork::m_ConsoleSubsystem(1, "console",256, 1);
#endif // EFRAMEWORK_INCLUDE_CONSOLE_SYSTEM

#ifdef FREERTOS
extern "C" void vApplicationTickHook( void )
{
  CScheduler::Scheduler(xTaskGetTickCount());
  
  #if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
  CeFrameWork::m_AnalogSampler.SamplesPush(xTaskGetTickCount());
  #endif
}

extern "C" void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed portCHAR *pcTaskName )
{
   LOG_ERROR("vApplicationStackOverflowHook %s", pcTaskName);
}

#else
#endif

