/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "include.h"
/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

#define DEV_FLASH 0
/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	
	return RES_OK;
	switch (pdrv) {
	case DEV_RAM :
		//result = RAM_disk_status();
		result = RES_OK;
		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;
	
	Flash_SectorErase(0);
	printf("硬盘初始化\r\n");
	return RES_OK;
	switch (pdrv) {
	case DEV_RAM :
		//result = RAM_disk_initialize();
		result = RES_OK;
		// translate the reslut code here

		return stat;

	case DEV_MMC :
		//result = MMC_disk_initialize();
		result = RES_OK;
		// translate the reslut code here

		return stat;

	case DEV_USB :
		//result = USB_disk_initialize();
		result = RES_OK;
		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	int result;

	
	flash_readdata(sector*4096,4096*count,buff);
	
	return RES_OK;
	
	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

		//result = RAM_disk_read(buff, sector, count);
		result = RES_OK;
		// translate the reslut code here

		return res;

	case DEV_MMC :
		// translate the arguments here

		//result = MMC_disk_read(buff, sector, count);
		result = RES_OK;
		// translate the reslut code here

		return res;

	case DEV_USB :
		// translate the arguments here
		result = RES_OK;
		//result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;

	uint32_t addr = sector*4096;
	uint32_t size = 4096*count;
	uint32_t setoraddr = addr;
	
	printf("文件系统写硬盘 count :%d\r\n",count);
	for(int i = 0;i<count;i++)
	{
		Flash_SectorErase(setoraddr);
		setoraddr += 4096;     //根据扇区计算扇区的起始地址
		printf("i : %d ,r\n",i);
	}
	printf("擦除结束\r\n");
	flash_writedata(addr,size,(uint8_t *)buff);
	
	return RES_OK;
	
	
	
	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here
		result = RES_OK;
		//result = RAM_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_MMC :
		// translate the arguments here
		result = RES_OK;
		//result = MMC_disk_write(buff, sector, count);
		// translate the reslut code here
		return res;

	case DEV_USB :
		// translate the arguments here
		result = RES_OK;
		//result = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	DWORD nFrom,nTo;
	int result;
	

	 if (pdrv == DEV_FLASH)  
    {  
        switch (cmd)   
        {  
         case CTRL_SYNC:  
             return RES_OK;  
  
         /* ???? 1024*1024*1024 =4 (MB) */  
						
				//扇区擦除
//				case CTRL_ERASE_SECTOR:
//					nFrom = *((DWORD*)buff);
//					nTo = *(((DWORD*)buff)+1);
//					for(i = nFrom;i <= nTo;i ++)
//						Flash_SectorErase(i);	
//						return RES_OK;  
//				
         case GET_SECTOR_COUNT:  
  
             *(WORD * )buff = FLASH_SECTOR_COUNT;//W25Q32 ?1024????4k bytes ???  
             return RES_OK;  
  
         /* ???? */  
  
         case GET_SECTOR_SIZE :  
  
             *(WORD * )buff = FLASH_SECTOR_SIZE;
             return RES_OK;  
  
         /*??? */  
  
         case GET_BLOCK_SIZE :  
             *(WORD * )buff = FLASH_BLOCK_SIZE;  
             return RES_OK;  
               
         default:  
             return RES_PARERR;  
         }  
    }  
    else  
    {  
        printf("!!!disk_ioctl ERR\r\n");  
        return RES_PARERR;  
    }  

	return RES_PARERR;
}

