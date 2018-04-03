/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/
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
#include "hci_api.h"
#include "board.h"
#include "hci_implementation.h"
#include "bluetooth_serial.h"
#include "message.h"

/* Defining this macro disables all debug messages. */
#define printf(fmt, args...)	/* NOPRINTF */

int hci_application_send_request(const uint8_t *buf, int len)
{
#if 0
  {
    int i;
    printf("%4d --> ", len);
    for (i = 0; i < len; i++)
      printf("0x%02x ", buf[i]);
    printf("\n");
  }
#endif

  board_uart_send(buf, len);
  return 0;
}

int hci_application_receive_response(uint8_t *buf, int len)
{
  uint8_t msg_len;
  int ret;

  ret = message_get_hci_timeout(buf, len, 250);

  //printf("==>%s (%s-%d),ret=%d \n",__FILE__,__FUNCTION__,__LINE__,ret);
  fflush(stdout);

  if(ret)
  {
    printf("message_get_hci_timeout() failed\n");
    return -1;
  }

  ret = HCI_API_CHECK_HCI_RESPONSE(buf);

  //printf("==>%s (%s-%d),ret=%d \n",__FILE__,__FUNCTION__,__LINE__,ret);
  fflush(stdout);

  if(ret)
  {
    printf("HCI_API_CHECK_HCI_RESPONSE failed\n");
    return -1;
  }

  msg_len = HCI_API_RETRIEVE_HCI_RESPONSE_LENGTH(buf);

  //printf("==>%s (%s-%d),msg_len=%d \n",__FILE__,__FUNCTION__,__LINE__,msg_len);
  fflush(stdout);

  if(msg_len > len)
  {
    printf("msg_len > len\n");
    return -1;
  }

  return msg_len;
}

int hci_application_receive_ext_response(uint8_t *buf, int len)
{
  uint8_t msg_len;
  int ret;

  ret = message_get_extended_hci_timeout(buf, len, 250);

  //printf("==>%s (%s-%d),ret=%d \n",__FILE__,__FUNCTION__,__LINE__,ret);
  fflush(stdout);

  if(ret)
  {
    printf("message_get_hci_timeout() failed\n");
    return -1;
  }

  ret = HCI_API_CHECK_HCI_RESPONSE(buf);

  //printf("==>%s (%s-%d),ret=%d \n",__FILE__,__FUNCTION__,__LINE__,ret);
  fflush(stdout);

  if(ret)
  {
    printf("HCI_API_CHECK_HCI_RESPONSE failed\n");
    return -1;
  }

  msg_len = HCI_API_RETRIEVE_HCI_RESPONSE_LENGTH(buf);

  //printf("==>%s (%s-%d),msg_len=%d \n",__FILE__,__FUNCTION__,__LINE__,msg_len);
  fflush(stdout);

  if(msg_len > len)
  {
    printf("msg_len > len\n");
    return -1;
  }

  return msg_len;
}

/************************** BEGIN of DISCLAIMER   **************************

- TOSHIBA is continually working to improve the quality and reliability of
  its products. Nevertheless, semiconductor devices in general can
  malfunction or fail due to their inherent electrical sensitivity and
  vulnerability to physical stress. It is the responsibility of the buyer,
  when utilizing TOSHIBA products, to comply with the standards of safety
  in making a safe design for the entire system, and to avoid situations in
  which a malfunction or failure of such TOSHIBA products could cause loss of
  human life, bodily injury or damage to property.

  In developing your designs, please ensure that TOSHIBA products are used
  within specified operating ranges as set forth in the most recent TOSHIBA
  products specifications.
  Also, please keep in mind the precautions and conditions set forth in the
  "Handling Guide for Semiconductor Devices," or
  "TOSHIBA Semiconductor Reliability Handbook" etc..

- The Toshiba products listed in this document are intended for usage in
  general electronics applications (computer, personal equipment, office
  equipment, measuring equipment, industrial robotics, domestic appliances,
  etc.).
  These Toshiba products are neither intended nor warranted for usage in
  equipment that requires extraordinarily high quality and/or reliability or
  a malfunction or failure of which may cause loss of human life or bodily
  injury ("Unintended Usage"). Unintended Usage include atomic energy control
  instruments, airplane or spaceship instruments, transportation instruments,
  traffic signal instruments, combustion control instruments, medical
  instruments, all types of safety devices, etc..
  Unintended Usage of Toshiba products listed in this document shall be
  made at the customer's own risk.

- Toshiba assumes no liability for any damage or losses (including but not
  limited to, loss of business profit,business interruption, loss of business
  information,and other pecuniary losses) occurring
  from the use of, or inability to use, this product.

- The products described in this document are subject to the foreign exchange
  and foreign trade laws.

- The products described in this document contain components made in the
  United States and subject to export control of the U.S. authorities.
  Diversion contrary to the U.S. law is prohibited.

- The information contained herein is presented only as a guide for the
  applications of our products.
  No responsibility is assumed by TOSHIBA CORPORATION for any infringements
  of intellectual property or other rights of the third parties which may
  result from its use.
  No license is granted by implication or otherwise under any intellectual
  property or other rights of TOSHIBA CORPORATION or others.

- The information contained herein is subject to change without notice.

****************************  END of DISCLAIMER ****************************/
