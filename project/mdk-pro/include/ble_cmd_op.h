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
	DISK_OP_SERIES_NUM = 1,//序列号
	DISK_OP_MOD ,	//监管
	DISK_OP_REGN ,//国家
	DISK_OP_RMD ,//型号
	DISK_OP_WMAC ,//wifi
	DISK_OP_BMAC ,//ble
	DISK_OP_CLHS,//color
	DISK_OP_NVSN ,//指纹码
	DISK_OP_LCM,//液晶码
	DISK_OP_BATT,//电池码
	DISK_OP_BCMS,//后相机
	DISK_OP_PCMS,//前相机
	DISK_OP_MLB,//主板码
}DISK_OP_T;


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
	
	CMD_PhoneInfor_SrNm				= 0x20,
	CMD_PhoneInfor_Mod				= 0x21,
	CMD_PhoneInfor_Regn				= 0x22,
	CMD_PhoneInfor_RMd				= 0x23,
	CMD_PhoneInfor_WMac				= 0x24,
	CMD_PhoneInfor_BMac				= 0x25,
	CMD_PhoneInfor_CLHS				= 0x26,
	CMD_PhoneInfor_NvSn				= 0x27,
	CMD_PhoneInfor_LCM				= 0x28,
	CMD_PhoneInfor_Batt				= 0x29,
	CMD_PhoneInfor_BCMS				= 0x2A,
	CMD_PhoneInfor_FCMS				= 0x2B,
	CMD_PhoneInfor_MLB				= 0x2C,
	// error code 
	CMD_Error              		= 0x88,
	CMD_Data_END							= 0xff,
}Protocol;	

typedef struct{
	uint8_t cmd_head;
	uint8_t data[19];
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

