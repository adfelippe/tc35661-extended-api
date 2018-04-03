/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/

/**
* @file tcu_api.h
*
* @brief Contains definitions and function declarations that are shared between
* the different parts of the TCU API.
*/
#ifndef __TCU_INTERNAL_H__
#define __TCU_INTERNAL_H__

#include "tcu_api.h"
//#include "debug_api.h"

uint16_t set_cmd_header_user_length(uint8_t *data, uint32_t cmd, uint16_t length, uint16_t offset);
uint16_t auto_set_cmd_header(uint8_t *data, uint32_t cmd, uint16_t offset);
enum tcu_func_status is_cmd_matching(uint8_t *data, uint32_t cmd);

void copy_array(const void *src, void *dst, uint16_t src_offset, uint16_t dst_offset, uint16_t count);
void write_array_8(uint8_t *pdata, uint8_t data, uint16_t *poffset);
void write_array_16(uint8_t *pdata, uint16_t data, uint16_t *poffset);

enum tcu_func_status tcu_send(uint8_t *data);

/*! */
#define REQUEST_PREAMBLE()     \
  uint8_t *buf = tcu_buffer;   \
  enum tcu_func_status status; \
  uint16_t offset = UART_PACKET_OFFSET;

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
