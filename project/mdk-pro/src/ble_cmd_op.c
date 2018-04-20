#include "include.h"


uint8_t demo_txt_file[] = "demo.txt";
uint8_t phone8sp_txt_file[] = "phone8.txt";
uint8_t uart_tx_buff[100];
ble_cmd ble_tx_data ; 
bool flg_ble_rx_finish =false;
#define INIT_TX_DATA_BUFF()   {memset(&ble_tx_data,0,sizeof(ble_tx_data));

/*******************************************************************************
*Function			: ble_cmd_rx_updatadisk(uint8_t * data,uint8_t len)
*Descriptor		: 
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void uart_tx_data(uint8_t *data,uint32_t size)
{
#if (NRF_LOG_ENABLED==0)
	int i=0;
	while(size)
	{
		while(NRF_SUCCESS!=app_uart_put(data[i]));
		i++;
		size--;
		if(i%15==0)
		{
			nrf_delay_ms(20);
		}
	}
#endif 	
}


/*******************************************************************************
*Function			: ble_cmd_rx_updatadisk(uint8_t * data,uint8_t len)
*Descriptor		: 
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void ble_cmd_rx_updatadisk(ble_cmd *op_cmd)
{
		
}


/*******************************************************************************
*Function			: ble_cmd_read_getfile(uint8_t *data)
*Descriptor		: 
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void ble_cmd_read_file(ble_cmd *op_cmd)
{
			
			// read_file();
}



/*******************************************************************************
*Function			: ble_cmd_read_getfile(uint8_t *data)
*Descriptor		: 
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void ble_cmd_write_file(ble_cmd *data)
{
	FIL *pf;
	DIR dirs;
	pf =&file;
	uint8_t filename[]="cmd.txt";
	write_file(pf,(TCHAR *)filename,_accmdiphonesp,sizeof(_accmdiphonesp));
	read_file((TCHAR *)filename);
	read_dir(&dirs);
}


/*******************************************************************************
*Function			: ble_cmd_write_phoneinfor(uint8_t *data)
*Descriptor		: 
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void ble_cmd_write_phoneinfor(ble_cmd *data)
{
	uint8_t cmd,version;

	cmd = data->data[1];
	version = data->data[0];
	
	iphone_disk_command_op(version,(DISK_CMD)cmd);	
}


/*******************************************************************************
*Function			: ble_cmd_read_phoneinfor(uint8_t *data)
*Descriptor		: 
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void ble_cmd_read_phoneinfor(ble_cmd *data)
{
	uint16_t index=0,targetindex,resize=4096;
	TCHAR *char_result;
	FIL *pf;
	pf =  &file;

	targetindex = data->data[0]+ DISK_CMD_WRITE_BASE_OFFSET;
	while(1)
	{
		char_result = f_gets ((TCHAR *)ReadBuffer,	resize,pf);	
//		NRF_LOG_RAW_HEXDUMP_INFO(ReadBuffer,strlen(ReadBuffer));	
		index++;
		if(index==targetindex)
		{
			uart_tx_data((uint8_t *)ReadBuffer,strlen((const char *)ReadBuffer));	
		}
		if(char_result==0)  
		{
			f_close(pf);
			break;
		}		
	}
}


/*******************************************************************************
*Function			: ble_cmd_read_phoneinfor(uint8_t *data)
*Descriptor		: 
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void ble_cmd_transparentdebug(ble_cmd *data)
{	
	ble_nus_string_send(&m_nus,(uint8_t *)data,20);
}


/*******************************************************************************
*Function			: ble_cmd_read_phoneinfor(uint8_t *data)
*Descriptor		: 
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void ble_cmd_getversion(ble_cmd *data)
{	
	
	ble_tx_data.cmd_head = data->cmd_head;
	ble_tx_data.data[0] = FIRMWARE_VERSION;
	
	ble_nus_string_send(&m_nus,(uint8_t *)&ble_tx_data,20);
}


/*******************************************************************************
*Function			: ble_cmd_getbattery(ble_cmd *data)
*Descriptor		: 
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void ble_cmd_getbattery(ble_cmd *data)
{	
	uint16_t battery = 300;
	uint16_t current = 1000;//ma
	ble_cmd tx_data ; 
	
	tx_data.cmd_head = data->cmd_head;
	tx_data.data[0] = (battery>>8)&0xff;
	tx_data.data[1] = (battery)&0xff;
	tx_data.data[2] = (current>>8)&0xff;
	tx_data.data[3] = (current)&0xff;
	
	ble_nus_string_send(&m_nus,(uint8_t *)&tx_data,20);
}


/*******************************************************************************
*Function			: ble_cmd_getbattery(ble_cmd *data)
*Descriptor		: 
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void ble_cmd_operatediskmode(ble_cmd *data)
{	  
	ble_cmd tx_data ; 

	tx_data.cmd_head = data->cmd_head;
	if(data->data[0]==READ_CODE)
	{
		
	}
	else if(data->data[0]==WRITE_CODE)
	{
		
	}
	
	ble_nus_string_send(&m_nus,(uint8_t *)&tx_data,20);
}

/*******************************************************************************
*Function			: ble_cmd_updatedisk(ble_cmd *data)
*Descriptor		: 
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void ble_cmd_updatedisk(ble_cmd *data)
{	
	uint16_t battery = 300;
	uint16_t current = 1000;//ma
	uint8_t stringlen ;
	uint8_t offset = 16;
	ble_cmd tx_data ; 
	
	tx_data.cmd_head = data->cmd_head;
	tx_data.cmd_num = data->cmd_num;
	tx_data.index = data->index;
	
	switch(data->data[0])
	{
		case DISK_OP_SERIES_NUM: stringlen = 12;break;
		case DISK_OP_MOD:        stringlen = 5; break; 
		case DISK_OP_REGN:			 stringlen = 4;	break;
		case DISK_OP_RMD:				 stringlen = 5; break;
		case DISK_OP_WMAC:			 stringlen = 40;break;
		case DISK_OP_BMAC:			 stringlen = 40;break;
		case DISK_OP_CLHS:				break;
		case DISK_OP_NVSN:			stringlen = 17; break;
		case DISK_OP_LCM:				stringlen = 40;break;
		case DISK_OP_BATT:			stringlen = 17;break;
		case DISK_OP_BCMS:			stringlen = 17;break;
		case DISK_OP_PCMS:			stringlen = 17;break;
		case DISK_OP_MLB:				stringlen = 20;break;
	}
	
	if(flg_ble_rx_finish)
	{
		read_file_data_for_line( phone8sp_txt_file,data->data[0],uart_tx_buff);
		memcpy(uart_tx_buff,data->data+1,stringlen);
		uart_tx_data(uart_tx_buff,strlen(uart_tx_buff));		
	}


	ble_nus_string_send(&m_nus,(uint8_t *)&tx_data,20);
}

/*******************************************************************************
*Function			: nus_data_handler_protocol_op(uint8_t * data,uint8_t len)
*Descriptor		: Ble data protocol operation
*Input				: none
*Return				: none
*Hist					:
*******************************************************************************/
void nus_data_handler_protocol_op(ble_cmd * data,uint8_t len)
{		
	INIT_TX_DATA_BUFF();

	switch(data->cmd_head)
	{
		case CMD_UpdateDisk:				ble_cmd_getbattery(data);			break;
		case CMD_GetBattery:				ble_cmd_getbattery(data);			break;
		case CMD_WritePhoneInfor:	ble_cmd_write_phoneinfor(data);break;
		case CMD_ReadPhoneInfor:		ble_cmd_read_phoneinfor(data);break;
		case CMD_ReadFile:					ble_cmd_read_file(data);		 	break;
		case CMD_WriteFile:				ble_cmd_write_file(data);			break;
		case CMD_TransparentDebug: ble_cmd_transparentdebug(data);break;
		case CMD_OperateDiskMode : 	break;
		case CMD_FirmwareVersion:  ble_cmd_getversion(data);break;
		
		case CMD_PhoneInfor_SrNm :break;
		case CMD_PhoneInfor_Mod :break;
		case CMD_PhoneInfor_SrNm :break;
		case CMD_PhoneInfor_Regn :break;
		case CMD_PhoneInfor_RMd :break;
		case CMD_PhoneInfor_WMac :break;
		case CMD_PhoneInfor_BMac :break;
		case CMD_PhoneInfor_CLHS :break;
		case CMD_PhoneInfor_NvSn :break;
		case CMD_PhoneInfor_CLHS :break;
		case CMD_PhoneInfor_LCM :break;
		case CMD_PhoneInfor_Batt :break;
		case CMD_PhoneInfor_BCMS :break;
		case CMD_PhoneInfor_FCMS :break;
		case CMD_PhoneInfor_MLB :break;
		
	 default:break;
	}
}

