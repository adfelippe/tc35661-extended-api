/****************************************************************************
*   COPYRIGHT 2015 All rights reserved
*   Toshiba Electronics Europe GmbH
*   European LSI Design and Engineering Center (ELDEC)
****************************************************************************/

#include "tcu_internal.h"
#include "tcu_spp.h"
#include <assert.h>
enum tcu_func_status TcuSppSetup_Request(void)
{
  REQUEST_PREAMBLE();

  offset = set_cmd_header_user_length(buf, TCU_SPP_SETUP_REQ, 0, offset);

  status = tcu_send(buf);
  if (status) {
    return status;
  }

  tcu_application_log_request(buf, "TCU_SPP_SETUP_REQ", "");

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuSppSetup_Response(uint8_t *buf, struct tcu_spp_setup_response *response)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_SPP_SETUP_RESP));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_SPP_SETUP_RESP));

  response->status = (enum tcu_mng_status)buf[offset];

  tcu_application_log_response(buf, "TCU_SPP_SETUP_RESP", "status %d", response->status);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuSppShutdown_Request(void)
{
  REQUEST_PREAMBLE();

  offset = set_cmd_header_user_length(buf, TCU_SPP_SHUTDOWN_REQ, 0, offset);

  status = tcu_send(buf);
  if (status) {
    return status;
  }

  tcu_application_log_request(buf, "TCU_SPP_SHUTDOWN_REQ", "");

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuSppShutdown_Response(uint8_t *buf, struct tcu_spp_shutdown_response *response)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_SPP_SHUTDOWN_RESP));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_SPP_SHUTDOWN_RESP));

  response->status = (enum tcu_mng_status)buf[offset];

  tcu_application_log_response(buf, "TCU_SPP_SHUTDOWN_RESP", "status %d", response->status);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuSppConnect_Request(struct tcu_spp_connect_request *request)
{
  uint8_t i;
  REQUEST_PREAMBLE();

  offset = CMD_ARRAY_PARAM_OFFS;

  copy_array(request->bd_addr, buf, 0, offset, TCU_SIZE_BDADDR);
  offset += TCU_SIZE_BDADDR;
  write_array_8(buf, request->baud_rate, &offset);
  write_array_8(buf, request->data_format.byte, &offset);
  write_array_8(buf, request->flow_control.byte, &offset);
  write_array_8(buf, request->x_on_char, &offset);
  write_array_8(buf, request->x_off_char, &offset);
  write_array_8(buf, request->parameter.byte & 0xff, &offset);
  write_array_8(buf, (request->parameter.byte >> 8) & 0xff, &offset);
  write_array_8(buf, request->server_chanel_validity, &offset);
  write_array_8(buf, request->server_channel, &offset);
  write_array_8(buf, request->use_of_link_key, &offset);
  if (request->use_of_link_key != 0) {
    for (i = 0; i < TCU_SIZE_LINKKEY; i++) {
      write_array_8(buf, request->link_key[i], &offset);
    }
  }

  offset = set_cmd_header_user_length(buf, TCU_SPP_CONNECT_REQ, offset - 7, UART_PACKET_OFFSET);
  status = tcu_send(buf);
  if (status) {
    return status;
  }

  tcu_application_log_request(buf, "TCU_SPP_CONNECT_REQ", "");

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuSppConnect_Event(uint8_t *buf, struct tcu_spp_connect_event *event)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_SPP_CONNECT_EVENT));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_SPP_CONNECT_EVENT));

  event->status = (enum tcu_mng_status)buf[offset++];
  copy_array(buf, event->bd_addr, offset, 0, sizeof(event->bd_addr));
  offset += sizeof(event->bd_addr);
  event->negotiated_frame_size_lower = buf[offset++];
  event->negotiated_frame_size_upper = buf[offset++];
  event->length_of_device_name = buf[offset++];
  copy_array(buf, event->device_name, offset, 0, event->length_of_device_name);

  tcu_application_log_response(buf, "TCU_SPP_CONNECT_EVENT", "status %d", event->status);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuSppDisconnect_Request(void)
{
  REQUEST_PREAMBLE();

  offset = set_cmd_header_user_length(buf, TCU_SPP_DISCONNECT_REQ, 0, offset);

  status = tcu_send(buf);
  if (status) {
    return status;
  }

  tcu_application_log_request(buf, "TCU_SPP_DISCONNECT_REQ", "");

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuSppDisconnect_Event(uint8_t *buf, struct tcu_spp_disconnect_event *event)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_SPP_DISCONNECT_EVENT));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_SPP_DISCONNECT_EVENT));

  event->status = (enum tcu_mng_status)buf[offset++];
  copy_array(&buf[offset], event->bd_addr, 0, 0, TCU_SIZE_BDADDR);
  offset += sizeof(event->bd_addr);
  event->reason = (enum tcu_spp_disconnect_reason)buf[offset];

  tcu_application_log_response(buf, "TCU_SPP_DISCONNECT_EVENT", "status %d", event->status);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuSppLineNotify_Event(uint8_t *buf, struct tcu_spp_line_notify_event *event)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_SPP_LINE_NOTIFY_EVENT));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_SPP_LINE_NOTIFY_EVENT));

  event->status = (enum tcu_mng_status)buf[offset++];
  event->line_status = buf[offset];

  tcu_application_log_response(buf, "TCU_SPP_LINE_NOTIFY_EVENT", "status %d", event->status);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuSppDataReceive_Event(uint8_t *buf, struct tcu_spp_data_receive_event *event)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_SPP_DATA_RECEIVE_EVENT));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_SPP_DATA_RECEIVE_EVENT));

  copy_array(&buf[offset], &event->length_of_data, 0, 0, sizeof(uint16_t));
  offset += sizeof(uint16_t);
  event->data = &buf[offset];

  tcu_application_log_response(buf, "TCU_SPP_DATA_RECEIVE_EVENT", "length_of_data %d", event->length_of_data);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuSppDataSent_Event(uint8_t *buf)
{
  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_SPP_DATA_SEND_EVENT));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_SPP_DATA_SEND_EVENT));

  tcu_application_log_response(buf, "TCU_SPP_DATA_SEND_EVENT", "status %d", 0);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuSppUuidAssign_Request(struct tcu_uuid_assign_request *request)
{
  REQUEST_PREAMBLE();
  offset = CMD_ARRAY_PARAM_OFFS;

  write_array_8(buf, request->initiator_uuid_data_type, &offset);

  switch (request->initiator_uuid_data_type) {
  case TCU_SSP_UUID16:
    copy_array(&request->initiator_uuid_value[0], buf, 0, offset, 2);
    offset += 2;
    break;
  case TCU_SSP_UUID32:
    copy_array(&request->initiator_uuid_value[0], buf, 0, offset, 4);
    offset += 4;
    break;
  case TCU_SSP_UUID128:
    copy_array(&request->initiator_uuid_value[0], buf, 0, offset, 16);
    offset += 16;
    break;
  default:
    break;
  }

  write_array_8(buf, request->acceptor_uuid_data_type, &offset);
  switch (request->acceptor_uuid_data_type) {
  case TCU_SSP_UUID16:
    copy_array(&request->acceptor_uuid_value[0], buf, 0, offset, 2);
    offset += 2;
    break;
  case TCU_SSP_UUID32:
    copy_array(&request->acceptor_uuid_value[0], buf, 0, offset, 4);
    offset += 4;
    break;
  case TCU_SSP_UUID128:
    copy_array(&request->acceptor_uuid_value[0], buf, 0, offset, 16);
    offset += 16;
    break;
  default:
    break;
  }

  offset = set_cmd_header_user_length(buf, TCU_SPP_UUID_ASSIGN_REQ, offset - 7, UART_PACKET_OFFSET);
  status = tcu_send(buf);
  if (status) {
    return status;
  }

  tcu_application_log_request(buf, "TCU_SPP_CONNECT_REQ", "");

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuSppUuidAssign_Response(uint8_t *buf, struct tcu_spp_uuid_assign_response *response)
{
  uint16_t offset = CMD_ARRAY_PARAM_OFFS;

  assert(PARSE_CMD_SERVICE_ID(buf) == GET_CMD_SERVICE_ID(TCU_SPP_UUID_ASSIGN_RESP));
  assert(PARSE_CMD_OPCODE(buf) == GET_CMD_OPCODE(TCU_SPP_UUID_ASSIGN_RESP));

  response->status = (enum tcu_mng_status)buf[offset];

  tcu_application_log_response(buf, "TCU_SPP_UUID_ASSIGN_RESP", "status %d", response->status);

  return TCU_FUNC_SUCCESS;
}

enum tcu_func_status TcuSppDataTransfer_Request(struct tcu_spp_data_transfer_request *request)
{
  REQUEST_PREAMBLE();
  offset = CMD_ARRAY_PARAM_OFFS;

  write_array_16(buf, request->length_of_data, (uint16_t *)&offset);
  copy_array(request->data, buf, 0, offset, request->length_of_data);
  offset += request->length_of_data;

  offset = set_cmd_header_user_length(buf, TCU_SPP_DATA_TRANSFER_REQ, offset - 7, UART_PACKET_OFFSET);
  status = tcu_send(buf);
  if (status) {
    return status;
  }

  tcu_application_log_request(buf, "TCU_SPP_DATA_TRANSFER_REQ", "");

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
