#include "CLpc21xxSpiDev.hpp"
#include "CScifu.hpp"

#define LPC214x_REG_READ_32(_reg)                  (mp_BaseAddr[_reg])
#define LPC214x_REG_WRITE_32(_reg, _value)         ((mp_BaseAddr[_reg]) = _value)

const uint8_t CLpc21xxSpiDev::m_SPCR  = 0;
const uint8_t CLpc21xxSpiDev::m_SPSR  = 1;
const uint8_t CLpc21xxSpiDev::m_SPDR  = 2;
const uint8_t CLpc21xxSpiDev::m_SPCCR = 3;
const uint8_t CLpc21xxSpiDev::m_SPINT = 7;

CLpc21xxSpiDev::CLpc21xxSpiDev(
   const uint32_t baseAddress,
   const uint32_t pclock
   )
{
   mp_BaseAddr = reinterpret_cast<uint32_t *>(baseAddress);
   m_PClock   = pclock;
   m_BaudRate = 9600;
   Init();
}

CLpc21xxSpiDev::CLpc21xxSpiDev(void)
{
   mp_BaseAddr = NULL;
} // CLpc21xxSpiDev::CLpc21xxSpiDev

int8_t CLpc21xxSpiDev::Init(void)
{
   int8_t   result = -1;
   uint32_t baudRateDiv = 0;

   LPC214x_REG_WRITE_32(m_SPCR, 0x20);
   // no initialization for SPSR
   // no initialization for SPDR

   baudRateDiv = m_PClock / m_BaudRate;
   baudRateDiv &= 0xFFFFFFFE;
   if(baudRateDiv < 8)
   {
      baudRateDiv = 8;
   }
   else if (baudRateDiv > 254)
   {
      baudRateDiv = 254;
   }
   else
   {}
   m_BaudRate = m_PClock / baudRateDiv;
   
   LPC214x_REG_WRITE_32(m_SPCCR, static_cast<uint8_t>(baudRateDiv));

   result = 0;
   return result;
} // CLpc21xxSpiDev::Init

CLpc21xxSpiDev::~CLpc21xxSpiDev(void)
{} // CLpc21xxSpiDev::~CLpc21xxSpiDev

uint16_t CLpc21xxSpiDev::ByteWrite(
  uint8_t data
)
{
   uint16_t reg       = 0;
   //rprintf("\r\nCLpc21xxSpiDev::ByteWrite 0x%02X", data);

   LPC214x_REG_WRITE_32(m_SPDR, data);

   /* Wait until SPI is ready to receive more data */
   
   while(!(LPC214x_REG_READ_32(m_SPSR) & 0x80))
   {}

   /* Check the status */
   //reg = LPC_2148_SPI_DRV_REG_READ_8(LPC_2148_SPI_DRV_S0SPSR_REG);
   /* TODO: handle errors here */

   /* Read the receive register */
   //reg = LPC_2148_SPI_DRV_REG_READ_16(LPC_2148_SPI_DRV_S0SPDR_REG);
   reg = LPC214x_REG_READ_32(m_SPDR);

   //if(reg != 0xFF)
   //{
   //   reg = 0xff;
   //}
   //SysIoPrintf(" %02u", (uint8_t)reg);

   return reg;
} /* ByteWrite */

int32_t CLpc21xxSpiDev::Read(
   void * const   p_Data,
   size_t         nBytes
   )
{
   Select(0);
	//rprintf("\r\nCLpc21xxSpiDev::Read");
   for(size_t i = 0; i < nBytes; i++)
     {
        (reinterpret_cast<uint8_t *>(p_Data))[i] =ByteWrite(0xFF);
		//rprintf("\r\nCLpc21xxSpiDev::Read 0x%02X", (reinterpret_cast<uint8_t *>(p_Data))[i]);
     }
	 
   UnSelect(0);
   
   return nBytes;
} // CLpc21xxSpiDev::Read

int32_t CLpc21xxSpiDev::Write(
   void const * const   p_Data,
   size_t               nBytes
   )
{
   size_t   bytesWritten  = 0;
   uint8_t  reg           = 0;
   
   Select(0);

   while(bytesWritten < nBytes)
   {
      reg = (static_cast<uint8_t const * const>(p_Data))[bytesWritten];
      LPC214x_REG_WRITE_32(m_SPDR, reg);

      while(0x00 == (LPC214x_REG_READ_32(m_SPSR) & 0x80))
      {}

      bytesWritten++;
   } // while(bytesWritten < nBytes)
   
   UnSelect(0);

   return bytesWritten;
} // CLpc21xxSpiDev::Write

int8_t CLpc21xxSpiDev::SetBaudRate(
   const uint32_t baudRate
   )
{
   CScifu_Info("CLpc21xxSpiDev::SetBaudRate=%u", baudRate);

   m_BaudRate = baudRate;
   Init();
   return 0;
} // CLpc21xxSpiDev::SetBaudRate

uint32_t CLpc21xxSpiDev::GetBaudRate(void)
{
   CScifu_Info("CLpc21xxSpiDev::GetBaudRate");
   return m_BaudRate;
} // CLpc21xxSpiDev::GetBaudRate
