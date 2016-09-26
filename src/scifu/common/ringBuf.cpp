/*
********************************************************************************
Copyright © 2006-2007 Canadian Space Agency 

Title to all rights of ownership in the Background Information is and shall 
remain the property of the Crown.  All intellectual property rights to all 
Foreground Information shall, immediately upon its conception, development, 
reduction to practice or production, vest in and remain the property of the 
Crown.  No Background or Foreground Information shall be disclosed in whole or 
in part without the prior written permission of the Crown.
********************************************************************************

CSCI NAME:  QuickSAT Flight Software
CSA-QSAT-EM-100329

FILENAME:   $Workfile: $
            $Date: 2007/10/13 23:57:25 $
            $Revision: 1.1 $

MODIFICATION HISTORY:
--------------------------------------------------------------------------------
$Log: ringBuf.c,v $
Revision 1.1  2007/10/13 23:57:25  Something Sweet
Original release


********************************************************************************
*/

#include <string.h>
#include "ringBuf.h"

/*
********************************************************************************
*
* FUNCTION NAME: RingBufFlush
*
********************************************************************************
*/
int8_t RingBufFlush(
	sRingBuf_t * const p_Ring   /* in */
)
{
	int8_t status = -1;

	if(0 != p_Ring)
   {
   	p_Ring->rdIndex = p_Ring->wrIndex = 0;

      memset(p_Ring->p_Buffer, 0, p_Ring->size);
   } /* if(0 != p_Ring) */
   
   return status;
} /* RingBufFlush */

/*
********************************************************************************
*
* FUNCTION NAME: RingBufGet
*
********************************************************************************
*/
uint16_t RingBufGet(
   sRingBuf_t * const 	p_Ring,     /* in */
   void * const      	p_Data,     /* in */
   const uint16_t      		nBytes      /* in */
   )
{
   uint16_t   bytesRetrieved = 0;

   while( (bytesRetrieved < nBytes) &&
   		(1 == RingBufByteGet(p_Ring, &(((uint8_t*)(p_Data))[bytesRetrieved])))
   		)
   {
      bytesRetrieved++;
      //SysIo::Printf("\r\nRingBufGet=%u", bytesRetrieved);
   } /* while(...) */

   return bytesRetrieved;
} /* RingBufGet */

/*
********************************************************************************
*
* FUNCTION NAME: RingBufInit
*
********************************************************************************
*/
int8_t RingBufInit(
   sRingBuf_t * const 	p_Ring,     /* in */
   void * const      	p_Buffer,   /* in */
   const uint16_t	size        /* in */
   )
{
   int8_t status = -1;

   if(0 != p_Buffer &&
      0 != size &&
      0 != p_Ring
      )
   {
      p_Ring->p_Buffer  = reinterpret_cast<uint8_t *>(p_Buffer);
      p_Ring->size      = size;

      /* Empty the buffer */
      RingBufFlush(p_Ring);

      status = 0;
   } /* if.. */
   
   return status;
} /* RingBufInit */

/*
********************************************************************************
*
* FUNCTION NAME: RingBufNumBytesGet
*
********************************************************************************
*/
int32_t RingBufNumBytesGet(
	sRingBuf_t * const 	p_Ring     /* in, static ring buffer to be initialized */
   )
{
	int32_t status = -1;
   
	if(0 != p_Ring)
   {
   	if(p_Ring->wrIndex > p_Ring->rdIndex)
      {
      	status = (p_Ring->wrIndex - p_Ring->rdIndex);
      }
      else if(p_Ring->rdIndex > p_Ring->wrIndex)
      {
      	status = (p_Ring->size - p_Ring->rdIndex + p_Ring->wrIndex);
      }
      else
      {
      	status = 0;
      }

      //SysIo::Printf("\r\nRingBufNumBytesGet=%u", status);

   } /* if(0 != p_Ring) */

   return status;
} /* RingBufNumBytes */

/*
********************************************************************************
*
* FUNCTION NAME: RingBufPut
*
********************************************************************************
*/
uint16_t RingBufPut(
   sRingBuf_t * const	p_Ring,     /* in */
   void const * const 		p_Data,     /* in */
   const uint16_t      		nBytes      /* in */
   )
{
   uint16_t   bytesAdded  = 0;
	uint16_t	top 			= 0;

   while(	(bytesAdded < nBytes) &&
   			(1 == RingBufBytePut(p_Ring, ((uint8_t *)(p_Data))[bytesAdded], top))
   	)
   {
      bytesAdded++;
      //SysIo::Printf("\r\nRingBufPut=%u", bytesAdded);
   } /* while(...) */
   
   return bytesAdded;
} /* RingBufPut */

/*
********************************************************************************
*
* FUNCTION NAME: 	RingBufByteGet
*
********************************************************************************
*/
unsigned char RingBufByteGet(
		sRingBuf_t * const 	p_Ring, /* in, static ring buffer */
		unsigned char	*		p_Byte
		)
{
	if(p_Ring->rdIndex == p_Ring->wrIndex)
	{
		return 0;
	}
	else
	{
		*p_Byte = p_Ring->p_Buffer[p_Ring->rdIndex];
		if((p_Ring->rdIndex + 1) == p_Ring->size)
		{
			p_Ring->rdIndex = 0;
		}
		else
		{
			p_Ring->rdIndex++;
		}
		return 1;
	}
}
