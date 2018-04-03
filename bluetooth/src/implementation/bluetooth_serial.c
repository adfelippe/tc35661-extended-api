/***************************************************************************
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
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>
#include "applog.h"
#include "bluetooth.h"
#include "bluetooth_serial.h"

/* Defining this macro disables all debug messages. */
//#define printf(fmt, args...)	/* NOPRINTF */

static int bt_fd;

int board_configure_uart(int baudrate, int flow_control)
{
	struct termios tty;
	speed_t sbaudrate ;

	switch(baudrate)
	{
		case 115200:
			sbaudrate = B115200;
			break;
		case 9600 :
			sbaudrate = B9600;
			break;
		default :
			sbaudrate = B115200;
			break;
	}

	tcgetattr(bt_fd, &tty);
	cfmakeraw(&tty);

	if(cfsetospeed(&tty, sbaudrate) < 0)
	{
		CRTCL_PRINTF(D_BT_MSG, "Error: Setting serial output baudrate %d.\n", sbaudrate);
		return -1;
	}

	if(cfsetispeed(&tty, sbaudrate) < 0)
	{
		CRTCL_PRINTF(D_BT_MSG, "Error: Setting serial input baudrate %d.\n", sbaudrate);
		return -1;
	}

#if 1
	/* Setting 8 data bits, no parity, 1 stop bit. */
	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
	tty.c_cflag &= ~(PARENB | PARODD);
	tty.c_cflag &= ~CSTOPB;
#else
	 tty.c_cflag = (tty.c_cflag & ~CSIZE);
	 tty.c_cflag = CS8 | CLOCAL | CREAD;
	 tty.c_cflag &= ~(PARENB | PARODD | CSTOPB | CRTSCTS | CMSPAR);
#endif
	tty.c_lflag = 0;
	tty.c_oflag = 0;
	tty.c_cc[VMIN] = 0;
	tty.c_cc[VTIME] = 0;

	if(flow_control == 1)
	{
		tty.c_cflag |= CRTSCTS;
	}
	else
	{
		tty.c_cflag &= ~CRTSCTS;
	}

	/* Setting the attributes up. */
	if(tcsetattr(bt_fd, TCSANOW, &tty) != 0)
	{
		CRTCL_PRINTF(D_BT_MSG, "Error: Setting serial attributes.\n");
		return -1;
	}

	/* Clearing the buffer */
	if(tcflush(bt_fd, TCIOFLUSH) != 0)
	{
		CRTCL_PRINTF(D_BT_MSG, "Error: Flushing BT Port.\n");
		return -1;
	}

	return 0;
}

int bt_read(uint8_t *pu8_recv_buff, uint16_t u16_req_len,int time_out)
{
	uint16_t u16_recv_cnt;
	int s_ret, rd_cnt;
	struct timeval port_timeval;
	fd_set set;

	// read till  u16_req_len bytes
	for (u16_recv_cnt = 0; u16_recv_cnt < u16_req_len;) {
		/* Setting fd's for select */
		FD_ZERO(&set);
		FD_SET(bt_fd, &set);
		/* Setting Read Time out */
		port_timeval.tv_sec = 5;  /* timeout_secs */
		port_timeval.tv_usec = 250000;

		s_ret = select(bt_fd + 1, &set, NULL, NULL, &port_timeval);

		switch (s_ret) {
		case 0:
			//printf("TIMEOUT\n");
			return u16_recv_cnt;
		case -1:
			printf("#");
			return -1;
		default:
			if ((rd_cnt = read(bt_fd, pu8_recv_buff + u16_recv_cnt,
					   (u16_req_len - u16_recv_cnt))) <= 0) {
				//printf("COMMUNICATION ERROR\n");
				return -ECOMM;
			} else {
				//printf("COMMUNICATION SUCCESSFUL\n");
				u16_recv_cnt += rd_cnt;
			}
			break;
		}
	}

	return u16_recv_cnt;
}

int board_uart_send(const uint8_t *data, int u16_length)
{
	uint16_t u16_snt_cnt;
	int i;

	for (u16_snt_cnt = 0; u16_snt_cnt < u16_length; u16_snt_cnt++)
	{
		if (write(bt_fd, &data[u16_snt_cnt], 1) != 1)
		{
			break;
		}

	}
	tcdrain(bt_fd);

	printf("<TX_BT> ");
	for (i = 0; i < u16_snt_cnt; i++)
	{
		if (data[i] < 0x10)
		{
			printf("0");
		}
		printf("%X ", data[i] & 0xFF);
	}
	printf("\n");

	return u16_snt_cnt;
}

void print_data_received(uint8_t *buff, uint8_t len)
{
	uint8_t i;
	printf("<RX_BT> ");
	for (i = 0; i < len; i++)
	{
		printf("%.2X ", buff[i] & 0xFF);
	}
	printf("\n");
}

int init_bt_comm(void)
{
	if((bt_fd = open(BT_COMM_PORT, O_RDWR | O_NONBLOCK)) < 0)
	{
		CRTCL_PRINTF(D_BT_MSG, "BT Port Open Failed.\n");
		return -1;
	}

	if(board_configure_uart(115200, 0) < 0)
	{
		CRTCL_PRINTF(D_BT_MSG, "BT Port Setup Failed.\n");
		return -1;
	}

	return 0;
}
