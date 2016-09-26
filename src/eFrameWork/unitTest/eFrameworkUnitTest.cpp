#include <stdlib.h>
#include "TestCScheduler.hpp"
#include "TestCSubsystem.hpp"
#include "CLog.hpp"
#include "jssUnit.hpp"

void eFrameworkUnitTest(void)
{
   CLog::VerbositySet(CLOG_OFF);
   
   CTestCScheduler::Run();
   CTestCSubsystem::Run();
   
   JSS_UNIT_PRINT_RESULTS();
   
} // eFrameworkUnitTest
