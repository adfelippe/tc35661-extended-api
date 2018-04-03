/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/

#include "tcu_api.h"

/*! Sets command service-id in the dest array */
#define SET_CMD_SVC_ID(data, svc_id) (data[CMD_ARRAY_SER_ID_OFFS] = GET_CMD_SERVICE_ID(svc_id))
/*! Sets comamnd op-code in the dest array */
#define SET_CMD_OP_CODE(data, op_code) (data[CMD_ARRAY_OPCODE_OFFS] = GET_CMD_OPCODE(op_code))

void vSetCmdLength(uint8_t *data, uint16_t length)
{
  data[CMD_ARRAY_LEN_OFFS] = ((length)&0xFF);
  data[CMD_ARRAY_LEN_OFFS + 1] = ((length) >> 8 & 0xFF);
}

void write_array_8(uint8_t *pdata, uint8_t data, uint16_t *poffset)
{
  pdata[*poffset] = data;
  *poffset += 1;
}

void write_array_16(uint8_t *pdata, uint16_t data, uint16_t *poffset)
{
  pdata[*poffset] = GET_BYTE_LSB(data);
  pdata[*poffset + 1] = GET_BYTE_MSB((data));
  *poffset += 2;
}

enum tcu_func_status tcu_send(uint8_t *data)
{
  uint16_t length;

  /* Total UART package length:3-bytes for packet length + 4-byte BLE message header + BLE message */
  length = UART_PACKET_OFFSET + GET_TOTAL_LE_MESSAGE_SIZE(data);

  /* Set length of entire UART package (first 3-bytes) */
  data[0] = (length >> 0) & 0xff;
  data[1] = (length >> 8) & 0xff;
  data[2] = 0; /* (length >> 16) & 0xff; */

  /* Send data via UART */
  tcu_application_send(data, length);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status is_cmd_matching(uint8_t *data, uint32_t cmd)
{
  enum tcu_func_status status;

  /* Check if matches */
  if (PARSE_CMD_SERVICE_ID(data) == GET_CMD_SERVICE_ID(cmd)) {
    if (PARSE_CMD_OPCODE(data) == GET_CMD_OPCODE(cmd)) {
      /* Command check OK */
      status = TCU_FUNC_SUCCESS;
    } else {
      /* Op-code error */
      status = TCU_FUNC_OPCODE;
    }
  } else {
    /* Service ID error */
    status = TCU_FUNC_SERVICEID;
  }

  return status;
}

uint16_t auto_set_cmd_header(uint8_t *data, uint32_t cmd, uint16_t offset)
{
  uint16_t count = 0;

  /* Set service ID */
  SET_CMD_SVC_ID(data, cmd);
  count++;

  /* Set Opcode */
  SET_CMD_OP_CODE(data, cmd);
  count++;

  /* Set length */
  vSetCmdLength(data, GET_PARAM_LENGTH(cmd));
  count += 2;

  return (offset + count);
}

void copy_array(const void *src, void *dst, uint16_t src_offset, uint16_t dst_offset, uint16_t count)
{
  uint8_t *psrc = (uint8_t *)src;
  uint8_t *pdst = (uint8_t *)dst;

  uint16_t i;

  for (i = 0; i < count; i++) {
    pdst[dst_offset + i] = psrc[src_offset + i];
  }
}

uint16_t set_cmd_header_user_length(uint8_t *data, uint32_t cmd, uint16_t length, uint16_t offset)
{
  uint16_t count = 0;

  /* Set service ID */
  SET_CMD_SVC_ID(data, cmd);
  count++;

  /* Set Opcode */
  SET_CMD_OP_CODE(data, cmd);
  count++;

  /* Set length */
  vSetCmdLength(data, length);
  count += 2;

  return (count + offset);
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
