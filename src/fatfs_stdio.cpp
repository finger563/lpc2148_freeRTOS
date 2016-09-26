#include <string.h>
//#include <dirent.h>
#include "CScifu.hpp"
#include "inttypes.h"
#include "diskio.h"
#include "ff.h"
#include "sdcard.hpp"


FATFS     fatFs[_DRIVES];

int32_t FatFSInit(void)
{
   int32_t result = -1;

   CScifu_Info("FatFSInit");

   //if(FR_OK == f_mount(1, &fatFs[0]))
   if(FR_OK == f_mount(0, &fatFs[0]))
   {
      result = 0;
   }
   else
   {}

   return result;
} // EfslInit

extern "C" DRESULT USB_disk_read (
	BYTE *buff,			/* Pointer to the data buffer to store read data */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..127) */
);

DRESULT USB_disk_read (
	BYTE *buff,			/* Pointer to the data buffer to store read data */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..127) */
)
{
   buff = 0;
	sector = 0;
	count = 0;
   return (DRESULT)(0);
}

extern "C" DRESULT ATA_disk_read (
	BYTE *buff,			/* Pointer to the data buffer to store read data */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..127) */
);

DRESULT ATA_disk_read (
	BYTE *buff,			/* Pointer to the data buffer to store read data */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..127) */
)
{
   buff = 0;
	sector = 0;
	count = 0;
   return (DRESULT)(0);
}

extern "C" DRESULT MMC_disk_read (
	BYTE *buff,			/* Pointer to the data buffer to store read data */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..127) */
);

DRESULT MMC_disk_read (
	BYTE *buff,			/* Pointer to the data buffer to store read data */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..127) */
)
{
   //rprintf("\r\nMMC_disk_read sector=%u count=%u", sector, count);
   SdCardRead(sector, buff, count *512);
   return RES_OK;
}

extern "C" DRESULT ATA_disk_ioctl (
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive data block */
);
DRESULT ATA_disk_ioctl (
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive data block */
)
{
   ctrl = 0;
	buff = 0;
   return (DRESULT)(0);
}

extern "C" DRESULT USB_disk_ioctl (
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive data block */
);
DRESULT USB_disk_ioctl (
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive data block */
)
{
   ctrl = 0;
	buff = 0;
   return (DRESULT)(0);
}

extern "C" DRESULT MMC_disk_ioctl (
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive data block */
)
{
   DRESULT res = RES_ERROR;

   //rprintf("\r\nMMC_disk_ioctl");

   switch (ctrl)
   {
      case CTRL_SYNC :
      {
         res = RES_OK;
         break;
      }
      case GET_SECTOR_SIZE :	/* Get sectors on the disk (WORD) */
      {
         *(WORD*)buff = 512;
	 res = RES_OK;
         break;
      }
      default:
      {
         res = RES_PARERR;
		 //rprintf("\r\nMMC_disk_ioctl");
      }
   }

   return res;
}

extern "C" DRESULT ATA_disk_write (
	const BYTE *buff,	/* Pointer to the data to be written */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..127) */
);

DRESULT ATA_disk_write (
	const BYTE *buff,	/* Pointer to the data to be written */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..127) */
)
{
   buff = 0;
	sector = 0;
	count = 0;
   return (DRESULT)(0);
}

extern "C" DRESULT USB_disk_write (
	const BYTE *buff,	/* Pointer to the data to be written */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..127) */
);

DRESULT USB_disk_write (
	const BYTE *buff,	/* Pointer to the data to be written */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..127) */
)
{
   buff = 0;
	sector = 0;
	count = 0;
   return (DRESULT)(0);
}
  
extern "C" DRESULT MMC_disk_write (
	const BYTE *buff,	/* Pointer to the data to be written */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..127) */
)
{
   //rprintf("\r\nMMC_disk_write sectors %u to %u", sector, sector + count);
   SdCardWrite(sector, buff, 512*count);
   return RES_OK;
}

extern "C" DSTATUS USB_disk_status (void);
DSTATUS USB_disk_status (void)
{
   //rprintf("\r\nUSB_disk_status");
   return 0;
}

extern "C" DSTATUS ATA_disk_status (void);
DSTATUS ATA_disk_status (void)
{
   //rprintf("\r\nUSB_disk_status");
   return 0;
}

extern "C" DSTATUS MMC_disk_status (void)
{
   return 0;
}

extern "C" DSTATUS MMC_disk_initialize (void)
{
   //rprintf("\r\nMMC_disk_initialize");
   return (DSTATUS)(0);
}

extern "C" DSTATUS USB_disk_initialize (void);
DSTATUS USB_disk_initialize (void)
{
   //rprintf("\r\nUSB_disk_initialize");
   return (DSTATUS)(0);
}

extern "C" DSTATUS ATA_disk_initialize (void);
DSTATUS ATA_disk_initialize (void)
{
   //rprintf("\r\nATA_disk_initialize");
   return (DSTATUS)(0);
}

DWORD get_fattime (void)
{
 
   return (DWORD)(0);
}
