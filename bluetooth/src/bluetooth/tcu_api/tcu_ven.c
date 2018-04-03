/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "tcu_internal.h"
#include "tcu_ven.h"

#include <assert.h>

/* Defining this macro disables all debug messages. */
#define printf(fmt, args...)	/* NOPRINTF */

enum tcu_func_status TcuVenGetModuleMaintenenace_Event(uint8_t *buf, struct tcu_ven_get_module_maintenance_event *event)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  printf("PARSE_CMD_SERVICE_ID =%x\n",PARSE_CMD_SERVICE_ID(buf) );
  printf("PARSE_CMD_OPCODE = %x\n",PARSE_CMD_OPCODE(buf) ) ;
  printf("GET_CMD_SERVICE_ID(TCU_VEN_GET_MODULE_MAINTENANCE_EVENT =%x\n",GET_CMD_SERVICE_ID(TCU_VEN_GET_MODULE_MAINTENANCE_EVENT));
  printf("GET_CMD_OPCODE(TCU_VEN_GET_MODULE_MAINTENANCE_EVENT =%x\n", GET_CMD_OPCODE(TCU_VEN_GET_MODULE_MAINTENANCE_EVENT));



  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_VEN_GET_MODULE_MAINTENANCE_EVENT));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_VEN_GET_MODULE_MAINTENANCE_EVENT));

  printf("buf[%d] =%d ",offset,buf[offset]);
  event->acceptor = buf[offset++];
  printf("event->acceptor =%d  \n",event->acceptor);
  printf(" buf[%d] =%d ",offset,buf[offset]);
  event->information_id = buf[offset++];
  printf("event->information_id =%d \n",event->information_id);
  printf("buf[%d] =%d",offset,buf[offset]);
  event->result = buf[offset++];
  printf("event->result =%d  \n",event->result);
  printf("buf[%d] =%d",offset,buf[offset]);
  event->information_type = buf[offset++];
  printf("event->information_type =%d \n",event->information_type);
  printf("buf[%d] =%d \n", offset,buf[offset]);
#if 1
  if (event->length != buf[offset]) {
    return TCU_FUNC_LENGTH;
  }
	#endif
  event->length = buf[offset++];
  printf("event->length = %d  \n",event->length );
  copy_array(&buf[offset], event->data, 0, 0, event->length);

  tcu_application_log_event(buf, "TCU_VEN_GET_MODULE_MAINTENANCE_EVENT", "");

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuVenSetModuleMaintenenace_Event(uint8_t *buf, struct tcu_ven_set_module_maintenance_event *event)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_VEN_SET_MODULE_MAINTENANCE_EVENT));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_VEN_SET_MODULE_MAINTENANCE_EVENT));

  event->acceptor = buf[offset++];
  event->information_id = buf[offset++];
  event->result = buf[offset++];

  tcu_application_log_event(buf, "TCU_VEN_SET_MODULE_MAINTENANCE_EVENT", "");

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuVenSetHostWakeupNotification_Request(struct tcu_ven_set_host_wakeup_notification_request *request)
{
  REQUEST_PREAMBLE();

  /* Set command op-code service id and parameter length */
  offset = set_cmd_header_user_length(buf, TCU_VEN_SET_HOST_WAKEUP_NOTIFICATION_REQ, 1, offset);

  buf[offset++] = request->mode;

  status = tcu_send(buf);
  if (status) {
    return status;
  }

  tcu_application_log_request(buf, "TCU_VEN_SET_HOST_WAKEUP_NOTIFICATION_REQ", "mode %d", request->mode);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuVenSetHostWakeupNotification_Response(uint8_t *buf, struct tcu_ven_set_host_wakeup_notification_response *response)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_VEN_SET_HOST_WAKEUP_RESP));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_VEN_SET_HOST_WAKEUP_RESP));

  response->status = (enum tcu_mng_status)buf[offset];

  tcu_application_log_response(buf, "TCU_VEN_SET_HOST_WAKEUP_RESP", "status %d", response->status);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuVenGetModuleMaintenance_Request(struct tcu_ven_get_module_maintenance_request *request)
{
  REQUEST_PREAMBLE();

  /* Set command op-code service id and parameter length */
  offset = set_cmd_header_user_length(buf, TCU_VEN_GET_MODULE_MAINTENANCE_REQ, 5 + request->request_len, offset);

  /* generic m2 get */
  buf[offset++] = 0x01; /* acceptor */
  buf[offset++] = (uint8_t)request->command;
  buf[offset++] = 0xff; /* reserved */
  buf[offset++] = (uint8_t)request->data_type;
  buf[offset++] = request->request_len;

  memcpy(&buf[offset], request->request_buf, request->request_len);
  offset += request->request_len;

  status = tcu_send(buf);
  if (status) {
    return status;
  }

  tcu_application_log_request(buf, "TCU_VEN_GET_MODULE_MAINTENANCE_REQ", "");

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuVenSetModuleMaintenance_Request(struct tcu_ven_set_module_maintenance_request *request)
{
  REQUEST_PREAMBLE();

  /* Set command op-code service id and parameter length */
  offset = set_cmd_header_user_length(buf, TCU_VEN_SET_MODULE_MAINTENANCE_REQ, 5 + request->request_len + request->data_len, offset);

  /* generic m2 set */
  buf[offset++] = 0x01; /* acceptor */
  buf[offset++] = (uint8_t)request->command;
  buf[offset++] = 0xff; /* reserved */
  buf[offset++] = (uint8_t)request->data_type;
  buf[offset++] = request->request_len + request->data_len;

  memcpy(&buf[offset], request->request_buf, request->request_len);
  offset += request->request_len;

  memcpy(&buf[offset], request->data_buf, request->data_len);
  offset += request->data_len;

  status = tcu_send(buf);
  if (status) {
    return status;
  }

  tcu_application_log_request(buf, "TCU_VEN_SET_MODULE_MAINTENANCE_REQ", "");

  return TCU_FUNC_SUCCESS;
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
