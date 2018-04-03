/****************************************************************************
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
#include "bluetooth_serial.h"
#include "board.h"
#include <unistd.h>

#include "message.h"

/* Defining this macro disables all debug messages. */
#define printf(fmt, args...)	/* NOPRINTF */

#define pdFALSE 0


int message_get_hci_timeout(uint8_t *buf, int length, int timeout_ms)
{
  uint8_t packet_type;
  uint8_t event_code;
  uint8_t command_length;
  uint8_t i;
  int  ret;

  //printf ("==>%s (%s-%d) \n",__FILE__,__FUNCTION__,__LINE__);

  ret = bt_read(&packet_type, 1, timeout_ms);

  if(ret == pdFALSE)
  {
    printf("could not extract hci message from buffer #1\n");
    return -1;
  }

  //printf ("==>%s (%s-%d) \n",__FILE__,__FUNCTION__,__LINE__);

  ret = bt_read(&event_code, 1, timeout_ms);

  if(ret == pdFALSE)
  {
    printf("could not extract hci message from buffer #2\n");
    return -1;
  }

  //printf ("==>%s (%s-%d) \n",__FILE__,__FUNCTION__,__LINE__);

  ret = bt_read(&command_length, 1, timeout_ms);

  if(ret == pdFALSE)
  {
    printf("could not extract hci message from buffer #3\n");
    return -1;
  }

  //printf ("==>%s (%s-%d) \n",__FILE__,__FUNCTION__,__LINE__);

  buf[0] = packet_type;
  buf[1] = event_code;
  buf[2] = command_length;

  for(i = 0; i < command_length; i++)
  {
    ret = bt_read(&buf[i + 3], 1, timeout_ms);

    if(ret == pdFALSE)
    {
      printf("could not extract hci message from buffer #4\n");
      return -1;
    }
  }

#if 1
  {
    int i;

    printf("command_length= %04d => ", command_length + 3);
    for(i = 0; i < command_length + 3; i++)
      printf("0x%02x ", buf[i]);
    printf("\n");
  }
#endif

  return 0;
}

int message_get_extended_hci_timeout(uint8_t *buf, int length, int timeout_ms)
{
  uint8_t packet_type;
  uint8_t event_code;
  uint8_t command_length;
  uint8_t status;
  uint8_t i;
  int  ret;

  //printf ("==>%s (%s-%d) \n",__FILE__,__FUNCTION__,__LINE__);

  ret = bt_read(&packet_type, 1, timeout_ms);

  if(ret == pdFALSE)
  {
    printf("could not extract hci message from buffer #1\n");
    return -1;
  }

  //printf ("==>%s (%s-%d) \n",__FILE__,__FUNCTION__,__LINE__);

  ret = bt_read(&event_code, 1, timeout_ms);

  if(ret == pdFALSE)
  {
    printf("could not extract hci message from buffer #2\n");
    return -1;
  }

  //printf ("==>%s (%s-%d) \n",__FILE__,__FUNCTION__,__LINE__);

  ret = bt_read(&command_length, 1, timeout_ms);

  if(ret == pdFALSE)
  {
    printf("could not extract hci message from buffer #3\n");
    return -1;
  }

  ret = bt_read(&status, 1, timeout_ms);

  if(ret == pdFALSE)
  {
    printf("could not extract hci message from buffer #4\n");
    return -1;
  }

  //printf ("==>%s (%s-%d) \n",__FILE__,__FUNCTION__,__LINE__);

  buf[0] = packet_type;
  buf[1] = event_code;
  buf[2] = command_length;
  buf[3] = status;

  for(i = 0; i < command_length; i++)
  {
    ret = bt_read(&buf[i + 4], 1, timeout_ms);

    if(ret == pdFALSE)
    {
      printf("could not extract hci message from buffer #4\n");
      return -1;
    }
  }

#if 1
  {
    int i;
    // Sum up 9 to the command length to get the full extended event message
    printf("command_length= %04d => ", command_length + 4);
    for(i = 0; i < command_length + 4; i++)
      printf("0x%02x ", buf[i]);
    printf("\n");
  }
#endif

  return 0;
}

int message_get_tcu_timeout(uint8_t *buf, int length, int timeout_ms)
{
  int len;
  int i;
  int ret;


  usleep(timeout_ms);
   printf("message_get_tcu_timeout\n");
  ret = bt_read(&buf[0], 1,0);
  if (ret == pdFALSE) {
    printf("bt_read() #1 failed\n");
    return -1;
  }
  ret = bt_read(&buf[1], 1,0 );
  if (ret == pdFALSE) {
    printf("xQueueReceive() #2 failed\n");
    return -1;
  }
  ret = bt_read(&buf[2], 1,0);
  if (ret == pdFALSE) {
    printf("xQueueReceive() #3 failed\n");
    return -1;
  }

  len = buf[0];
  len |= (buf[1] << 8);
  len |= (buf[2] << 16);

  for (i = 0; i < len - 3; i++) {
  	ret = bt_read(&buf[i+3], 1,timeout_ms);
    if (ret == pdFALSE) {
      printf("xQueueReceive() @ data failed\n");
      return -1;
    }
  }

#if 1
  {
    int i;
    printf("%4d <-- ", len);
    for (i = 0; i < len; i++)
      printf("0x%02x ", buf[i]);
    printf("\n");
  }
#endif

  return 0;
}




int message_put_unexpected_event(uint8_t *buf, int length)
{
  int ret;

  ret = board_uart_send(buf, length);
  if (ret == pdFALSE) {
    return -1;
  }

  return 0;
}
