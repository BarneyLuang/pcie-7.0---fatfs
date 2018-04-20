/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_H
#define __FLASH_H
#include "include.h"
/* Includes ------------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#ifdef FUNC_EXTERNAL_FLASH_EN


//#define FLASH_W25Q32_GND         					8



/* Flash cmd index */
#define	FLASH_CMD_READ_DATA							0x03
#define	FLASH_CMD_FAST_READ							0x0B
#define	FLASH_CMD_WRITE_ENABLE					0x06
#define	FLASH_CMD_WRITE_DISABLE					0x04
#define	FLASH_CMD_PAGE_PROGRAM					0x02
#define	FLASH_CMD_SECTOR_ERASE					0x20
#define	FLASH_CMD_BLOCK_ERASE_32KB			0x52
#define	FLASH_CMD_BLOCK_ERASE_64KB			0xD8
#define	FLASH_CMD_POWER_DOWN						0xB9
#define	FLASH_CMD_RELEASE_POWER_DOWN		0xAB
#define	FLASH_CMD_READ_STATUS1					0x05
#define	FLASH_CMD_READ_STATUS2					0x35
#define	FLASH_CMD_READ_STATUS3					0x15
#define	FLASH_CMD_WRITE_STATUS1					0x01
#define	FLASH_CMD_WRITE_STATUS2					0x31
#define	FLASH_CMD_WRITE_STATUS3					0x11
#define	FLASH_STATUS1_BUSY_BIT					0x01
#define	FLASH_STATUS1_WEL_BIT						0x02

#define	FLASH_DUMMY_BYTE								0xFF
#define	FLASH_PAGE_MAX_BYTES						256
																				
//#define	Flash_CS_Enable()								
//#define	Flash_CS_Disable()	
																				
																				
#define	Flash_CS_Enable()								nrf_gpio_pin_clear(FLASH_W25Q32_CS);
#define	Flash_CS_Disable()							nrf_gpio_pin_set(FLASH_W25Q32_CS);

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define FLASH_PAGE_SIZE			256
#define FLASH_SECTOR_SIZE		4096
#define FLASH_SECTOR_COUNT	(8*1024*1024/FLASH_SECTOR_SIZE)   //2048
#define FLASH_BLOCK_SIZE		65536  		//
#define FLASH_PAGES_PER_SECTOR	FLASH_SECTOR_SIZE/FLASH_PAGE_SIZE			//16
#define DISK_CMD_WRITE_BASE_OFFSET    0
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

typedef enum {	
	DISK_CMD_WRITE_STRING_NUM = 0x00,     //序列号
	DISK_CMD_WRITE_MOD ,									//监管
	DISK_CMD_WRITE_REGN,									//国家
	DISK_CMD_WRITE_RMD,										//型号
	DISK_CMD_WRITE_WIFI_MAC,							//wifi
	DISK_CMD_WRITE_BLE_MAC,								//ble
	DISK_CMD_WRITE_COLOR,									//颜色
	DISK_CMD_WRITE_NVSN,									//指纹码
	DISK_CMD_WRITE_LCM,										//液晶码
	DISK_CMD_WRITE_BATT,									//电池码
	DISK_CMD_WRITE_BCMS,									//后相机
	DISK_CMD_WRITE_FCMS,									//前相机
	DISK_CMD_WRITE_MLB,										//主板码
	DISK_CMD_WRITE_ALL_DISK		=0xff,						//擦写整个硬盘
}DISK_CMD;

typedef enum {	
	PHONE_VER_6  = 0,
	PHONE_VER_6SP  ,
	PHONE_VER_7,
	PHONE_VER_7SP,
	PHONE_VER_8,
	PHONE_VER_8SP,
	PHONE_VER_X,
}PHONE_VER;
#endif
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern FATFS fs;      //所有文件操作的变量，用完要释放
extern FIL file;			//所有文件操作的变量，用完要释放
extern uint8_t filename[16];
extern uint32_t tick_count ;
extern BYTE ReadBuffer[4096];
/* Exported functions --------------------------------------------------------*/
void Memory_Write_byte(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);
void Memory_Read_byte(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
void Flash_PowerOn(void);
void Flash_PowerOff(void);
void Flash_WriteEnable(void);
void flash_readdata(unsigned int addr, unsigned short int rx_size, unsigned char *p_arg);
void flash_writedata(unsigned int addr, unsigned short int tx_size, unsigned char *p_arg);
void Flash_SectorErase(unsigned int addr);
void Flash_BlockErase(unsigned int addr);
void ReadFlashStatueRegister(uint8_t reg,uint8_t size ,uint8_t *p_arg);
void WriteFlashStatueRegister(uint8_t reg,uint8_t size ,uint8_t *p_arg);

void InitFlashSpi(void);
void Mount_SPI_Flash_File_System(void) ;
bool UartIphoneDataTx(void);
void SPI_Flash_Erase_Sector(uint32_t Dst_Addr);   

void init_disk(void);
void format_disk(void);

void read_dir(DIR *dirs);
void write_file(FIL *pf , TCHAR *path_name,const uint8_t *data,uint32_t data_size);
void read_file( TCHAR *path_name);
void read_file_for_line( TCHAR *path_name,uint32_t line);

void delete_file(void);
void W25X_Read_Sector(uint32_t nSector, uint8_t * pBuffer);
void W25X_Write_Sector(uint32_t nSector, uint8_t* pBuffer);

/* disk operation*/
void iphone_disk_command_op(uint8_t version,DISK_CMD cmd);

/*----------------------------------------------------------------------------*/


#endif

