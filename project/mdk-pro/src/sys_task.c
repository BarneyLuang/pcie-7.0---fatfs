#include "include.h"

System_Timer_t system_timer_flag;
System_func_t system_func_flag;
uint32_t sys_tick_count =0;

void tick_timer_handler(void * p_context)
{
	sys_tick_count++;
	
	system_timer_flag.T_100HZ = 1;
	
	if(sys_tick_count%100 == 0)
	{
		system_timer_flag.T_1HZ =1;
	}
	if(sys_tick_count%50 == 0)
	{
		system_timer_flag.T_2HZ =1;
	}
	if(sys_tick_count%25 == 0)
	{
		system_timer_flag.T_4HZ =1;
	}
	if(sys_tick_count%20 == 0)
	{
		system_timer_flag.T_5HZ =1;
	}
	if(sys_tick_count%4 == 0)
	{
		system_timer_flag.T_25HZ =1;
	}
	
	if(sys_tick_count%10 == 0)
	{
		system_timer_flag.T_10HZ =1;
	}
	if(sys_tick_count%12 == 0)
	{
		system_timer_flag.T_8HZ =1;
	}
	if(sys_tick_count%6 == 0)
	{
		system_timer_flag.T_16HZ =1;
	}
	
	
	//NRF_LOG_INFO("tick_timer_handler tick_ocunt: %d\r\n",tick_count);
} 


void System_Task(void)
{

	if(system_timer_flag.T_25HZ)
	{
		system_timer_flag.T_25HZ = false;
	}
	
	/* scan keyboard and check backlight statue --- 16HZ */	
	if(system_timer_flag.T_16HZ)		//It's actually 25HZ
	{
		system_timer_flag.T_16HZ = false;
		/* key scan*/
		key_scan();
		disk_scan();
	}	
	
	/*  */	
	if(system_timer_flag.T_2HZ)
	{
		system_timer_flag.T_2HZ = false;
		system_func_flag.flag_led_flash_2hz	=1;
	}
	
  if(system_timer_flag.T_4HZ)
	{
		system_timer_flag.T_4HZ = false;
	}
	
	if(system_timer_flag.T_100HZ)
	{
		system_timer_flag.T_100HZ = false;
	}
	
	if(system_timer_flag.T_1HZ)
	{
		system_timer_flag.T_1HZ = false;
		get_adc_vol();
		enter_sleep_mode();
	// if the value is effective, show the value and send it out by BLE
	}

	if(system_timer_flag.T_10HZ)
	{
		system_timer_flag.T_10HZ = false;
	// if the value is effective, show the value and send it out by BLE
	}
	
}


void System_Func(void)
{
	led_func();
}

