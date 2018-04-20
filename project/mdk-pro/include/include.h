#ifndef _INCLUDE_H__
#define _INCLUDE_H__


#include "app_util_platform.h"
#include "app_timer.h"
#include "app_button.h"
#include "app_uart.h"

#include "nrf_gpio.h"
#include "nrf_drv_spi.h"
#include "nrf_drv_adc.h"

#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_delay.h"


/*    central   io   									  */
#define CHECK_DISK_PLACE_CORRECT_PIN   		3
#define CHECK_DISK_PLACE_PIN   		4
#define CHECK_CHARGING_PIN   		  12
#define MAINBOARD_POWER_EN_PIN   		13
/*    led 															*/
#define LED_DISK_INFOR_PIN        5
#define LED_DISK_OP_BLUE_PIN      8
#define LED_DISK_OP_GREEN_PIN     4
#define LED_WAKE_UP_PIN        		10
/*     key  io 													*/
#define KEY_WAKEUP_PIN   					21      //7
#define KEY_CHANGE_MODE_PIN       20 			//6 
/* Flash io 												  	*/
#define FLASH_W25Q32_CS          				 16 //24
#define FLASH_W25Q32_SO         				 28
#define FLASH_W25Q32_SI         				 25
#define FLASH_W25Q32_SCK         				 29
#define FLASH_W25Q32_WP          				 17
#define FLASH_W25Q32_HOLD         			 18
#define FLASH_W25Q32_POWER         			 19

#define LED_DISK_VALID_ON()    	 	nrf_gpio_pin_set(LED_DISK_INFOR_PIN);
#define LED_DISK_VALID_OFF()    	nrf_gpio_pin_clear(LED_DISK_INFOR_PIN);
#define LED_DISK_OP_BLUE_ON()   	nrf_gpio_pin_set(LED_DISK_OP_BLUE_PIN);
#define LED_DISK_OP_BLUE_OFF()    nrf_gpio_pin_clear(LED_DISK_OP_BLUE_PIN);
#define LED_DISK_OP_GREEN_ON()   	nrf_gpio_pin_set(LED_DISK_OP_GREEN_PIN);
#define LED_DISK_OP_GREEN_OFF()   nrf_gpio_pin_clear(LED_DISK_OP_GREEN_PIN);
#define LED_WAKE_UP_ON()  			 	nrf_gpio_pin_set(LED_WAKE_UP_PIN);
#define LED_WAKE_UP_OFF()    			nrf_gpio_pin_clear(LED_WAKE_UP_PIN);

#define FUNC_EXTERNAL_FLASH_EN

#define  FIRMWARE_VERSION        					101

/*service */
#include "ble_nus.h"                                /**< UART RX buffer size. */

//fatfs
#include "ff.h"
#include "diskio.h"

//application
#include "flash.h"
#include "memory_interface.h"
#include "ble_cmd_op.h"

/* bsp */
#include "key.h"
#include "power.h"
#include "sys_task.h"
#include "led.h"
#define CENTRAL_LINK_COUNT              0                                           /**< Number of central links used by the application. When changing this number remember to adjust the RAM settings*/
#define PERIPHERAL_LINK_COUNT           1                                           /**< Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/

#define DEVICE_NAME                     "NewStart"                               /**< Name of device. Will be included in the advertising data. */
#define NUS_SERVICE_UUID_TYPE           BLE_UUID_TYPE_BLE                  /**< UUID type for the Nordic UART Service (vendor specific). */

#define APP_ADV_INTERVAL                64                                          /**< The advertising interval (in units of 0.625 ms. This value corresponds to 40 ms). */
#define APP_ADV_TIMEOUT_IN_SECONDS      180                                         /**< The advertising timeout (in units of seconds). */

#define APP_TIMER_PRESCALER             0                                           /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE         4                                           /**< Size of timer operation queues. */

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(20, UNIT_1_25_MS)             /**< Minimum acceptable connection interval (20 ms), Connection interval uses 1.25 ms units. */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(75, UNIT_1_25_MS)             /**< Maximum acceptable connection interval (75 ms), Connection interval uses 1.25 ms units. */
#define SLAVE_LATENCY                   0                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)             /**< Connection supervisory timeout (4 seconds), Supervision Timeout uses 10 ms units. */
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER)  /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER) /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                           /**< Number of attempts before giving up the connection parameter negotiation. */

#define DEAD_BEEF                       0xDEADBEEF                                  /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

#define UART_TX_BUF_SIZE                256                                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE                256         


extern  const unsigned char _accmdiphonesp[0x23E9];


/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
extern ble_nus_t                        m_nus;         
extern const app_timer_id_t m_tick_timer_id ;
extern const app_timer_id_t m_key_timer_id ;
#endif 

