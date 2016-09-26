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
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "CLogoDynamite.hpp"
#include "CLog.hpp"
#include "ff.h"
#include "Logomatic_V2.hpp"


#include "CAnalogSampler.hpp"
#include "CSubsystem.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIABLES
CLogoDynamite::sConfigurationCmd_t	CLogoDynamite::m_CmdList[] = 
  {
    // Default configuration
    // CMD				// HELP				// VALUE
    {"FREQUENCY",		"",		      "100"},
    {"P1",				"",				"Y"},
    {"P2",				"",				"Y"},
    {"P3",				"",				"Y"},
    {"P4",				"",				"Y"},
    {"P5",				"",				"Y"},
    {"P6",				"",				"Y"},
    {"P7",				"",				"Y"},
    {"P8",				"",				"Y"},
    {"VBAT",				"",				"Y"},
    {"MODE",				"",				"ASCII"},
    {"VERBOSITY",		"",				"WARNING"},
  };
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// METHODS

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CLogoDynamite::CLogoDynamite(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{} // CLogoDynamite::CLogoDynamite

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CLogoDynamite::~CLogoDynamite(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{} // CLogoDynamite::~CLogoDynamite

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLogoDynamite::BannerPrint(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
  LOG("\r\n");
  LOG("\r\n");
  LOG("\r\n+----------------------------------------------------------------------+");
  LOG("\r\n|                                                                      |");
  LOG("\r\n|         ______________________________       . \\  | / .             |");
  LOG("\r\n|        /                            / \\        \\ \\ / /            |");
  LOG("\r\n|       |       LogoDYNAMITE!        | {========= >- -<                |");
  LOG("\r\n|        \\____________________________\\_/        / / \\ \\           |");
  LOG("\r\n|                                              . /  | \\ .             |");
  LOG("\r\n|                                                                      |");
  LOG("\r\n|                                                                      |");
  LOG("\r\n+----------------------------------------------------------------------+");
  LOG("\r\n>");
} // CLogoDynamite::BAnnerPrint

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int32_t	CLogoDynamite::ConfigurationApply(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
  int32_t           result            = -1;
  uint32_t	         frequency         = 0;
  CAnalogSampler *  mp_AnalogSampler  = reinterpret_cast<CAnalogSampler *>(CSubsystem::Get("analogSampler"));
   
  LOG_TRACE("CLogoDynamite::ConfigurationApply");
   
#if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
  if(0 == mp_AnalogSampler)
    {
      LOG_WARNING("CLogoDynamite::ConfigurationApply could not get %s subsystem", "analogSampler");
    }
  else
    {}
#endif

  for(uint32_t i = 0; i < (sizeof(m_CmdList) / sizeof(m_CmdList[0])); i++)
    {
      if(0 == strncmp("FREQUENCY", m_CmdList[i].cmd, sizeof(m_CmdList[i].cmd)))
	{
	  sscanf(m_CmdList[i].value, "%u", &frequency);
	  if(frequency > configTICK_RATE_HZ)
	    {
	      frequency = configTICK_RATE_HZ;
	      LOG_WARNING("CLogoDynamite::ConfigurationApply FREQUENCY was to high. Limited to =%u", frequency);
	    }
	  else
	    {}
	  LOG_INFO("CLogoDynamite::ConfigurationApply FREQUENCY=%u", frequency);
#if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
	  mp_AnalogSampler->SamplingRateSet(frequency);
#endif
	}
      else if(0 == strncmp("P1", m_CmdList[i].cmd, sizeof(m_CmdList[i].cmd)))
	{
	  LOG_DEBUG("CLogoDynamite::ConfigurationApply P1=%c", m_CmdList[i].value[0]);
	  if('Y' == m_CmdList[i].value[0])
	    {
#if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
	      mp_AnalogSampler->ChannelInit(4, true, "P1");
#endif
	    }
	  else
	    {
#if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
	      mp_AnalogSampler->ChannelInit(4, false, "P1");
#endif
	    }
	}
      else if(0 == strncmp("P2", m_CmdList[i].cmd, sizeof(m_CmdList[i].cmd)))
	{
	  LOG_DEBUG("CLogoDynamite::ConfigurationApply P2=%c", m_CmdList[i].value[0]);
	  if('Y' == m_CmdList[i].value[0])
	    {
#if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
	      mp_AnalogSampler->ChannelInit(3, true, "P2");
#endif
	    }
	  else
	    {
#if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
	      mp_AnalogSampler->ChannelInit(3, false, "P2");
#endif
	    }
	}
      else if(0 == strncmp("P3", m_CmdList[i].cmd, sizeof(m_CmdList[i].cmd)))
	{
	  LOG_DEBUG("CLogoDynamite::ConfigurationApply P3=%c", m_CmdList[i].value[0]);
	  if('Y' == m_CmdList[i].value[0])
	    {
#if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
	      mp_AnalogSampler->ChannelInit(2, true, "P3");
#endif
	    }
	  else
	    {
#if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
	      mp_AnalogSampler->ChannelInit(2, false, "P3");
#endif
	    }
	}
      else if(0 == strncmp("P4", m_CmdList[i].cmd, sizeof(m_CmdList[i].cmd)))
	{
	  LOG_DEBUG("CLogoDynamite::ConfigurationApply P4=%c", m_CmdList[i].value[0]);
	  if('Y' == m_CmdList[i].value[0])
	    {
#if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
	      mp_AnalogSampler->ChannelInit(5, true, "P4");
#endif
	    }
	  else
	    {
#if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
	      mp_AnalogSampler->ChannelInit(5, false, "P4");
#endif
	    }
	}
      else if(0 == strncmp("P5", m_CmdList[i].cmd, sizeof(m_CmdList[i].cmd)))
	{
	  LOG_DEBUG("CLogoDynamite::ConfigurationApply P5=%c", m_CmdList[i].value[0]);
	  if('Y' == m_CmdList[i].value[0])
	    {
#if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
	      mp_AnalogSampler->ChannelInit(15, true, "P5");
#endif
	    }
	  else
	    {
#if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
	      mp_AnalogSampler->ChannelInit(15, false, "P5");
#endif
	    }
	}
      else if(0 == strncmp("P6", m_CmdList[i].cmd, sizeof(m_CmdList[i].cmd)))
	{
	  LOG_DEBUG("CLogoDynamite::ConfigurationApply P6=%c", m_CmdList[i].value[0]);
	  if('Y' == m_CmdList[i].value[0])
	    {
#if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
	      mp_AnalogSampler->ChannelInit(14, true, "P6");
#endif
	    }
	  else
	    {
#if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
	      mp_AnalogSampler->ChannelInit(14, false, "P6");
#endif
	    }
	}
      else if(0 == strncmp("P7", m_CmdList[i].cmd, sizeof(m_CmdList[i].cmd)))
	{
	  LOG_DEBUG("CLogoDynamite::ConfigurationApply P7=%c", m_CmdList[i].value[0]);
	  if('Y' == m_CmdList[i].value[0])
	    {
#if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
	      mp_AnalogSampler->ChannelInit(10, true, "P7");
#endif
	    }
	  else
	    {
#if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
	      mp_AnalogSampler->ChannelInit(10, false, "P7");
#endif
	    }
	}
      else if(0 == strncmp("P8", m_CmdList[i].cmd, sizeof(m_CmdList[i].cmd)))
	{
	  LOG_DEBUG("CLogoDynamite::ConfigurationApply P8=%c", m_CmdList[i].value[0]);
	  if('Y' == m_CmdList[i].value[0])
	    {
#if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
	      mp_AnalogSampler->ChannelInit(11, true, "P8");
#endif
	    }
	  else
	    {
#if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
	      mp_AnalogSampler->ChannelInit(11, false, "P8");
#endif
	    }
	}
      else if(0 == strncmp("VBAT", m_CmdList[i].cmd, sizeof(m_CmdList[i].cmd)))
	{
	  LOG_DEBUG("CLogoDynamite::ConfigurationApply VBAT=%c", m_CmdList[i].value[0]);
	  if('Y' == m_CmdList[i].value[0])
	    {
#if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
	      mp_AnalogSampler->ChannelInit(12, true, "VBAT");
#endif
	    }
	  else
	    {
#if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
	      mp_AnalogSampler->ChannelInit(12, false, "VBAT");
#endif
	    }
	}
      else if(0 == strncmp("MODE", m_CmdList[i].cmd, sizeof(m_CmdList[i].cmd)))
	{
	  LOG_DEBUG("CLogoDynamite::ConfigurationApply MODE=%s", m_CmdList[i].value);
	  if(0 == strncmp("ASCII", m_CmdList[i].value, sizeof(m_CmdList[i].value)))
	    {
#if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
	      mp_AnalogSampler->Modeset(CAnalogSampler::ASCII);
#endif
	    }
	  else if(0 == strncmp("BINARY", m_CmdList[i].value, sizeof(m_CmdList[i].value)))
	    {
#if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
	      mp_AnalogSampler->Modeset(CAnalogSampler::BINARY);
#endif
	    }
	  else
	    {
	      LOG_WARNING("CLogoDynamite::ConfigurationApply invalid mode=%s - using ASCII", m_CmdList[i].value);
#if EFRAMEWORK_INCLUDE_ANALOG_SAMPLER
	      mp_AnalogSampler->Modeset(CAnalogSampler::ASCII);
#endif
	    }
	}
      else if(0 == strncmp("VERBOSITY", m_CmdList[i].cmd, sizeof(m_CmdList[i].cmd)))
	{
	  LOG_DEBUG("CLogoDynamite::ConfigurationApply VERBOSITY=%s", m_CmdList[i].value);
	  if(0 == strncmp("OFF", m_CmdList[i].value, sizeof(m_CmdList[i].value)))
	    {
	      CLog::VerbositySet(0);
	    }
	  else if(0 == strncmp("ERROR", m_CmdList[i].value, sizeof(m_CmdList[i].value)))
	    {
	      CLog::VerbositySet(1);
	    }
	  else if(0 == strncmp("WARNING", m_CmdList[i].value, sizeof(m_CmdList[i].value)))
	    {
	      CLog::VerbositySet(2);
	    }
	  else if(0 == strncmp("INFO", m_CmdList[i].value, sizeof(m_CmdList[i].value)))
	    {
	      CLog::VerbositySet(3);
	    }
	  else
	    {
	      LOG_WARNING("CLogoDynamite::ConfigurationApply invalid verbosity=%s - using default", m_CmdList[i].value);
	    }
	}
      else
	{}
    } // for(uint32_t i = 0; i < (sizeof(m_CmdList) / sizeof(m_CmdList[0])); i++)

  return result;
} // CLogoDynamite::ConfigurationApply

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int32_t	CLogoDynamite::ConfigurationRead(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
  LOG_TRACE("CLogoDynamite::ConfigurationRead");
   
  int32_t	result = -1;
  FIL 		p_File;
  char		buffer[100];
  char *	p_Value = NULL;
  int		endOfLine = 0;

  if(FR_OK != f_open(&p_File, LOGO_DYNAMITE_CONF_FILE_NAME, FA_READ))
    {
      LOG_WARNING("CLogoDynamite::ConfigurationRead configuration file (%s) does not exist. Creating it.", LOGO_DYNAMITE_CONF_FILE_NAME);
      // The configuration file does not exist. Create the default one.
      DefaultConfigurationWrite();
    }
  else
    {
      LOG_DEBUG("CLogoDynamite::ConfigurationRead configuration file (%s) found.", LOGO_DYNAMITE_CONF_FILE_NAME);
      LOG_DEBUG("CLogoDynamite::ConfigurationRead looking for %u maximum commands", sizeof(m_CmdList) / sizeof(m_CmdList[0]));
	  
      // For each line in the file
      while(NULL != f_gets(buffer, sizeof(buffer), reinterpret_cast<FIL *>(&p_File)))
	{
	  LOG_DEBUG("CLogoDynamite::ConfigurationRead found directive %s", buffer);
         
	  // Parse the file for all the commands supported
	  for(uint32_t i = 0; i < (sizeof(m_CmdList) / sizeof(m_CmdList[0])); i++)
	    {
	      // Check if the line matches any command
	      p_Value = strstr(buffer, m_CmdList[i].cmd);

	      // If it does, set the configuration accordingly
	      if(NULL != p_Value)
		{
		  LOG_DEBUG("CLogoDynamite::ConfigurationRead found match");

		  strncpy(m_CmdList[i].value, p_Value + strlen(m_CmdList[i].cmd) + 1, sizeof(m_CmdList[i].value));
		  endOfLine = strcspn(m_CmdList[i].value, "\r\n");

		  if(strlen(m_CmdList[i].value) != static_cast<unsigned int>(endOfLine))
		    {
		      m_CmdList[i].value[endOfLine] = '\0';
		    }
		  else
		    {}
		  break;
		}
	      else
		{}
	    } // for
	} // while

      f_close(&p_File);
    } // if

  return result;
} // CLogoDynamite::ConfigurationRead

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int32_t	CLogoDynamite::DefaultConfigurationWrite(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
  LOG_TRACE("CLogoDynamite::DefaultConfigurationWrite");
   
  int32_t  result = -1;
  FIL      p_File;

  if(FR_OK == f_open(&p_File, LOGO_DYNAMITE_CONF_FILE_NAME, FA_WRITE | FA_CREATE_ALWAYS))
    {
      LOG_DEBUG("CLogoDynamite::DefaultConfigurationWrite %s file created.", LOGO_DYNAMITE_CONF_FILE_NAME);
      for(uint32_t i = 0; i < (sizeof(m_CmdList) / sizeof(m_CmdList[0])); i++)
	{
	  f_printf(&p_File, "%s=%s\r\n", m_CmdList[i].cmd, m_CmdList[i].value);
	} // for
      LOG_DEBUG("CLogoDynamite::DefaultConfigurationWrite closing file");
      
      f_close(&p_File);
    }
  else
    {
      // This is kind of critical. But we can always use the feault settings
      LOG_WARNING("CLogoDynamite::DefaultConfigurationWrite failed to create file.");
    }

  return result;
} // CLogoDynamite::DefaultConfigurationWrite

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLogoDynamite::Init(void)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
  LOG_TRACE("CLogoDynamite::Init");
   
  CLog::VerbositySet(DYNAMIC_LOG_VERBOSITY);
   
  LogomaticV2Init();
   
  BannerPrint();
   
  ConfigurationRead();
   
  ConfigurationApply();
   
  CSubsystem::Startup();
   
  vTaskStartScheduler();
} // CLogoDynamite::Init

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
