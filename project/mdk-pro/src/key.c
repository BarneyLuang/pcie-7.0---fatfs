#include "include.h"

KEY_Status key_status;
uint8_t new_key_status;
uint8_t old_key_status;
uint8_t key_code;
uint8_t key_hold_16hz_counter;



/*******************************************************************************
*Function	:Read_keyboard
*Descriptor	:read key value
*Input		:none
*Return		:none
*Hist		:
*******************************************************************************/
void init_key(void)
{
	nrf_gpio_cfg_input(KEY_CHANGE_MODE_PIN,NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(KEY_WAKEUP_PIN,NRF_GPIO_PIN_PULLUP);

	/*默认硬盘读写模式*/
	nrf_gpio_cfg_output(MAINBOARD_POWER_EN_PIN);

	DISK_WRITE_MODE_ENABLE();
}


/*******************************************************************************
*Function	:Read_keyboard
*Descriptor	:read key value
*Input		:none
*Return		:none
*Hist		:
*******************************************************************************/
unsigned char Read_keyboard(void)
{
	unsigned char temp = 0;
		
	if(nrf_gpio_pin_read(KEY_CHANGE_MODE_PIN)==0)
	{
		temp += BUTTON_CHANGE_MODE;
	}
	if(nrf_gpio_pin_read(KEY_WAKEUP_PIN)==0)
	{
		temp += BUTTON_WAKEUP;
	}		

	return (temp);
}  

/*******************************************************************************
*Function	:key_scan
*Descriptor	:read key value
*Input		:none
*Return		:none
*Hist		:
*******************************************************************************/
void key_scan(void)
{
	key_hold_16hz_counter++;
	key_code=0;
	new_key_status = Read_keyboard();
	
	
	//===================================================================================
	if(new_key_status!=0)			// any key press
	{
		//===============================================================================
		if(old_key_status==0)	// just press
		{
			switch(new_key_status)
			{
				case BUTTON_WAKEUP:
					key_code=PRESS_BUTTON_WAKEUP;
					system_func_flag.flag_wakeup =1;
					mainboard_wake_up();
					NRF_LOG_INFO("BUTTON_WAKEUP   按键\r\n");
					break;
				case BUTTON_CHANGE_MODE:
					key_code=PRESS_BUTTON_CHANGE_MODE;
					if(system_func_flag.board_mode!=MODE_OP_DISK_ING)
					{
						if(system_func_flag.board_mode==MODE_OP_DISK)
						{
							system_func_flag.board_mode= MODE_REBOOT_SYSTEM ;
						}
						else if(system_func_flag.board_mode==MODE_REBOOT_SYSTEM)
						{
							system_func_flag.board_mode= MODE_OP_DISK ;
						}
					}
					NRF_LOG_INFO("BUTTON_CHANGE_MODE   按键\r\n");
					break;
				default:
					break;
			}
			key_hold_16hz_counter = 0;
			old_key_status=new_key_status;		//update keyboard status
		}
		else	// press key irregularly
		{
			key_hold_16hz_counter=0;
			old_key_status=new_key_status;		//update
		}
	}
	else if (old_key_status==new_key_status)	//hold
	{
			switch(new_key_status)
			{
				case BUTTON_WAKEUP:
					if (key_hold_16hz_counter >= 25)//It's actually 25HZ
					{
						key_hold_16hz_counter=0;
					}
					break;
				case BUTTON_CHANGE_MODE:
					if(!key_status.holdkey)
					{
						if (key_hold_16hz_counter >= 25)//It's actually 25HZ
						{
							key_hold_16hz_counter=0;
							key_status.holdkey = true;
						}
					}else if(key_hold_16hz_counter >= 6)
					{
						key_hold_16hz_counter=0;
						key_status.processed = false;
					}
					break;
			}
	}
	else	//all keys release
	{
		key_status.holdkey = false;
		key_hold_16hz_counter=0;
		if(old_key_status==0)	// already release
		{
			key_status.processed = false;
		}else{
		  switch(old_key_status)	
			{
				case BUTTON_WAKEUP:
					key_code=RELEASE_BUTTON_WAKEUP;
					key_status.processed = false;
					NRF_LOG_INFO("BUTTON_WAKEUP   释放\r\n");
					break;
				case BUTTON_CHANGE_MODE:
					key_code=RELEASE_BUTTON_CHANGE_MODE;
					key_status.processed = false;
					NRF_LOG_INFO("BUTTON_CHANGE_MODE   释放\r\n");
					break;
				default:
					break;
			}
			old_key_status=new_key_status;	//update
		}
	}
	

}



void disk_scan(void)
{   
	  if(nrf_gpio_pin_read(CHECK_DISK_PLACE_PIN)==0)
		{
			system_func_flag.flag_disk_exict =1;
			if(nrf_gpio_pin_read(CHECK_DISK_PLACE_CORRECT_PIN)==0)
			{
				system_func_flag.flag_disk_valid =1;
			}
			else 
			{
				//硬盘放置不正确
				system_func_flag.flag_disk_valid =0;
			}
		}
		else 
		{
			 system_func_flag.flag_disk_exict =0;
			 system_func_flag.flag_disk_valid =0;
		}
}
