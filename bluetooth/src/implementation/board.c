/*  HCI API extended - Features to control radio settings
*   These additions are useful for anyone wishing to control PAN1026/TC35661
*   radio settings in order to go under any certitication test
*   Author: Anderson Felippe <adfelippe@gmail.com>
****************************************************************************/
/* "THE BEER-WARE LICENSE" (Revision 42):
 * <adfelippe@gmail.com> incremented this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return.
 * 							     Anderson Felippe
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "board.h"
#include "bluetooth_serial.h"
#include "bluetooth/hci_api/include/hci_api.h"
#include "implementation/include/hci_api.h"
#include "bluetooth/bt_api/include/bt_api_ven.h"
#include "bt_api.h"

/* Defining this macro disables all debug messages. */
//#define printf(fmt, args...)	/* NOPRINTF */

static enum board_device_state state = board_state_hci;



int board_hw_reset(void)
{
	int ret;

	system("echo 0 > /sys/class/gpio/gpio91/value");
	/* Module Reset */
	printf("Bluetooth Module Reset\n");
	system("echo 1 > /sys/class/gpio/gpio33/value");
	usleep(500000);	/* 500 mSec Delay For Restarting */
	system("echo 0 > /sys/class/gpio/gpio33/value");
	usleep(1000000); /* 100 mSec Delay after reset seq. */
	ret = hci_api_hardware_reset_handler();
	if (ret) {
		printf("hci_api_hardware_reset_handler() failed\n");
		return -1;
	}
 	printf("\n");
  	return 0;
}

/* configures UART  */
void board_init_bluetooth_pan(void)
{
	int ret;

	if((ret = board_hw_reset()) != 0)
		printf( "board_hw_reset() failed\n");

	board_configure_uart(115200, 0);

	if((ret = hci_api_reset()) != 0)
		printf( "hci_api_reset() failed\n");

	/* Give some time to TC3566x to come up again */
	usleep(100000);

	board_configure_uart(115200, 1);
}

int board_init_bluetooth(const uint8_t **version, int *version_len)
{
	uint8_t bd_addr[6];
	uint8_t buf[6];
    	int i;
      	int ret;

	board_init_bluetooth_pan();
	ret = hci_api_set_i2_enable();

	if(ret != 0)
	{
		printf("hci_api_set_i2_enable() failed\n");
	}

	ret = hci_api_read_eeprom(0xa0, 0x01, 0x02, buf, sizeof(buf));

	if(ret != 0)
	{
		printf("hci_api_read_eeprom() failed\n");
	}

	/* byte order in eeprom is reversed to what HCI controller expects */
	for(i = 0; i < 6; i++)
	{
		bd_addr[5 - i] = buf[i];
	}

	if((ret = hci_api_write_bd_addr(&bd_addr[0])) != 0)
	{
		printf("hci_api_write_bd_addr() failed\n");
	}

	if((ret = hci_api_read_firmware_version(version, version_len)) != 0)
	{
		printf("hci_api_read_firmware_version() failed\n");
	}

	if((ret = hci_api_switch_to_tcu_mode(*version, *version_len)) != 0)
	{
		printf("hci_api_switch_to_tcu_mode() failed\n");
	}

    	state = board_state_tcu;
	usleep(300000);	/* delay to allow TC356xx to switch to TCU mode */
	return 0;
}

/* this board uses the eeprom attached to the TC35661 */
#define NVM_ADDRESS 0xa0

int board_nvm_read(uint16_t address, uint8_t *data, uint32_t length)
{
  enum bt_api_result ret;

  ret = bt_api_ven_eeprom_read(NVM_ADDRESS, 0x01, address, data, length);
  if (ret) {
    printf("bt_api_ven_eeprom_write() failed\n");
    return -1;
  }

  return 0;
}

int board_nvm_write(uint16_t address, uint8_t *data, uint32_t length)
{
  enum bt_api_result ret;

  ret = bt_api_ven_eeprom_write(NVM_ADDRESS, 0x01, address, data, length);
  if (ret) {
    printf("bt_api_ven_eeprom_write() failed\n");
    return -1;
  }

  return 0;
}
