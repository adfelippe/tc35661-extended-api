#ifndef __BOARD__
#define __BOARD__
#include <stdint.h>
/*! board device state enumeration*/
enum board_device_state {
  board_state_hci, /*!< board is in HCI mode */
  board_state_tcu, /*!< board is in TCU mode */
};

int board_hw_reset(void);

void board_init_bluetooth_pan(void);

int board_init_bluetooth(const uint8_t **version, int *version_len);

int board_nvm_read(uint16_t address, uint8_t *data, uint32_t length);

int board_nvm_write(uint16_t address, uint8_t *data, uint32_t length);



#endif
