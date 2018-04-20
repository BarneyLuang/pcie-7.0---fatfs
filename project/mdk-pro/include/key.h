#ifndef KEY_H__
#define KEY_H__

#define DISK_WRITE_MODE_ENABLE()				nrf_gpio_pin_set(MAINBOARD_POWER_EN_PIN);					         
#define DISK_WRITE_MODE_DISABLE()				nrf_gpio_pin_clear(MAINBOARD_POWER_EN_PIN);

/* key value */
#define	BUTTON_WAKEUP					0x01		// Button A: Back
#define	BUTTON_CHANGE_MODE					0x02		// Button B: Left

#define	BUTTON_ALL					0x0F

/* Exported types ------------------------------------------------------------*/
enum KEY_CODE_DEFINE
{
	//--- press key ---
	PRESS_BUTTON_WAKEUP=1,
	PRESS_BUTTON_CHANGE_MODE,
	
	//--- hold key ---
	HOLD_BUTTON_WAKEUP,
	HOLD_BUTTON_CHANGE_MODE,

	//--- release key ---
	RELEASE_BUTTON_WAKEUP,
	RELEASE_BUTTON_CHANGE_MODE,

};
typedef struct {
	uint8_t holdkey						:1;
	uint8_t processed					:1;
	uint8_t backlight					:1;
	uint8_t 						:1;
	uint8_t 						:1;
	uint8_t 						:1;
	uint8_t 						:1;
	uint8_t 						:1;
}KEY_Status;
	
void init_key(void);
void key_scan(void);
void disk_scan(void);
#endif 

