#ifndef testcscheduler_included
#define testcscheduler_included

#include <stdint.h>
#include "jssUnit.hpp"
#include "CScheduler.hpp"


class CTestCScheduler
{
   public:
      static void Run(void)
      {
         TestRegister();
      } // Run
   
   protected:
   private:
   
      static void TestRegister(void)
      {
         #define TESTREGISTER_ENTRY_NUM   50
         static CScheduler::sEntry_t entry[TESTREGISTER_ENTRY_NUM];
      
         JSS_UNIT_TEST_EQUAL(0, CScheduler::NumEntriesGet());
         // Humongous rate creates a zero clock divider
         JSS_UNIT_TEST_EQUAL(-1, CScheduler::Register(1 << 31, entry[0]));
         JSS_UNIT_TEST_EQUAL(0, CScheduler::NumEntriesGet());
         
         JSS_UNIT_TEST_EQUAL(0, CScheduler::NumEntriesGet());
         // Fails to register if rate is 0
         JSS_UNIT_TEST_EQUAL(-1, CScheduler::Register(0, entry[0]));
         JSS_UNIT_TEST_EQUAL(0, CScheduler::NumEntriesGet());
	   
         for(uint32_t i = 0; i < TESTREGISTER_ENTRY_NUM; i++)
         {
            JSS_UNIT_TEST_EQUAL(i, CScheduler::NumEntriesGet());
            JSS_UNIT_TEST_EQUAL(0, CScheduler::Register(10, entry[i]));
            JSS_UNIT_TEST_EQUAL(i + 1, CScheduler::NumEntriesGet());
         }
      } // TestRegister
   
      CTestCScheduler()
      {}
   
      ~CTestCScheduler()
      {}
}; // class CTestCScheduler

#endif // testcscheduler_included
