/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "include.h"
/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */

#define ATA		0
#define MMC		1
#define USB		2

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
	
DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
	int result;
//	if (drv)
//	{
//		return STA_NOINIT;
//	}
//	result = SPI_Flash_Init();
	
	
	printf("\r\n正在初始化,请稍候...");
	return 0;
	switch (drv) 
	{
		case 0:
			result = RES_OK  ;
		 return result;
	}
		return STA_NOINIT;

#if 0
	switch (drv) 
	{
	case ATA :
		result = ATA_disk_initialize();
		// translate the reslut code here

		return stat;

	case MMC :
		result = MMC_disk_initialize();
		// translate the reslut code here

		return stat;

	case USB :
		result = USB_disk_initialize();
		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
#endif
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
	return RES_OK;
#if 0
	DSTATUS stat;
	int result;

	switch (drv) 
	{
	case ATA :
		result = ATA_disk_status();
		// translate the reslut code here
		return stat;

	case MMC :
		result = MMC_disk_status();
		// translate the reslut code here
 		return stat;

	case USB :
		result = USB_disk_status();
		// translate the reslut code here
		return stat;
	}
	return STA_NOINIT;
#endif
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..512) */
)
{
	int i;
	for(i=0;i<count;i++)
	{
		W25X_Read_Sector(sector,buff);
		sector ++;
		buff += FLASH_SECTOR_SIZE;
	}
	
	return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{
	int i;
	for(i=0;i<count;i++)
	{	
		SPI_Flash_Erase_Sector(sector);
		W25X_Write_Sector(sector,(uint8_t *)buff);
		printf("\r\nerase %d",sector);
		sector ++;
		buff += FLASH_SECTOR_SIZE;
	}
	return RES_OK;
}
#endif /* _READONLY */

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

/************************************************************************************
CTRL_SYNC			Make sure that the disk drive has finished pending write process. 
					When the disk I/O module has a write back cache, flush the dirty sector immediately. 
					This command is not used in read-only configuration. 
GET_SECTOR_SIZE		Returns sector size of the drive into the WORD variable pointed by Buffer. 
					This command is not used in fixed sector size configuration, _MAX_SS is 512. 
GET_SECTOR_COUNT	Returns number of available sectors on the drive into the DWORD variable pointed by Buffer.
					This command is used by only f_mkfs function to determine the volume size to be created. 
GET_BLOCK_SIZE		Returns erase block size of the flash memory in unit of sector into the DWORD variable pointed by Buffer.
					The allowable value is 1 to 32768 in power of 2. 
					Return 1 if the erase block size is unknown or disk devices. 
					This command is used by only f_mkfs function and it attempts to align data area to the erase block boundary. 
CTRL_ERASE_SECTOR	Erases a part of the flash memory specified by a DWORD array {<start sector>, <end sector>} pointed by Buffer. 
					When this feature is not supported or not a flash memory media, this command has no effect.
					The FatFs does not check the result code and the file function is not affected even if the sectors are not erased well. 
					This command is called on removing a cluster chain when _USE_ERASE is 1. 
************************************************************************************/
DRESULT disk_ioctl(
	BYTE drv,		/* Physical drive nmuber(0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_OK;
	DWORD nFrom,nTo;
	int i;
	
	switch(ctrl)
	{
		case CTRL_SYNC :
			break;
		
		//扇区擦除
		case CTRL_ERASE_SECTOR:
			nFrom = *((DWORD*)buff);
			nTo = *(((DWORD*)buff)+1);
			for(i = nFrom;i <= nTo;i ++)
				SPI_Flash_Erase_Sector(i);	
			break;
		case GET_BLOCK_SIZE:
			*(DWORD*)buff = FLASH_BLOCK_SIZE;
			break;
		
		case GET_SECTOR_SIZE:
			*(DWORD*)buff = FLASH_SECTOR_SIZE;
			break;
		
		case GET_SECTOR_COUNT:
			*(DWORD*)buff = FLASH_SECTOR_COUNT;
			break;
			
		default:
			res = RES_PARERR;
			break;
	}
	return res;
}

DWORD get_fattime(void)
{
/*
 t = Time_GetCalendarTime();
    t.tm_year -= 1980;  //年份改为1980年起
    t.tm_mon++;          //0-11月改为1-12月
    t.tm_sec /= 2;       //将秒数改为0-29
    
    date = 0;
    date = (t.tm_year << 25) | (t.tm_mon<<21) | (t.tm_mday<<16)|\
            (t.tm_hour<<11) | (t.tm_min<<5) | (t.tm_sec);

    return date;
*/
	return 0;
}


