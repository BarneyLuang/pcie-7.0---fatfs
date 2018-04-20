#include "include.h"


static bool flagblink =false ;


void init_led(void)
{
			//init led 
	nrf_gpio_cfg_output(LED_DISK_INFOR_PIN);
	nrf_gpio_cfg_output(LED_DISK_OP_BLUE_PIN);
	nrf_gpio_cfg_output(LED_DISK_OP_GREEN_PIN);
	
	nrf_gpio_cfg_output(LED_WAKE_UP_PIN);
}

void led_func(void)
{
//		if(system_func_flag.flag_led_flash_2hz)
		{
			system_func_flag.flag_led_flash_2hz = 0;
			flagblink = !flagblink;
			
			// check disk led
			if(system_func_flag.flag_disk_exict)
			{
				if(system_func_flag.flag_disk_valid )
				{ 
						LED_DISK_VALID_ON();
				}
				else 
				{
					nrf_gpio_pin_toggle(LED_DISK_INFOR_PIN);
				}
			}
			else
			{
					LED_DISK_VALID_OFF();
			}
			
			//work mode led
			switch(system_func_flag.board_mode)
			{
				case 	MODE_REBOOT_SYSTEM :
					LED_DISK_OP_BLUE_ON();
					LED_DISK_OP_GREEN_OFF();
					break;  //刷机模式
				case  MODE_OP_DISK :  
					LED_DISK_OP_GREEN_ON();
					LED_DISK_OP_BLUE_OFF();
					break;      //读写操作硬盘模式
				case	MODE_OP_DISK_ING :
					LED_DISK_OP_BLUE_OFF();
					nrf_gpio_pin_toggle(LED_DISK_OP_GREEN_PIN);
					break;    //正在读写硬盘
			}
			//wakeup led
			if(system_func_flag.flag_wakeup)
			{
				LED_WAKE_UP_ON();
			}
			else 
			{
				LED_WAKE_UP_OFF();
			}
		}
}

