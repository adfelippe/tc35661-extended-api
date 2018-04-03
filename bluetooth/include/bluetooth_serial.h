#ifndef __BOARD_SERIAL__
#define __BOARD_SERIAL__

#include <stdint.h>
int board_configure_uart(int, int);
int bt_read(uint8_t *, uint16_t, int);
int board_uart_send(const uint8_t *, int);
int init_bt_comm(void);
void print_data_received(uint8_t *buff, uint8_t len);


#endif /* __BOARD_SERIAL__ */
