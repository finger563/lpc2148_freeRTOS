/*******************************************************************************
*                                                                              *
* eFramework CPU load calibration                                              *
*                                                                              *
* AUTHOR: Jean-Sebastien Stoezel                                               *
*         js.stoezel@gmail.com                                                 *
*                                                                              *
*******************************************************************************/

/*******************************************************************************
*                                                                              *
* INCLUDES                                                                     *
*                                                                              *
*******************************************************************************/

#include <stdint.h>
#include "Logomatic_V2.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "CLog.hpp"
#include "CSubsystem.hpp"


/*******************************************************************************
*                                                                              *
* TYPES                                                                        *
*                                                                              *
*******************************************************************************/

/*******************************************************************************
*                                                                              *
* PROTOTYPES                                                                   *
*                                                                              *
*******************************************************************************/
extern "C" int    main( void );


/*******************************************************************************
*                                                                              *
* abort                                                                        *
*                                                                              *
*******************************************************************************/
extern "C" void abort(void)
{
  while (1 == 1)
  {}
} // abort

class CCpuLoadSubsystemCalibration : public CSubsystem
{
   public:
   protected:
   private:

   public:
      CCpuLoadSubsystemCalibration(
	    const uint32_t		rate,		// in, periodic activation of the susbystem
        char * 			    p_Name,		// in, name of the subsystem
        const uint32_t		stackSize,	// in, statck size
        const uint32_t		priority	// in, priority of the subsystem
		)
      {
	     m_Rate						= rate;
         mp_Name				    = p_Name;
         m_StackSize				= stackSize;
         m_Priority					= priority;
	  } // CCpuLoadSubsystemCalibration
	  
	  ~CCpuLoadSubsystemCalibration()
	  {}

   protected:
	  
	  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // NAME:          Init
      // DESCRIPTION:   Initialize the subsystem
      // RETURN VALUE:  None.
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	  virtual int32_t  Init(void)
	  {
         mp_CpuLoadSubsystem = reinterpret_cast<CCpuLoadSubsystem *>(CSubsystem::Get("cpuLoad"));
		 
		 return CSubsystem::Init();
	  } // Init
	  
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // NAME:          PeriodicHandler
      // DESCRIPTION:   
      // RETURN VALUE:  None.
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      virtual void PeriodicHandler(void)
	  {
	     uint32_t 	maxCnt 	= 0;
		 uint32_t	cpuLoad	= 0;
	     
		 // Get the CPU load counter
		 cpuLoad = mp_CpuLoadSubsystem->CpuLoadGet(maxCnt);
		 
		 // Display it
		 LOG("CPU_MAX=%u (%u%%)", maxCnt, cpuLoad);
	  }
   
   private:
      CCpuLoadSubsystemCalibration()
	  {}

   public:
   protected:
   private:
      CCpuLoadSubsystem * mp_CpuLoadSubsystem;
}; // class CCpuLoadSubsystemCalibration

static CCpuLoadSubsystemCalibration	cpuLoadSubsystemCalibration(1, "cpu_calibration", 1024, 0);

/*******************************************************************************
*                                                                              *
* main                                                                         *
*                                                                              *
*******************************************************************************/
int main( void )
{   
   // Initialize the logomatic board processor and preipherals
   LogomaticV2Init();
   
   LOG_TRACE("main");
   
   CLog::VerbositySet(CLOG_OFF);
   
   // Initialize the logomatic board processor and preipherals
   LogomaticV2Init();
   
   // Initialize all the subsystems instantiated
   CSubsystem::Startup();
   
   // start the multitasking environment.   
   vTaskStartScheduler();
   
   return 0;
} // main
