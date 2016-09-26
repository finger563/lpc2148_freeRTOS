#ifndef jssunittest_included
#define jssunittest_included

#include <stdint.h>
#include <stdio.h>

#include "CLog.hpp"

extern uint32_t   jssUnitTotalTests;
extern uint32_t   jssUnitSucceededTests;

#define JSS_UNIT_ERROR                          LOG_ERROR
#define JSS_UNIT_INFO                           LOG_INFO
#define JSS_UNIT_TEST_EQUAL(__argA, __argB)     if(__argA == __argB){jssUnitSucceededTests++;}else{CLog::VerbositySet(CLOG_ERROR);JSS_UNIT_ERROR("%s, %u, %s NOT EQUAL %s", __FILE__, __LINE__, #__argA, #__argB);CLog::VerbositySet(CLOG_OFF);}jssUnitTotalTests++;
#define JSS_UNIT_PRINT_RESULTS()                CLog::VerbositySet(CLOG_INFO);LOG_INFO("UNIT TEST RESULTS");LOG_INFO("TOTAL TESTS:\t\t%08u", jssUnitTotalTests);LOG_INFO("TOTAL SUCCEEDED TESTS:\t%08u", jssUnitSucceededTests);LOG_INFO("TOTAL FAILED TESTS:\t\t%08u", jssUnitTotalTests-jssUnitSucceededTests);


#endif // jssunittest_included
