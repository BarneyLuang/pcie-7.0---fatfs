#ifndef _SYS_TASK_H_
#define	_SYS_TASK_H_

#include "include.h"

typedef enum
{
	MODE_REBOOT_SYSTEM =1   ,  //刷机模式
	MODE_OP_DISK            ,  //读写操作硬盘模式
	MODE_OP_DISK_ING        ,
}board_work_mode_t;


typedef struct
{	
  uint16_t T_1HZ					: 1;
	uint16_t T_2HZ					: 1;
	uint16_t T_4HZ					: 1;
	uint16_t T_5HZ					: 1;
	uint16_t T_8HZ					: 1;
	uint16_t T_16HZ					: 1;
	uint16_t T_25HZ					: 1;
	uint16_t T_100HZ				: 1;
	uint16_t T_10HZ					: 1;
	uint16_t 					: 1;
	uint16_t 					: 1;
	uint16_t 					: 1;
	uint16_t 					: 1;
	uint16_t					: 1;
	uint16_t 					: 1;
	uint16_t					: 1;
}System_Timer_t;

typedef struct
{	
	uint16_t flag_wakeup							: 1;
	uint16_t flag_boardsleep					: 1;
	uint16_t flag_charging						: 1;
	uint16_t flag_disk_valid					: 1;
	uint16_t flag_disk_exict					: 1;
	uint16_t flag_led_flash_2hz					: 1;
	uint16_t 					: 1;
	uint16_t 					: 1;
	uint16_t 				: 1;
	uint16_t 				: 1;
	uint16_t   					: 1;
	uint16_t 						: 1;
	uint16_t 						: 1;
	uint16_t 					: 1;
	uint16_t 					: 1;
	uint16_t 					: 1;
	board_work_mode_t board_mode;
}System_func_t;

extern System_Timer_t system_timer_flag;
extern System_func_t system_func_flag;


void tick_timer_handler(void * p_context);
void System_Task(void);
void System_Func(void);
#endif 

