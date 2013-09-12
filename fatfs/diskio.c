/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */

#include <stdio.h>		/* for printf */
#include <string.h>		/* for memcpy */
#include "stm32f4xx.h"
#include "stm32f4_discovery_sdio_sd.h"


extern SD_CardInfo SDCardInfo;
BYTE SDInitialized = 0;

#define SD_DISK_PDRV	0 /* Physical drive number for SD */
//#define DEBUG

#ifdef DEBUG
#include "sdio_debug.h"
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT(x) do {} while (0)
#endif

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	SD_Error	SDError;
	
	if( pdrv == SD_DISK_PDRV )
	{
		if (SDInitialized == 1)
		{
			DEBUG_PRINT(("Allredy configure SD card.\n"));
			return 0;	/* All OK */
		}
		
		DEBUG_PRINT(("Configure SD card...\n"));
		if ((SDError = SD_Init()) != SD_OK)
		{
			DEBUG_PRINT(("Configure SD card - FAILED. SD_Init()\n"));
			#ifdef DEBUG 
			SD_print_error( SDError );
			#endif
			return STA_NOINIT;
		}
		
		#ifdef DEBUG 
		if ((SDError = SD_PrintInfo()) != SD_OK)
		{
			DEBUG_PRINT(("Configure SD card - FAILED. SD_PrintInfo()\n"));
			SD_print_error( SDError );
			return STA_NOINIT;
		}
		
		if ((SDError = SD_PrintCardStatus()) != SD_OK)
		{
			DEBUG_PRINT(("Configure SD card - FAILED. SD_PrintCardStatus()\n"));
			SD_print_error( SDError );
			return STA_NOINIT;
		}
		#endif
		
		SDInitialized = 1;
		DEBUG_PRINT(("Configure SD card - OK.\n"));
		
		return 0;	/* All OK */
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
	if( pdrv == SD_DISK_PDRV )
	{
		/*
		switch (SD_GetState())
		{
		case	SD_CARD_READY:
			return RES_OK;
		case	SD_CARD_IDENTIFICATION:
		case	SD_CARD_STANDBY:
		case	SD_CARD_TRANSFER:
		case	SD_CARD_SENDING:
		case	SD_CARD_RECEIVING:
		case	SD_CARD_PROGRAMMING:
		case	SD_CARD_DISCONNECTED:
			return RES_NOTRDY;
		case	SD_CARD_ERROR:
		default:
			return RES_ERROR;
		}
		*/
		
		return SDInitialized ? RES_OK : STA_NOINIT;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/
#define BLOCK_SIZE 512

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..128) */
)
{
//	SDTransferState SD_TransferState;
	DRESULT dresult = RES_OK;
	SD_Error SDError;
	if( pdrv == SD_DISK_PDRV )
	{	
		DEBUG_PRINT(("disk_read  pdrv=%d *buff=%p sector=%d count=%d\n",pdrv,buff,sector,count));
		if(count <= 1)
		{
			DEBUG_PRINT(("SD_ReadBlock:"));
			SDError = SD_ReadBlock(buff, sector*BLOCK_SIZE, BLOCK_SIZE);
			#ifdef DEBUG 
			SD_print_error(SDError);
			#endif
			
			#ifdef SD_DMA_MODE
			DEBUG_PRINT(("SD_WaitReadOperation:"));
			SDError = SD_WaitReadOperation();
			#ifdef DEBUG 
			SD_print_error(SDError);
			#endif
			#endif
			
			while( SD_GetStatus() != SD_TRANSFER_OK );
			if(SDError != SD_OK)dresult = RES_ERROR;
		}
		else
		{
			DEBUG_PRINT(("SD_ReadMultiBlocks:"));
			SDError = SD_ReadMultiBlocksFIXED(buff, sector*BLOCK_SIZE, BLOCK_SIZE, count);
			//SDError = SD_ReadMultiBlocks(buff, sector*BLOCK_SIZE, BLOCK_SIZE, count);
			#ifdef DEBUG 
			SD_print_error(SDError);
			#endif
			
			#ifdef SD_DMA_MODE
			DEBUG_PRINT(("SD_WaitReadOperation:"));
			SDError = SD_WaitReadOperation();
			#ifdef DEBUG 
			SD_print_error(SDError);
			#endif
			#endif
			
			while( SD_GetStatus() != SD_TRANSFER_OK );
			if(SDError != SD_OK)dresult = RES_ERROR;
		}
		return dresult;
	}
	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..128) */
)
{
	DRESULT dresult = RES_OK;
	SD_Error SDError;
	if( pdrv == SD_DISK_PDRV )
	{	
		DEBUG_PRINT(("disk_write  pdrv=%d *buff=%p sector=%d count=%d\n",pdrv,buff,sector,count));
		if(count <= 1)
		{
			DEBUG_PRINT(("SD_WriteBlock:"));
			SDError = SD_WriteBlock((uint8_t*)(buff), sector*BLOCK_SIZE, BLOCK_SIZE);
			#ifdef DEBUG 
			SD_print_error(SDError);
			#endif
			
			#ifdef SD_DMA_MODE
			DEBUG_PRINT(("SD_WaitWriteOperation:"));
			SDError = SD_WaitWriteOperation();
			#ifdef DEBUG 
			SD_print_error(SDError);
			#endif
			#endif
			
			while( SD_GetStatus() != SD_TRANSFER_OK );
			if(SDError != SD_OK)dresult = RES_ERROR;
		}
		else
		{
			DEBUG_PRINT(("SD_ReadMultiBlocks:"));
			SDError = SD_WriteMultiBlocksFIXED((uint8_t*)(buff), sector*BLOCK_SIZE, BLOCK_SIZE, count);
			//SDError = SD_WriteMultiBlocks(buff, sector*BLOCK_SIZE, BLOCK_SIZE, count);
			#ifdef DEBUG 
			SD_print_error(SDError);
			#endif
			
			#ifdef SD_DMA_MODE
			DEBUG_PRINT(("SD_WaitWriteOperation:"));
			SDError = SD_WaitWriteOperation();
			#ifdef DEBUG 
			SD_print_error(SDError);
			#endif
			#endif
			
			while( SD_GetStatus() != SD_TRANSFER_OK );
			if(SDError != SD_OK)dresult = RES_ERROR;
		}
		return dresult;
	}
	return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	if( pdrv == SD_DISK_PDRV )
	{
		
	}
	return RES_PARERR;
}
#endif



DWORD get_fattime ( void )
{
	return	((2013UL-1980) << 25)	      // Year = 2006
			| (8UL << 21)	      // Month = Feb
			| (30UL << 16)	      // Day = 9
			| (13U << 11)	      // Hour = 22
			| (00U << 5)	      // Min = 30
			| (00U >> 1)	      // Sec = 0
			;
}
