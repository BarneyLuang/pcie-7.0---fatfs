#ifndef BLE_CMD_OP_H
#define BLE_CMD_OP_H


/* Includes ------------------------------------------------------------------*/
#include "include.h"


/* Exported define -----------------------------------------------------------*/
#define BLE_CMD_HEAD_1          0xaa
#define BLE_CMD_HEAD_2          0xbb
/* Exported types ------------------------------------------------------------*/

typedef enum
{
	READ_CODE = 0,
	WRITE_CODE,
}OP_CODE;


typedef enum
{
	CMD_UpdateDisk						= 0x10,
	CMD_GetBattery						= 0x11,
	CMD_ReadFile							= 0x12,
	CMD_WriteFile			   			= 0x13,
	CMD_OperateDiskMode				= 0x14,
	CMD_WritePhoneInfor				= 0x16,
	CMD_ReadPhoneInfor 				= 0x17,
	CMD_FirmwareVersion				= 0x18,
	      
	//	CMD_GetDeviceAddress	= 0x19,
	CMD_TransparentDebug      = 0x1A,
	CMD_BleOTA			    			= 0x1B,
	CMD_BleSleepMode					= 0x1D,	
	// error code 
	CMD_Error              		= 0x88,

	CMD_Data_END							= 0xff,
}Protocol;	

typedef struct{
	uint8_t cmd_head;
	uint8_t data[20];
	uint8_t size;
	uint8_t index;
}ble_cmd;


/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define CMD_HEAD_1            0xAA
#define CMD_HEAD_2            0xBB
#define DATA_LEN							20
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void uart_tx_data(uint8_t *data,uint32_t size);
void nus_data_handler_protocol_op(ble_cmd * data,uint8_t len);
#endif

