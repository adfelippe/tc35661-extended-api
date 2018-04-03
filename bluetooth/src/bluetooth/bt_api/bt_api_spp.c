/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "bt_api_spp.h"
#include "tcu_api.h"
#include "tcu_spp.h"
#include "tcu_mng.h"
#include "bt_api_internal.h"
#include "key_store_bt_api.h"
//#include "debug_api.h"

/* Defining this macro disables all debug messages. */
#define printf(fmt, args...)   /* NOPRINTF */

#define TCU_SPP_DEFAULT_CMD_TIMEOUT 100

enum tcu_func_status extract_spp_data(uint8_t *buf, void *return_data)
{
  enum tcu_func_status status;
  uint8_t opcode = PARSE_CMD_OPCODE(buf);

  if (PARSE_CMD_SERVICE_ID(buf) != TCU_BT_SERVICE_SPP) {
    return TCU_FUNC_SERVICEID;
  }

  switch (opcode) {
  case GET_CMD_OPCODE(TCU_SPP_CONNECT_EVENT):
    status = TcuSppConnect_Event(buf, (struct tcu_spp_connect_event *)return_data);
    break;
  case GET_CMD_OPCODE(TCU_SPP_DISCONNECT_EVENT):
    status = TcuSppDisconnect_Event(buf, (struct tcu_spp_disconnect_event *)return_data);
    break;
  case GET_CMD_OPCODE(TCU_SPP_LINE_NOTIFY_EVENT):
    status = TcuSppLineNotify_Event(buf, (struct tcu_spp_line_notify_event *)return_data);
    break;
  case GET_CMD_OPCODE(TCU_SPP_DATA_RECEIVE_EVENT):
    status = TcuSppDataReceive_Event(buf, (struct tcu_spp_data_receive_event *)return_data);
    break;
  case GET_CMD_OPCODE(TCU_SPP_DATA_SEND_EVENT):
    status = TcuSppDataSent_Event(buf);
    break;
  case GET_CMD_OPCODE(TCU_SPP_SETUP_RESP):
    status = TcuSppSetup_Response(buf, (struct tcu_spp_setup_response *)return_data);
    break;
  case GET_CMD_OPCODE(TCU_SPP_SHUTDOWN_RESP):
    status = TcuSppShutdown_Response(buf, (struct tcu_spp_shutdown_response *)return_data);
    break;
  case GET_CMD_OPCODE(TCU_SPP_UUID_ASSIGN_RESP):
    status = TcuSppUuidAssign_Response(buf, (struct tcu_spp_uuid_assign_response *)return_data);
    break;
  default:
    status = TCU_FUNC_PARSING;
    break;
  }

  return status;
}

static enum bt_api_result TcuSppConnectHandler(uint8_t *buf)
{
  struct tcu_spp_connect_event event;
  enum tcu_func_status status;

  printf("\n");

  status = TcuSppConnect_Event(buf, &event);
  if (status) {
    return BT_RESULT_ERROR;
  }

  /* inform application? */
  if (!event.status) {
    bt_application_spp_connect_callback(event.bd_addr);
  }
  return BT_RESULT_SUCCESS;
}

static enum bt_api_result TcuSppDisconnectHandler(uint8_t *buf)
{
  struct tcu_spp_disconnect_event event;
  enum tcu_func_status status;

  printf("\n");

  status = extract_spp_data(buf, (void *)&event);
  if (status) {
    return BT_RESULT_ERROR;
  }

  bt_application_spp_disconnect_callback();

  return BT_RESULT_SUCCESS;
}

static enum bt_api_result TcuSppDataReceiveHandler(uint8_t *buf)
{
  struct tcu_spp_data_receive_event event;
  enum tcu_func_status status;

  printf("\n");

  status = extract_spp_data(buf, (void *)&event);
  if (status) {
    return BT_RESULT_ERROR;
  }

  /* inform application? */
  bt_application_spp_data_received_callback(event.data, event.length_of_data);
  return BT_RESULT_SUCCESS;
}

static enum bt_api_result TcuSppDataSentHandler(uint8_t *buf)
{
  enum tcu_func_status status;

  printf("\n");

  status = extract_spp_data(buf, (void *)0);
  if (status) {
    return BT_RESULT_ERROR;
  }

  /* inform application? */
  bt_application_spp_data_sent_callback();

  return BT_RESULT_SUCCESS;
}

static enum bt_api_result TcuSppLineNotifyHandler(uint8_t *buf)
{
  struct tcu_spp_line_notify_event event;
  enum tcu_func_status status;

  printf("\n");

  status = extract_spp_data(buf, (void *)&event);
  if (status) {
    return BT_RESULT_ERROR;
  }

#if 0
  bt_api_spp_line_notify_callback(&event);
#endif
  return BT_RESULT_SUCCESS;
}

enum bt_api_result bt_spp_event_handler(uint8_t *buf)
{
  uint16_t command = PARSE_CMD_MESSAGE(buf);

  switch (command) {
  case GET_CMD_MESSAGE(TCU_SPP_CONNECT_EVENT):
    return TcuSppConnectHandler(buf);
  case GET_CMD_MESSAGE(TCU_SPP_DISCONNECT_EVENT):
    return TcuSppDisconnectHandler(buf);
  case GET_CMD_MESSAGE(TCU_SPP_DATA_RECEIVE_EVENT):
    return TcuSppDataReceiveHandler(buf);
  case GET_CMD_MESSAGE(TCU_SPP_DATA_SEND_EVENT):
    return TcuSppDataSentHandler(buf);
  case GET_CMD_MESSAGE(TCU_SPP_LINE_NOTIFY_EVENT):
    return TcuSppLineNotifyHandler(buf);
  default:
    printf("unhandled BT SPP event 0x%04x received\n", command);
    break;
  }
  return BT_RESULT_ERROR;
}

enum bt_api_result bt_api_spp_init(void)
{
  struct tcu_spp_setup_response response;
  enum tcu_func_status status;
  enum bt_api_result result;

  status = TcuSppSetup_Request();
  if (status != TCU_FUNC_SUCCESS) {
    printf("TcuSppInit_Request() failed\n");
    return BT_RESULT_ERROR;
  }

  result = bt_wait_for_specific_event(TCU_SPP_SETUP_RESP, TCU_SPP_DEFAULT_CMD_TIMEOUT, (void *)(&response));
  if (result) {
    printf("TcuSppInit_Request() @ TCU_Spp_INIT_RESP failed\n");
    return result;
  }

  return BT_RESULT_SUCCESS;
}

#define PRINT_BD_ADDR(x) \
  x[0], x[1], x[2], x[3], x[4], x[5]

enum bt_api_result bt_api_spp_connect(uint8_t *bd_addr, uint8_t server_channel)
{
  struct tcu_spp_connect_request request;
  struct tcu_accept_event event;
  enum tcu_func_status status;
  enum bt_api_result result;
  int ret;

  printf("bd_addr %02x:%02x:%02x:%02x:%02x:%02x\n", PRINT_BD_ADDR(bd_addr));

  memcpy(request.bd_addr, bd_addr, TCU_SIZE_BDADDR);
  request.baud_rate = TCU_SPP_BAUD_115200bps;
  request.data_format.byte = 0x00;
  request.data_format.format.data_bit = 0x03; /* 8 data bits */
  request.data_format.format.parity = 0x01;   /* odd parity */
  request.flow_control.byte = 0x06;           /* rtc_output */
  request.x_on_char = 0x01;
  request.x_off_char = 0x01;
  request.parameter.byte = 0x00;
  request.server_chanel_validity = 0x01;
  request.server_channel = server_channel;

  ret = key_store_bt_retrieve_link_key(request.bd_addr, request.link_key);
  if (!ret) {
    request.use_of_link_key = 1;
  } else {
    request.use_of_link_key = 0;
  }

  status = TcuSppConnect_Request(&request);
  if (status != TCU_FUNC_SUCCESS) {
    printf("TcuSppConnect_Request() failed\n");
    return BT_RESULT_ERROR;
  }

  result = bt_wait_for_specific_event(TCU_ACCEPT, TCU_SPP_DEFAULT_CMD_TIMEOUT, (void *)(&event));
  if (result) {
    printf("TcuSppConnect_Request() @ TCU_ACCEPT failed\n");
    return result;
  }
  return BT_RESULT_SUCCESS;
}

enum bt_api_result bt_api_spp_uuid_assign(struct bt_api_spp_uuid *initiator, struct bt_api_spp_uuid *acceptor)
{
  struct tcu_spp_uuid_assign_response response;
  struct tcu_uuid_assign_request request;
  enum tcu_func_status status;
  enum bt_api_result result;

  request.initiator_uuid_data_type = (enum tcu_spp_uuid_data_type)initiator->uuid_length;
  switch (request.initiator_uuid_data_type) {
  case TCU_SSP_UUID16:
    memcpy(&request.initiator_uuid_value[0], &initiator->uuid[0], 2);
    break;
  case TCU_SSP_UUID32:
    memcpy(&request.initiator_uuid_value[0], &initiator->uuid[0], 4);
    break;
  case TCU_SSP_UUID128:
    memcpy(&request.initiator_uuid_value[0], &initiator->uuid[0], 16);
    break;
  default:
    break;
  }

  request.acceptor_uuid_data_type = (enum tcu_spp_uuid_data_type)acceptor->uuid_length;
  switch (request.acceptor_uuid_data_type) {
  case TCU_SSP_UUID16:
    memcpy(&request.acceptor_uuid_value[0], &acceptor->uuid[0], 2);
    break;
  case TCU_SSP_UUID32:
    memcpy(&request.acceptor_uuid_value[0], &acceptor->uuid[0], 4);
    break;
  case TCU_SSP_UUID128:
    memcpy(&request.acceptor_uuid_value[0], &acceptor->uuid[0], 16);
    break;
  default:
    break;
  }

  status = TcuSppUuidAssign_Request(&request);
  if (status != TCU_FUNC_SUCCESS) {
    printf("TcuSppUuidAssign_Request() failed\n");
    return BT_RESULT_ERROR;
  }

  result = bt_wait_for_specific_event(TCU_SPP_UUID_ASSIGN_RESP, TCU_SPP_DEFAULT_CMD_TIMEOUT, (void *)(&response));
  if (result) {
    printf("TcuSppUuidAssign_Request() @ TCU_SPP_UUID_ASSIGN_RESP failed\n");
    return result;
  }
  if (response.status) {
    printf("TcuSppUuidAssign_Request() @ TCU_SPP_UUID_ASSIGN_RESP failed\n");
    return BT_RESULT_ERROR;
  }

  return BT_RESULT_SUCCESS;
}

enum bt_api_result bt_api_spp_deinit(void)
{
  struct tcu_spp_shutdown_response response;
  enum tcu_func_status status;
  enum bt_api_result result;

  status = TcuSppShutdown_Request();
  if (status != TCU_FUNC_SUCCESS) {
    printf("TcuSppShutdown_Request() failed\n");
    return BT_RESULT_ERROR;
  }

  result = bt_wait_for_specific_event(TCU_SPP_SHUTDOWN_RESP, TCU_SPP_DEFAULT_CMD_TIMEOUT, (void *)(&response));
  if (result) {
    printf("TcuSppShutdown_Request() @ TCU_SPP_SHUTDOWN_RESP failed\n");
    return result;
  }
  return BT_RESULT_SUCCESS;
}

enum bt_api_result bt_api_spp_disconnect(void)
{
  struct tcu_accept_event event;
  enum tcu_func_status status;
  enum bt_api_result result;

  status = TcuSppDisconnect_Request();
  if (status != TCU_FUNC_SUCCESS) {
    printf("TcuSppDisconnect_Request() failed\n");
    return BT_RESULT_ERROR;
  }

  result = bt_wait_for_specific_event(TCU_ACCEPT, TCU_SPP_DEFAULT_CMD_TIMEOUT, (void *)(&event));
  if (result) {
    printf("TcuSppDisconnect_Request() @ TCU_ACCEPT failed\n");
    return result;
  }

  return BT_RESULT_SUCCESS;
}

enum bt_api_result bt_api_spp_data_transfer(uint8_t *buf, uint16_t len)
{
  struct tcu_spp_data_transfer_request request;
  struct tcu_accept_event event;
  enum tcu_func_status status;
  enum bt_api_result result;

  request.data = buf;
  request.length_of_data = len;

  status = TcuSppDataTransfer_Request(&request);
  if (status != TCU_FUNC_SUCCESS) {
    printf("TcuSppDataTransfer_Request() failed\n");
    return BT_RESULT_ERROR;
  }

  result = bt_wait_for_specific_event(TCU_ACCEPT, TCU_SPP_DEFAULT_CMD_TIMEOUT, (void *)(&event));
  if (result) {
    printf("TcuSppDataTransfer_Request() @ TCU_ACCEPT failed\n");
    return result;
  }
  return BT_RESULT_SUCCESS;
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
