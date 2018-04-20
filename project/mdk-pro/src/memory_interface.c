/* Includes ------------------------------------------------------------------*/
#include "include.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* SPI handler declaration */

#define SDC_SCK_PIN     FLASH_W25Q32_SCK  ///< SDC serial clock (SCK) pin.
#define SDC_MOSI_PIN    FLASH_W25Q32_SI  ///< SDC serial data in (DI) pin.
#define SDC_MISO_PIN    FLASH_W25Q32_SO  ///< SDC serial data out (DO) pin.
#define SDC_CS_PIN      FLASH_W25Q32_CS  ///< SDC chip select (CS) pin.

/**
 * @brief  SDC block device definition
 * */

BYTE ReadBuffer[4096];        /* 读缓冲区 */


FATFS fs;      //所有文件操作的变量，用完要释放
FIL file;			//所有文件操作的变量，用完要释放
uint8_t filename[16] ="demo.txt";

uint32_t tick_count ;

static volatile bool spi_xfer_done;  /**< Flag used to indicate that SPI instance completed the transfer. */

#define SPI_INSTANCE  0 /**< SPI instance index. */
static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);  /**< SPI instance. */


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void flash_readdata(unsigned int addr, unsigned short int rx_size, unsigned char *p_arg);
void flash_writedata(unsigned int addr, unsigned short int rx_size, unsigned char *p_arg);
void Flash_SectorErase(unsigned int addr);
void Flash_PowerOff(void);



#ifdef FUNC_EXTERNAL_FLASH_EN

/**
 * @brief SPI user event handler.
 * @param event
 */
void spi_event_handler(nrf_drv_spi_evt_t const * p_event)
{
    spi_xfer_done = true;
//    if (m_rx_buf[0] != 0)
//    {
//      //  NRF_LOG_INFO(" Received: \r\n");
//			//	printf("spi recive:%d,%d,%d,%d \r\n",m_rx_buf[0],m_rx_buf[1],m_rx_buf[2],m_rx_buf[3]);
//        NRF_LOG_HEXDUMP_INFO(m_rx_buf, strlen((const char *)m_rx_buf));
//    }
}


/*******************************************************************************
*Function			: Flash_WriteEnable
*Descriptor		: 
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void Flash_WriteEnable(void)
{
//	Flash_CS_Enable();
//	SPI_TxRx_OneByte(FLASH_CMD_WRITE_ENABLE);
//	Flash_CS_Disable();
	uint8_t mData = FLASH_CMD_WRITE_ENABLE;
	
	Flash_CS_Enable();
	nrf_drv_spi_transfer(&spi,&mData, 1, NULL, 0);
	Flash_CS_Disable();
}

/*******************************************************************************
*Function			: Flash_WaitTaskFinished
*Descriptor		: 
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void Flash_WaitTaskFinished(void)
{
	uint8_t reg_status;
	
//	Flash_CS_Enable();
//	SPI_TxRx_OneByte(FLASH_CMD_READ_STATUS1);
//	do
//	{
//		reg_status = SPI_TxRx_OneByte(FLASH_DUMMY_BYTE);
//	}while(reg_status & FLASH_STATUS1_BUSY_BIT);
//	Flash_CS_Disable();
	uint8_t mData[4] = {FLASH_CMD_READ_STATUS1,0xff};
	
	Flash_CS_Enable();
	nrf_drv_spi_transfer(&spi,mData, 1,NULL, 0);
	while(reg_status & FLASH_STATUS1_BUSY_BIT)
	{
		nrf_drv_spi_transfer(&spi,NULL,0, &reg_status, 1);
	}
	Flash_CS_Disable();
}

/*******************************************************************************
*Function			: Flash_SectorErase
*Descriptor		: 
*Input				: addr
*Return				: none
*Hist					:
*******************************************************************************/
void Flash_SectorErase(unsigned int addr)
{
	uint8_t m_Data[4];
	addr &= 0x7FF000;						// 4kb per sector (max second is 2048)
/*----------------------------------------------------------------------------*/
//	Flash_CS_Enable();
//	SPI_TxRx_OneByte(FLASH_CMD_SECTOR_ERASE);
//	SPI_TxRx_OneByte((unsigned char)(addr >> 16));	// a16 ~ a23
//	SPI_TxRx_OneByte((unsigned char)(addr >> 8));	// a08 ~ a15
//	SPI_TxRx_OneByte((unsigned char)(addr));				// a00 ~ a07
//	Flash_CS_Disable();
/*----------------------------------------------------------------------------*/
	Flash_WaitTaskFinished();   
	Flash_WriteEnable();
	m_Data[0] = FLASH_CMD_SECTOR_ERASE;
	m_Data[1] = (unsigned char)(addr >> 16);
	m_Data[2] = (unsigned char)(addr >> 8);
	m_Data[3] = (unsigned char)(addr) ;
	Flash_CS_Enable();
	nrf_drv_spi_transfer(&spi,(uint8_t *)m_Data, 4, NULL, 0);
	Flash_CS_Disable();
	Flash_WaitTaskFinished();
	nrf_delay_ms(200);
}

/*******************************************************************************
*Function			: Flash_BlockErase
*Descriptor		: 
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void Flash_BlockErase(unsigned int addr)		// 64KB mode
{
	Flash_WriteEnable();
/*----------------------------------------------------------------------------*/
	addr &= 0x7F0000;						// 64kb per block (max block is 7F)
//	Flash_CS_Enable();
//	SPI_TxRx_OneByte(FLASH_CMD_BLOCK_ERASE_64KB);
//	SPI_TxRx_OneByte((unsigned char)(addr >> 16));	// a16 ~ a23
//	SPI_TxRx_OneByte((unsigned char)(addr >> 8));	// a08 ~ a15
//	SPI_TxRx_OneByte((unsigned char)(addr));				// a00 ~ a07
//	Flash_CS_Disable();
/*----------------------------------------------------------------------------*/
	uint8_t m_Data[4];
	m_Data[0] = FLASH_CMD_BLOCK_ERASE_64KB;
	m_Data[1] = (unsigned char)(addr >> 16);
	m_Data[2] = (unsigned char)(addr >> 8);
	m_Data[3] = (unsigned char)(addr) ;
	
	Flash_CS_Enable();
	nrf_drv_spi_transfer(&spi,(uint8_t *)m_Data, 4, NULL, 0);
	Flash_CS_Disable();
	Flash_WaitTaskFinished();
	nrf_delay_ms(200);
}

/*******************************************************************************
*Function			: Flash_PowerOn
*Descriptor		: 
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void Flash_PowerOn(void)
{
	nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
//	spi_config.ss_pin   = FLASH_W25Q32_CS;
	spi_config.miso_pin = FLASH_W25Q32_SO;
	spi_config.mosi_pin = FLASH_W25Q32_SI;
	spi_config.sck_pin  = FLASH_W25Q32_SCK;
	
//	spi_config.frequency = NRF_DRV_SPI_FREQ_8M;
	
	APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config,spi_event_handler));
	nrf_gpio_cfg_output(FLASH_W25Q32_CS);
	nrf_gpio_cfg_output(FLASH_W25Q32_WP);
	nrf_gpio_cfg_output(FLASH_W25Q32_HOLD);
	nrf_gpio_cfg_output(FLASH_W25Q32_POWER);

	nrf_gpio_pin_set(FLASH_W25Q32_POWER);
	nrf_gpio_pin_set(FLASH_W25Q32_WP);
	nrf_gpio_pin_set(FLASH_W25Q32_HOLD);
	
	uint8_t m_Data[4];
	m_Data[0] = FLASH_CMD_RELEASE_POWER_DOWN;
	Flash_CS_Enable();
	nrf_drv_spi_transfer(&spi,(uint8_t *)m_Data, 1, NULL, 0);
	Flash_CS_Disable();
	
	nrf_delay_ms(500);
}

/*******************************************************************************
*Function			: Flash_PowerOff
*Descriptor		: 
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void Flash_PowerOff(void)
{
	uint8_t m_Data[4];
	m_Data[0] = FLASH_CMD_POWER_DOWN;
	Flash_CS_Enable();
	nrf_drv_spi_transfer(&spi,(uint8_t *)m_Data, 1, NULL, 0);
	Flash_CS_Disable();
	
	nrf_gpio_pin_clear(FLASH_W25Q32_POWER);
	nrf_gpio_pin_clear(FLASH_W25Q32_WP);
	nrf_gpio_pin_clear(FLASH_W25Q32_HOLD);
}

/*******************************************************************************
*Function			:
*Descriptor		: 
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void flash_readdata(unsigned int addr, unsigned short int rx_size, unsigned char *p_arg)
{
	uint8_t m_Data[4];
	m_Data[0] = FLASH_CMD_READ_DATA;
	m_Data[1] = (unsigned char)(addr >> 16);
	m_Data[2] = (unsigned char)(addr >> 8);
	m_Data[3] = (unsigned char)(addr) ;
	
	
	Flash_CS_Enable();
	nrf_drv_spi_transfer(&spi,(uint8_t *)m_Data, 4, NULL,0 );
//	nrf_drv_spi_transfer(&spi,NULL, 0, p_arg,rx_size );
	while(rx_size)
	{
	  nrf_drv_spi_transfer(&spi,NULL, 0, p_arg++,1 );
		rx_size--;
	}
	Flash_CS_Disable();
}


/*******************************************************************************
*Function			:
*Descriptor		: 
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void WriteFlashStatueRegister(uint8_t reg,uint8_t size ,uint8_t *p_arg)
{
	Flash_CS_Enable();
	nrf_drv_spi_transfer(&spi,&reg,1, NULL,0);
	nrf_drv_spi_transfer(&spi,p_arg,size, NULL,0);
	Flash_CS_Disable();
}
/*******************************************************************************
*Function			:
*Descriptor		: 
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void ReadFlashStatueRegister(uint8_t reg,uint8_t size ,uint8_t *p_arg)
{
	Flash_CS_Enable();
	nrf_drv_spi_transfer(&spi,&reg,1, NULL,0);
	nrf_drv_spi_transfer(&spi,NULL,0, p_arg,size );
	Flash_CS_Disable();
}


/*******************************************************************************
*Function			:
*Descriptor		: 
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void flash_writedata(unsigned int addr, unsigned short int tx_size, unsigned char *p_arg)
{

	uint8_t m_Data[4];
	m_Data[0] = FLASH_CMD_PAGE_PROGRAM;
	m_Data[1] = (unsigned char)(addr >> 16);
	m_Data[2] = (unsigned char)(addr >> 8);
	m_Data[3] = (unsigned char)(addr) ;
	
	Flash_WaitTaskFinished();
	Flash_WriteEnable();
	Flash_CS_Enable();
	nrf_drv_spi_transfer(&spi,(uint8_t *)m_Data, 4 , NULL,0 );
	while(true)
	{
		nrf_drv_spi_transfer(&spi,p_arg++, 1, NULL,0 );
		if((--tx_size) == 0)
			break;
		addr++;
		if((addr & 0xff) == 0)				// next page
		{
			Flash_CS_Disable();
			Flash_WaitTaskFinished();
			Flash_WriteEnable();
			Flash_CS_Enable();
			m_Data[0] = FLASH_CMD_PAGE_PROGRAM;
			m_Data[1] = (unsigned char)(addr >> 16);
			m_Data[2] = (unsigned char)(addr >> 8);
			m_Data[3] = (unsigned char)(addr) ;
			nrf_drv_spi_transfer(&spi,(uint8_t *)m_Data, 4, NULL,0 );
		}
	}
	Flash_CS_Disable();
/*----------------------------------------------------------------------------*/
	Flash_WaitTaskFinished();
	nrf_delay_ms(200);
}


//擦除一个扇区
//Dst_Addr:扇区地址 0~511 for w25x16
//擦除一个山区的最少时间:150ms
void SPI_Flash_Erase_Sector(uint32_t Dst_Addr)   
{   
	Dst_Addr*=FLASH_SECTOR_SIZE;

	Flash_SectorErase(Dst_Addr);
}  

//Sector Read
void W25X_Read_Sector(uint32_t nSector, uint8_t * pBuffer)
{	
//扇区号转为地址

//	NRF_LOG_INFO("W25X_Read_Sector  \r\n");
	uint32_t addr;
	uint32_t offset =128;
	nSector *=FLASH_SECTOR_SIZE ;
	for(int j=0;j<FLASH_SECTOR_SIZE/offset;j++)
	{
		addr = nSector + j*offset;
		flash_readdata(addr, offset,pBuffer+j*offset);	
	}
//	NRF_LOG_HEXDUMP_INFO(pBuffer,FLASH_SECTOR_SIZE);
	
}

//Sector Write
void W25X_Write_Sector(uint32_t nSector, uint8_t* pBuffer)
{	
	//扇区号转为地址
	uint32_t offset =128;
	uint32_t addr;
	//先擦除
	SPI_Flash_Erase_Sector( nSector);  

	nSector *= FLASH_SECTOR_SIZE;
	for(int j=0;j<FLASH_SECTOR_SIZE/offset;j++)
	{
		addr = nSector + j*offset;
		flash_writedata(addr, offset,(uint8_t*)pBuffer+j*offset);	
	}
	
}

#endif


bool UartIphoneDataTx(void)
{
	static int i=0;
	static bool flagTxIphoneCmd =false;
	if(!flagTxIphoneCmd)
	{
		flagTxIphoneCmd = true;
		i = 0;
	}
	if(i<sizeof(_accmdiphonesp))
	{
		if(NRF_SUCCESS==app_uart_put(_accmdiphonesp[i]))
		i++;
	}
	else 
	{
		flagTxIphoneCmd =false;
		return true;
	}
	return false;
}


/*******************************************************************************
*Function			: iphone_disk_command_op(uint8_t version,DISK_CMD cmd)
*Descriptor		: 
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void iphone_disk_command_op(uint8_t version,DISK_CMD cmd)
{		

	uint8_t cfgfilname[20];
	
	switch(version)
	{
		case PHONE_VER_6:			memcpy(cfgfilname,filename,sizeof(filename));break;
		case PHONE_VER_6SP: 	memcpy(cfgfilname,filename,sizeof(filename));break;
		case PHONE_VER_7:   	memcpy(cfgfilname,filename,sizeof(filename));break;
		case PHONE_VER_7SP:   memcpy(cfgfilname,filename,sizeof(filename));break;
		case PHONE_VER_8:   	memcpy(cfgfilname,filename,sizeof(filename));break;
		default:break;
	}
	memset(ReadBuffer,0,sizeof(ReadBuffer));
	
	if(cmd == DISK_CMD_WRITE_ALL_DISK)
	{
		read_file((TCHAR *)cfgfilname);
	}
	else
	{
		read_file_for_line((TCHAR *)cfgfilname,cmd);
	}
	
}


/*******************************************************************************
*Function			:read_file_for_line( TCHAR *path_name,uint32_t line)
*Descriptor		: 
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void read_file_for_line( TCHAR *path_name,uint32_t line)
{
	uint32_t resize = 4096;
	FRESULT ff_result;
	TCHAR *char_result;
	uint8_t index=0;
	FIL *pf ;
	pf =  &file;	
	
	ff_result = f_open(pf,path_name,FA_READ | FA_WRITE );
	memset(ReadBuffer,0,sizeof(ReadBuffer));
	
	while(1)
	{
		char_result= f_gets ((TCHAR *)ReadBuffer,	resize,		pf	);	
//		NRF_LOG_RAW_HEXDUMP_INFO(ReadBuffer,strlen(ReadBuffer));	
		index++;
		if(index==line)
		{
			uart_tx_data(ReadBuffer,strlen(ReadBuffer));	
		}
		if(char_result==0)  
		{
			f_close(pf);
			break;
		}		
	}
}


/*******************************************************************************
*Function			:read_file_data_for_line( TCHAR *path_name,uint32_t line)
*Descriptor		: 
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void read_file_data_for_line( TCHAR *path_name,uint32_t line,uint8_t *data)
{
	uint32_t resize = 4096;
	FRESULT ff_result;
	TCHAR *char_result;
	uint8_t index=0;
	FIL *pf ;
	pf =  &file;	
	
	ff_result = f_open(pf,path_name,FA_READ | FA_WRITE );
	memset(ReadBuffer,0,sizeof(ReadBuffer));
	
	while(1)
	{
		char_result= f_gets ((TCHAR *)ReadBuffer,	resize,		pf	);	
//		NRF_LOG_RAW_HEXDUMP_INFO(ReadBuffer,strlen(ReadBuffer));	
		index++;
		if(index==line)
		{
			data = ReadBuffer;
			//uart_tx_data(ReadBuffer,strlen(ReadBuffer));	
			return ;
		}
		if(char_result==0)  
		{
			f_close(pf);
			break;
		}		
	}
}

/*******************************************************************************
*Function			: read_file( TCHAR *path_name)
*Descriptor		: 
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void read_file( TCHAR *path_name)
{
	uint32_t resize = 4096;
	uint32_t fnum;
	FRESULT ff_result;
	TCHAR *char_result;
	FIL *pf ;
	pf =  &file;
	 //一定要定义成静态变量
	
	NRF_LOG_INFO("！！打开文件   :\r\n");	
	NRF_LOG_HEXDUMP_INFO(path_name,10);
	
	tick_count = 0;
	
	
	ff_result = f_open(pf,path_name,FA_READ | FA_WRITE );
	if(ff_result==FR_OK)
	{
		NRF_LOG_INFO("！！打开文件成功(%d)\n",ff_result);	
	}
	else 
	{
		NRF_LOG_INFO("！ 打开文件失败：(%d)\n",ff_result);
	}

	while(1)
	{
		ff_result = f_read(pf, ReadBuffer, resize, &fnum);		
		
		if(ff_result==FR_OK)
		{
#if (NRF_LOG_ENABLED==0)			
			uart_tx_data(ReadBuffer,fnum);	
#endif 			
			NRF_LOG_RAW_INFO("\r\n》已经读取数据长度 fnum:%d  读取得的文件数据为： \r\n ",fnum);	
//			NRF_LOG_HEXDUMP_INFO(ReadBuffer,fnum);	
			if(fnum<=0)
			{
					f_close(pf);
					break;
			}
		}
		else
		{
			NRF_LOG_INFO("！！文件读取失败：(%d)\n",ff_result);
			break;
		}						
	}

	NRF_LOG_INFO("读取文件用时  tick_count: %d0ms\r\n",tick_count);
	
//	ff_result = f_open(pf,path_name,FA_READ | FA_WRITE );
//	memset(ReadBuffer,0,sizeof(ReadBuffer));
//	uint8_t index=0;
//	while(1)
//	{
//		 char_result= f_gets (
//			(TCHAR *)ReadBuffer,	/* Pointer to the string buffer to read */
//			resize,		/* Size of string buffer (characters) */
//			pf		/* Pointer to the file object */
//			);	
////		NRF_LOG_RAW_HEXDUMP_INFO(ReadBuffer,strlen(ReadBuffer));	
//		index++;
////		if(index==3)
//		{
//			uart_tx_data(ReadBuffer,strlen(ReadBuffer));	
//		}
//		if(char_result==0)  
//		{
//			f_close(pf);
//			break;
//		}		
//	}
}


void write_file(FIL *pf , TCHAR *path_name,const uint8_t *data,uint32_t data_size)
{
	uint32_t index;
	FRESULT ff_result;
	
	ff_result = f_open(pf,(TCHAR *)path_name,FA_READ | FA_WRITE | FA_CREATE_NEW );  
	if(ff_result == FR_EXIST )
	{
		ff_result = f_open(pf,(TCHAR *)path_name,FA_READ | FA_WRITE );  //| FA_CREATE_NEW
	}
	if (ff_result == FR_OK )
	{
		NRF_LOG_INFO("打开文件成功!\r\n");
		ff_result=f_write(pf,data,data_size,&index);  //sizeof (_accmdiphonesp)
		if(ff_result==FR_OK)
		{
			NRF_LOG_RAW_INFO("向文件写入的数据为：\n%s\n",(uint32_t)_accmdiphonesp);
			NRF_LOG_RAW_INFO("已经写入数据长度 index:%d \r\n",index);
		}
		else
		{
			NRF_LOG_INFO("！！文件写入失败：(%d)\n",ff_result);
		}   
	}
	else 
	{
		NRF_LOG_INFO("创建文件失败   ff_result:%d!",ff_result);
	}
	f_close(pf);
}

void read_dir(DIR *dirs)
{
	FRESULT ff_result;
	uint32_t files_num = 0;
	FILINFO finfo; 
	
	ff_result = f_opendir(dirs,"/");
	if (ff_result == FR_OK) 
	{
		NRF_LOG_INFO("-----------文件列表-------\r\n");
		while ((f_readdir(dirs, &finfo) == FR_OK) && finfo.fname[0]) 
		{
			if (finfo.fattrib & AM_DIR)//如果是文件夹 
			{ 
				continue;
			} 
			else 
			{   
				files_num++;
				//显示文件名,显示文件实际大小 ,文件实际大小采用四舍五入法
			//	printf("\r\n/%12s%7ld KB ",  &finfo.fname[0],(finfo.fsize+512)/1024);
				NRF_LOG_RAW_INFO("file: %s\r\n",(uint32_t)finfo.fname);
			}
		}
		if(files_num == 0 )//无文件
		{
			NRF_LOG_INFO("无文件,请返回先创建文件!\r\n");
			return;
		}
	}
	else
	{
		NRF_LOG_INFO("打开根目录失败!\r\n");
		NRF_LOG_INFO("错误代码: %u\r\n",ff_result);
	}
}


void format_disk()
{
		FIL *pf = &file;

		DIR dirs;
		
    FRESULT ff_result;
		NRF_LOG_INFO("开始初始化文件系统\r\n");
		Flash_PowerOn();
		ff_result = f_mount(0,&fs);		
	
//		Flash_BlockErase(0);
//		ff_result=f_mkfs(0,1,4096);
//		NRF_LOG_INFO("SPI格式化硬盘成功\r\n");
//		
//		NRF_LOG_INFO("Create fatFS  ff_result:%d\r\n",ff_result);	
//		write_file(pf,(TCHAR *)filename,_accmdiphonesp,sizeof(_accmdiphonesp));
		
		read_file((TCHAR *)filename);
//		read_dir(&dirs);
		NRF_LOG_INFO("文件系统初始化成功\r\n");
}


void init_disk(void)
{
	FIL *pf = &file;
	DIR dirs;
	FRESULT ff_result;

	//init check flag pin 
	nrf_gpio_cfg_input(CHECK_DISK_PLACE_CORRECT_PIN,NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(CHECK_DISK_PLACE_PIN,NRF_GPIO_PIN_PULLUP);
	
	NRF_LOG_INFO("开始初始化文件系统\r\n");
	Flash_PowerOn();
	ff_result = f_mount(0,&fs);		

	if(ff_result==0)
	{
		NRF_LOG_INFO("文件系统初始化成功\r\n");
	}
	//	Flash_BlockErase(0);
	//	ff_result=f_mkfs(0,1,4096);
	//	NRF_LOG_INFO("SPI格式化硬盘成功\r\n");
	//		
	//	NRF_LOG_INFO("Create fatFS  ff_result:%d\r\n",ff_result);	
	//	write_file(pf,(TCHAR *)filename,_accmdiphonesp,sizeof(_accmdiphonesp));

	read_file((TCHAR *)filename);
	//		read_dir(&dirs);
}
