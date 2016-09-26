#ifndef ringBufLib_included
#define ringBufLib_included

/*
********************************************************************************
Copyright � 2006-2007 Canadian Space Agency 

Title to all rights of ownership in the Background Information is and shall 
remain the property of the Crown.  All intellectual property rights to all 
Foreground Information shall, immediately upon its conception, development, 
reduction to practice or production, vest in and remain the property of the 
Crown.  No Background or Foreground Information shall be disclosed in whole or 
in part without the prior written permission of the Crown.
********************************************************************************

CSCI NAME:  QuickSAT Flight Software
CSA-QSAT-EM-100329

FILENAME:   	$Workfile: $
            	$Date: 2007/10/13 23:57:25 $
            	$Revision: 1.1 $

DESCRIPTION:	This library provides routines for creating and using ring buffers,
               which are first-in-first-out circular buffers. The routines simply
               manipulate the ring buffer data structure; no kernel functions are
               invoked. In particular, ring buffers by themselves provide no task
               synchronization or mutual exclusion. 

               However, the ring buffer pointers are manipulated in such a way
               that a reader task (invoking RingBufGet( )) and a writer task
               (invoking RingBufPut( )) can access a ring simultaneously
               without requiring mutual exclusion. This is because readers
               only affect a read pointer and writers only affect a write
               pointer in a ring buffer data structure. However, access by
               multiple readers or writers must be interlocked through a
               mutual exclusion mechanism (i.e., a mutual-exclusion semaphore
               guarding a ring buffer). 

NOTES:			Need to figure out what to do with the nesting modes, the Upd72001
					being slaves...

MODIFICATION HISTORY:
--------------------------------------------------------------------------------
$Log: ringBuf.h,v $
Revision 1.1  2007/10/13 23:57:25  Something Sweet
Original release


********************************************************************************
*/

#include "inttypes.h"

typedef struct
{
   uint8_t  * 		p_Buffer;   /* Pointer to the start of the ring */
   unsigned int  	size;       /* size, in bytes of the ring buffer */
   unsigned int	rdIndex;    /* Where to read the next byte available */
   unsigned int  	wrIndex;    /* Where the last data was written to */
} sRingBuf_t;

/*
********************************************************************************
*
* FUNCTION NAME: 	RingBufByteGet
*
* DESCRIPTION: 	Get 1 byte from the ring buffer. This is defined as a macro so
*						it executes faster.
*
* ASSUMPTIONS: 	_p_Ring is valid.
*
* CODE REUSE:  	None
********************************************************************************
*/
unsigned char RingBufByteGet(
		sRingBuf_t * const 	p_Ring, /* in, static ring buffer */
		unsigned char	*		p_Byte
		);

/*
********************************************************************************
*
* FUNCTION NAME: RingBufBytePut
*
* DESCRIPTION: Add 1 byte to the ring buffer. Return 1 on success, 0 otherwise.
*
* ASSUMPTIONS: None
*
* CODE REUSE:  None
********************************************************************************
*/
#define RingBufBytePut(_p_Ring, _data, _top)		\
    (						\
    _top = (_p_Ring)->wrIndex,			\
    (_top == (_p_Ring)->rdIndex - 1) ?		\
	0 					\
    :						\
	(					\
    	(_top == (_p_Ring)->size - 1) ?	\
	    (					\
	    ((_p_Ring)->rdIndex == 0) ?		\
		0				\
	    :					\
		(				\
		(_p_Ring)->p_Buffer[_top] = _data,	\
		(_p_Ring)->wrIndex = 0,		\
		1				\
		)				\
	    )					\
	:					\
	    (					\
	    (_p_Ring)->p_Buffer[_top] = _data,		\
	    (_p_Ring)->wrIndex++,			\
	    1					\
	    )					\
	)					\
    )

/*
********************************************************************************
*
* FUNCTION NAME:	RingBufFlush
*
* DESCRIPTION: 	Discard any data stored in the ring buffer. Return OK on success
*						ERROR if p_Ring is not valid.
*
* ASSUMPTIONS: 	None
*
* CODE REUSE:  	None
********************************************************************************
*/
int8_t RingBufFlush(
	sRingBuf_t * const p_Ring   /* in, ring to be flushed */
);

/*
********************************************************************************
*
* FUNCTION NAME:	RingBufFreeBytesGet
*
* DESCRIPTION: 	Return the number of free bytes available in the ring buffer.
*
* ASSUMPTIONS: 	None
*
* CODE REUSE:  	None
********************************************************************************
*/
#define RingBufFreeBytesGet(_p_Ring) ((_p_Ring)->size - RingBufNumBytesGet(_p_Ring))

/*
********************************************************************************
*
* FUNCTION NAME: 	RingBufGet
*
* DESCRIPTION: 	Get bytes from the ring buffer. Return the number of bytes read,
*						ERROR if p_Ring is not valid.
*
* ASSUMPTIONS: 	None
*
* CODE REUSE:  	None
********************************************************************************
*/
uint16_t RingBufGet(
   sRingBuf_t * const	p_Ring,     /* in, static ring buffer to be initialized. */
   void * const      	p_Data,     /* in, data to be added. */
   const uint16_t      		nBytes      /* in, number of bytes to be added */
   );

/*
********************************************************************************
*
* FUNCTION NAME: RingBufInit
*
* DESCRIPTION: 	Initialize a ring buffer structure and link it to a
*              	static buffer. The actual number of bytes used is size - 1.
*
* ASSUMPTIONS: 	None
*
* CODE REUSE:  	None
********************************************************************************
*/
int8_t RingBufInit(
	sRingBuf_t * const 	p_Ring,     /* in, static ring buffer to be initialized */
   void * const      	p_Buffer,   /* in, static buffer to be linked to the ring */
                                 		/* buffer */
   const uint16_t      		size        /* in, size (in bytes) of the ring buffer */
   );

/*
********************************************************************************
*
* FUNCTION NAME: 	RingBufNumBytesGet
*
* DESCRIPTION: 	Return the number of bytes in the ring buffer, ERROR if p_Ring
*                 is not valid.
*
* ASSUMPTIONS: 	None
*
* CODE REUSE:  	None
********************************************************************************
*/
int32_t RingBufNumBytesGet(
	sRingBuf_t * const 	p_Ring     /* in, static ring buffer to be initialized */
   );

/*
********************************************************************************
*
* FUNCTION NAME: 	RingBufPut
*
* DESCRIPTION: 	Add bytes to the ring buffer. Return the number of bytes added,
*						ERROR if p_Ring is not valid.
*
* ASSUMPTIONS: 	None
*
* CODE REUSE:  	None
********************************************************************************
*/
uint16_t RingBufPut(
   sRingBuf_t * const	p_Ring,     /* in, static ring buffer to be initialized. */
   void const * const  		p_Data,     /* in, data to be added. */
   const uint16_t            nBytes      /* in, number of bytes to be added */
   );

#endif /* ringBufLib_included */


