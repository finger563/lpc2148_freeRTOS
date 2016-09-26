#ifndef testcsubsystem_included
#define testcsubsystem_included

#include "CSubsystem.hpp"
#include "CLog.hpp"
#include "jssUnit.hpp"

class CSubsystemFriend : public CSubsystem
{
   public:
   protected:
   private:

   public:
      ~CSubsystemFriend()
	  {}

	   CSubsystemFriend(
         const uint32_t		rate,
         char * 				p_Name,
         const uint32_t		stackSize,
         const uint32_t		priority,
         const bool        critical
         )
      {
         m_Rate       = rate;
         mp_Name      = p_Name;
         m_StackSize  = stackSize;
         m_Priority   = priority;
         m_IsCritical = critical;
         m_MaxActivationPackets = 2;
      } // CSubsystemFriend::CSubsystemTest

   protected:
   private:
      CSubsystemFriend()
	  {}

   public:
   protected:
   private:
}; // CSubsystemFriend

class CSubsystemFoe : public CSubsystem
{
   public:
   protected:
   private:

   public:
      ~CSubsystemFoe()
	  {}

	   CSubsystemFoe(
         const uint32_t		rate,
         char * 				p_Name,
         const uint32_t		stackSize,
         const uint32_t		priority,
         const bool        critical
      )
      {
         m_Rate = rate;
         mp_Name = p_Name;
         m_StackSize = stackSize;
         m_Priority = priority;
         m_IsCritical = critical;
         m_MaxActivationPackets = 2;
      }

   protected:

      virtual int32_t Init(void)
      {
         return -1;
      }

   private:
      CSubsystemFoe()
	  {}

   public:
   protected:
   private:
}; // CSubsystemFoe

class CTestCSubsystem
{
   public:
      static void Run(void)
      {
         LOG_TRACE("CTestCSubsystem::Run");
         #if 0
         100811 jstoezel: can't do that in embedded systems...
         TestCSubsystem();
         #endif
         TestStartup();
      } // Run
      
   protected:
   private:
      CTestCSubsystem()
      {}
      
      ~CTestCSubsystem()
      {}
   
      static void TestCSubsystem(void)
      // Tests:
      // - CSubsystem
      // - Add
      // - Get
      // - Init
      // - IsCritical
      // - NumGet
      // - Startup
      {
         #define MAX_SUBSYSTEMS    100
         static char names[MAX_SUBSYSTEMS][50];
         CSubsystemFriend * p_Subsystems[MAX_SUBSYSTEMS];
         
         LOG_TRACE("CTestCSubsystem::TestCSubsystem");
   
         CSubsystem::ResetAll();
   
         JSS_UNIT_TEST_EQUAL(0, CSubsystem::NumGet());
         
         for(uint32_t i = 0; i < MAX_SUBSYSTEMS; i++)
         {
            sprintf(names[i], "susbsytem%u", i);
            p_Subsystems[i] = new CSubsystemFriend(i, names[i], i*i, i*i*i, false);
   
            // All previous subsystems are unchanged
            for(uint32_t j = 0; j <= i; j++)
            {
               JSS_UNIT_TEST_EQUAL(p_Subsystems[j], CSubsystem::Get(names[j]));
            }
   
            // Still the same number of subsystems
            JSS_UNIT_TEST_EQUAL(i + 1, CSubsystem::NumGet());
         } // for
   
         JSS_UNIT_TEST_EQUAL(0, CSubsystem::Startup());
   
         CSubsystem::ResetAll();
   
         delete [] *p_Subsystems;
      } // TestCSubsystem
   
      static void TestStartup(void)
      // Tests:
      // - Startup
      // - Init
      // - IsCritical
      // - NumGet
      {
         LOG_TRACE("CTestCSubsystem::TestStartup");
         
         CSubsystem::ResetAll();
   
          // Fails if critical subsystem fails
          JSS_UNIT_TEST_EQUAL(0, CSubsystem::NumGet());
          CSubsystemFoe subsystem1(1, "subsystem1", 128, 0, true);
          JSS_UNIT_TEST_EQUAL(1, CSubsystem::NumGet());
          JSS_UNIT_TEST_EQUAL(-1, CSubsystem::Startup());
   
          CSubsystem::ResetAll();
   
          // Passes if non critical subsystem fails
          JSS_UNIT_TEST_EQUAL(0, CSubsystem::NumGet());
          CSubsystemFoe subsystem2(1, "subsystem2", 128, 0, false);
          JSS_UNIT_TEST_EQUAL(1, CSubsystem::NumGet());
          JSS_UNIT_TEST_EQUAL(0, CSubsystem::Startup());
   
          CSubsystem::ResetAll();
   
          // Passes if critical subsystem succeeds
          JSS_UNIT_TEST_EQUAL(0, CSubsystem::NumGet());
          CSubsystemFriend subsystem3(1, "subsystem3", 128, 0, true);
          JSS_UNIT_TEST_EQUAL(1, CSubsystem::NumGet());
          JSS_UNIT_TEST_EQUAL(0, CSubsystem::Startup());
   
          CSubsystem::ResetAll();
   
          // Passes if non critical subsystem passes
          JSS_UNIT_TEST_EQUAL(0, CSubsystem::NumGet());
          CSubsystemFriend subsystem4(1, "subsystem4", 128, 0, false);
          JSS_UNIT_TEST_EQUAL(1, CSubsystem::NumGet());
          JSS_UNIT_TEST_EQUAL(0, CSubsystem::Startup());
   
          CSubsystem::ResetAll();
   
      } // TestStartup
}; // class CTestCSubsystem

#endif // testcsubsystem_included
