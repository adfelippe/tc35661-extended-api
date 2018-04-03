/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include "tcu_api.h"
#include "board.h"
#include "message.h"
#include "bluetooth_serial.h"
//#include "event.h"
//#include "debug.h"

/* Defining this macro disables all debug messages. */
#define printf(fmt, args...)	/* NOPRINTF */

int tcu_application_send(uint8_t *data, uint16_t length)
{
  printf("data %p, length %d\n", data, length);

#if 0
  {
    int i;
    printf("%4d --> ", length);
    for (i = 0; i < length; i++)
      printf("0x%02x ", data[i]);
    printf("\n");
  }
#endif

  board_uart_send(data, length);

  return 0;
}

extern int tcu_application_wait_for_specific_event(uint8_t expected_service_id, uint8_t expected_opcode, uint32_t timeout_ms, uint8_t *buf, int buf_length)
{
  int length;
  int ret;

again:
  printf("going to sleep\n");

  ret = message_get_tcu_timeout(buf, buf_length, timeout_ms);
  if (ret) {
    printf("message_get_tcu_timeout() failed\n");
    return -1;
  }

  length = buf[0];
  length |= (buf[1] << 8);
  length |= (buf[2] << 16);

  if (expected_service_id != PARSE_CMD_SERVICE_ID(buf)) {
    printf("unexpected cmd @ service. expected service 0x%02x, got service 0x%02x, opcode 0x%02x\n", expected_service_id, PARSE_CMD_SERVICE_ID(buf), PARSE_CMD_OPCODE(buf));
    /* handle unexpected cmd */
    ret = message_put_unexpected_event(buf, length);
    printf("message_put_unexpected_event() failed\n");
    goto again;
  }
  if (expected_opcode != PARSE_CMD_OPCODE(buf)) {
    printf("unexpected cmd @ opcode. expected opcode 0x%02x, got service 0x%02x, opcode 0x%02x\n", expected_opcode, PARSE_CMD_SERVICE_ID(buf), PARSE_CMD_OPCODE(buf));
    /* handle unexpected cmd */
    ret = message_put_unexpected_event(buf, length);
    printf("message_put_unexpected_event() failed\n");
    goto again;
  }
printf("tcu_application_wait_for_specific_event success\n");
  return 0;
}

#ifdef DEBUG
void tcu_application_log_request(uint8_t *buf, const char *name, const char *format, ...)
{
  printf("%s: ", name);
  va_list myargs;
  va_start(myargs, format);
  vprintf(format, myargs);
  va_end(myargs);
  printf("\n");
}

void tcu_application_log_response(uint8_t *buf, const char *name, const char *format, ...)
{
  printf("%s: ", name);
  va_list myargs;
  va_start(myargs, format);
  vprintf(format, myargs);
  va_end(myargs);
  printf("\n");
}

void tcu_application_log_event(uint8_t *buf, const char *name, const char *format, ...)
{
  printf("%s: ", name);
  va_list myargs;
  va_start(myargs, format);
  vprintf(format, myargs);
  va_end(myargs);
  printf("\n");
}
#else
void tcu_application_log_request(uint8_t *buf, const char *name, const char *format, ...)
{
}

void tcu_application_log_response(uint8_t *buf, const char *name, const char *format, ...)
{
}

void tcu_application_log_event(uint8_t *buf, const char *name, const char *format, ...)
{
}
#endif

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
