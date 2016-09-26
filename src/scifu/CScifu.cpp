#include "CScifu.hpp"

char                                  CScifu::m_IoBuffer[SCIFU_IO_BUFFER_SIZE];
CScifuDevice *                        CScifu::mp_IoDev = NULL;
uint32_t                   CScifu::m_LogLevel = SCIFU_INFO;
bool                                  CScifu::m_StopOnFirstError  = false;
CScifuDevice *				   		   CScifu::mp_FirstDevice = NULL;
