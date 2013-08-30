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
#include "sdio_debug.h"

extern SD_CardInfo SDCardInfo;
BYTE SDInitialized = 0;

#define SD_DISK_PDRV	0 /* Physical drive number for SD */

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	SD_Error			SDError;

	if( pdrv == SD_DISK_PDRV )
	{
		if (SDInitialized == 1)
		{
			printf("Allredy configure SD card.\n");
			return 0;	/* All OK */
		}
		
		printf("Configure SD card...\n");
		if ((SDError = SD_Init()) != SD_OK)
		{
			printf("Configure SD card - FAILED. SD_Init()\n");
			SD_print_error( SDError );
			return STA_NOINIT;
		}
		
		if ((SDError = SD_PrintInfo()) != SD_OK)
		{
			printf("Configure SD card - FAILED. SD_PrintInfo()\n");
			SD_print_error( SDError );
			return STA_NOINIT;
		}
		
		if ((SDError = SD_PrintCardStatus()) != SD_OK)
		{
			printf("Configure SD card - FAILED. SD_PrintCardStatus()\n");
			SD_print_error( SDError );
			return STA_NOINIT;
		}
		
		SDInitialized = 1;
		printf("Configure SD card - OK.\n");
		
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
	  SDTransferState state;
//  DRESULT res = RES_OK;
  


//  STM_EVAL_LEDOn ( LED_ORANGE );

  //SD_ReadMultiBlocksFIXED ( buff, sector, 512, 1 );
  SD_ReadBlock(buff, sector, 512);
 // SD_WaitReadOperation ( );
  
//   while ( 1 )
//   {
//     state = SD_GetStatus ( );
//     if ( state == SD_TRANSFER_OK )
//       break;
//   } // while

//  STM_EVAL_LEDOff ( LED_ORANGE );

  return RES_OK;
	
	
// SD_Error Status;
//  
//     printf("disk_read %d %p %10d %d\n",pdrv,buff,sector,count);
//  
//     if (SD_Detect() != SD_PRESENT)
//         return(RES_NOTRDY);
//  
//     if ((DWORD)buff & 3) // DMA Alignment failure, do single up to aligned buffer
//     {
//         DRESULT res = RES_OK;
//         DWORD scratch[BLOCK_SIZE / 4]; // Alignment assured, you'll need a sufficiently big stack
//          
//         while(count--)
//         {
//             res = disk_read(pdrv, (void *)scratch, sector++, 1);
//  
//             if (res != RES_OK)
// 			{
//                 break;
// 			}
//             memcpy(buff, scratch, BLOCK_SIZE);
//  
//             buff += BLOCK_SIZE;
//         }
//          
//         return(res);
//     }
//      
//   Status = SD_ReadMultiBlocksFIXED(buff, sector, BLOCK_SIZE, count); // 4GB Compliant
//  
//     if (Status == SD_OK)
//     {
//         SDTransferState State;
//          
//         Status = SD_WaitReadOperation(); // Check if the Transfer is finished
//  		SD_print_error(Status);
// 		
//         while((State = SD_GetStatus()) == SD_TRANSFER_BUSY); // BUSY, OK (DONE), ERROR (FAIL)
//          
//         if (State == SD_TRANSFER_ERROR)
//             return(RES_ERROR);
//         else
// 		{
// 			SD_print_error(Status);
//             return(RES_OK);
// 		}
//     }
//     else
//         return(RES_ERROR);

// 	SD_Error SDError;
// 	printf("disk_read %d %p %10d %d\n",pdrv,buff,sector,count);
// 	SDError = SD_ReadMultiBlocksFIXED ( buff, sector, 512, count );
// 	SD_print_error(SDError);
// 	return RES_OK;

	
	
// 	uint16_t	Transfer_Length;
// 	uint32_t	Memory_Offset;
// 	SD_Error	SDStatus;
// 	
// 	if( pdrv == SD_DISK_PDRV )
// 	{
//         Transfer_Length = count;// * 512;
//         Memory_Offset = sector;//sector * 512;

// 		printf("disk_read %d %p %10d %d\n",pdrv,buff,sector,count);
// 		
//         if (count == 1)
//             SDStatus = SD_ReadBlock(buff, Memory_Offset, Transfer_Length);
//         else
//             SDStatus = SD_ReadMultiBlocks(buff, Memory_Offset, 512, count);

//         if (SDStatus != SD_OK)
//             return RES_ERROR;

// #ifdef	SD_DMA_MODE
//         SDStatus = SD_WaitReadOperation();
//         if (SDStatus != SD_OK)
//             return RES_ERROR;

//         while (SD_GetStatus() != SD_TRANSFER_OK)
//             ;
// #endif
//         return RES_OK;

// 	}
// 	return RES_PARERR;
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
	if( pdrv == SD_DISK_PDRV )
	{
		
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
